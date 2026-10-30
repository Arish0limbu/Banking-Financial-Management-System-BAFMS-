const fs = require("node:fs");
const http = require("node:http");
const path = require("node:path");
const { spawnSync } = require("node:child_process");

const projectRoot = path.resolve(__dirname, "..");
const runtimeDirectory = path.join(__dirname, "runtime");
const runtimeDataDirectory = path.join(runtimeDirectory, "data");
const sampleDataDirectory = path.join(__dirname, "sample-data");
const apiExecutable = path.join(projectRoot, "web-api.exe");
const port = Number(process.env.BAFMS_WEB_PORT || 4173);

fs.mkdirSync(runtimeDataDirectory, { recursive: true });
for (const fileName of ["accounts.txt", "transactions.txt"]) {
  const runtimeFile = path.join(runtimeDataDirectory, fileName);
  if (!fs.existsSync(runtimeFile))
    fs.copyFileSync(path.join(sampleDataDirectory, fileName), runtimeFile);
}

const staticFiles = new Map([
  ["/", ["index.html", "text/html; charset=utf-8"]],
  ["/styles.css", ["styles.css", "text/css; charset=utf-8"]],
  ["/app.js", ["app.js", "text/javascript; charset=utf-8"]],
]);

function sendJson(response, status, data) {
  response.writeHead(status, {
    "Content-Type": "application/json; charset=utf-8",
    "Cache-Control": "no-store",
    "X-Content-Type-Options": "nosniff",
  });
  response.end(JSON.stringify(data));
}

function runCppApi(args) {
  const result = spawnSync(apiExecutable, args, {
    cwd: runtimeDirectory,
    encoding: "utf8",
    timeout: 5000,
    windowsHide: true,
  });

  if (result.error) {
    throw new Error("Could not start the C++ API. Run start-web-demo.bat first.");
  }

  let data;
  try {
    data = JSON.parse(result.stdout || "{}");
  } catch {
    throw new Error("The C++ API returned an invalid response.");
  }

  if (result.status !== 0) {
    const error = new Error(data.error || "The C++ API could not complete the request.");
    error.status = 400;
    throw error;
  }
  return data;
}

function readRequestBody(request) {
  return new Promise((resolve, reject) => {
    let body = "";
    request.setEncoding("utf8");
    request.on("data", (chunk) => {
      body += chunk;
      if (body.length > 16_384) {
        reject(new Error("Request body is too large."));
        request.destroy();
      }
    });
    request.on("end", () => {
      try {
        resolve(JSON.parse(body));
      } catch {
        reject(new Error("Send a valid JSON request."));
      }
    });
    request.on("error", reject);
  });
}

function parseAccountNumber(value) {
  const text = String(value ?? "");
  if (!/^\d+$/.test(text))
    return null;
  const number = Number(text);
  return Number.isSafeInteger(number) && number > 0 ? number : null;
}

async function handleApi(request, response, url) {
  try {
    if (request.method === "GET" && url.pathname === "/api/accounts") {
      sendJson(response, 200, runCppApi(["accounts"]));
      return;
    }
    if (request.method === "GET" && url.pathname === "/api/transactions") {
      sendJson(response, 200, runCppApi(["transactions"]));
      return;
    }
    if (request.method !== "POST" || url.pathname !== "/api/transactions") {
      sendJson(response, 404, { error: "API route was not found." });
      return;
    }

    const body = await readRequestBody(request);
    const type = String(body.type || "").toUpperCase();
    const accountNumber = parseAccountNumber(body.accountNumber);
    const destinationNumber = type === "TRANSFER"
      ? parseAccountNumber(body.destinationAccountNumber)
      : 0;
    const amountText = String(body.amount ?? "").trim();
    if (!accountNumber ||
        (type !== "DEPOSIT" && type !== "WITHDRAWAL" && type !== "TRANSFER") ||
        (type === "TRANSFER" && !destinationNumber) ||
        !/^\d+(?:\.\d{1,2})?$/.test(amountText)) {
      sendJson(response, 400, { error: "Check the transaction type, account, and amount." });
      return;
    }

    const amount = Number(amountText);
    if (!Number.isFinite(amount) || amount < 0.01 || amount > 1_000_000_000) {
      sendJson(response, 400, { error: "Amount must be between 0.01 and 1,000,000,000.00." });
      return;
    }
    const cents = Math.round(amount * 100);
    const result = runCppApi([
      "transaction",
      type,
      String(accountNumber),
      String(destinationNumber),
      String(cents),
    ]);
    sendJson(response, 200, result);
  } catch (error) {
    sendJson(response, error.status || 500, { error: error.message });
  }
}

const server = http.createServer((request, response) => {
  const url = new URL(request.url, "http://127.0.0.1");
  if (url.pathname.startsWith("/api/")) {
    handleApi(request, response, url);
    return;
  }

  if (request.method !== "GET" || !staticFiles.has(url.pathname)) {
    response.writeHead(404, { "Content-Type": "text/plain; charset=utf-8" });
    response.end("Not found");
    return;
  }

  const [fileName, contentType] = staticFiles.get(url.pathname);
  const filePath = path.join(__dirname, fileName);
  fs.readFile(filePath, (error, contents) => {
    if (error) {
      response.writeHead(500, { "Content-Type": "text/plain; charset=utf-8" });
      response.end("Unable to load the demo page.");
      return;
    }
    response.writeHead(200, {
      "Content-Type": contentType,
      "X-Content-Type-Options": "nosniff",
      "Content-Security-Policy": "default-src 'self'; style-src 'self'; script-src 'self'; connect-src 'self'",
    });
    response.end(contents);
  });
});

server.listen(port, "127.0.0.1", () => {
  console.log(`BAFMS local demo: http://127.0.0.1:${port}`);
  console.log("Demo data is isolated in web-demo/runtime/data.");
  console.log("Press Ctrl+C to stop the server.");
});
