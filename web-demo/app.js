const accountsBody = document.querySelector("#accounts-body");
const transactionsBody = document.querySelector("#transactions-body");
const accountSelect = document.querySelector("#account-number");
const destinationSelect = document.querySelector("#destination-account");
const typeSelect = document.querySelector("#transaction-type");
const destinationField = document.querySelector("#destination-field");
const accountLabel = document.querySelector("#account-label");
const form = document.querySelector("#transaction-form");
const submitButton = document.querySelector("#submit-transaction");
const formMessage = document.querySelector("#form-message");

const moneyFormat = new Intl.NumberFormat("en-NP", {
  style: "currency",
  currency: "NPR",
  minimumFractionDigits: 2,
  maximumFractionDigits: 2,
});

function formatMoney(amount) {
  return moneyFormat.format(Number(amount) || 0);
}

function makeCell(text, className = "") {
  const cell = document.createElement("td");
  if (className)
    cell.className = className;
  cell.textContent = text;
  return cell;
}

function makeEmptyRow(tableBody, columns, message) {
  const row = document.createElement("tr");
  const cell = makeCell(message, "empty-state");
  cell.colSpan = columns;
  row.append(cell);
  tableBody.replaceChildren(row);
}

function setAccountOptions(accounts) {
  const activeAccounts = accounts.filter((account) => account.active);
  const previousAccount = accountSelect.value;
  const previousDestination = destinationSelect.value;

  for (const select of [accountSelect, destinationSelect]) {
    const placeholder = document.createElement("option");
    placeholder.value = "";
    placeholder.textContent = activeAccounts.length ? "Choose an account" : "No active accounts";
    select.replaceChildren(placeholder);
    for (const account of activeAccounts) {
      const option = document.createElement("option");
      option.value = String(account.accountNumber);
      option.textContent = `${account.accountNumber} · ${account.type} · ${formatMoney(account.balance)}`;
      select.append(option);
    }
  }

  if (activeAccounts.some((account) => String(account.accountNumber) === previousAccount))
    accountSelect.value = previousAccount;
  if (activeAccounts.some((account) => String(account.accountNumber) === previousDestination))
    destinationSelect.value = previousDestination;

  accountSelect.disabled = activeAccounts.length === 0;
  destinationSelect.disabled = activeAccounts.length < 2;
  updateTransactionFields();
}

function renderAccounts(accounts) {
  const activeAccounts = accounts.filter((account) => account.active);
  const totalBalance = activeAccounts.reduce((total, account) => total + Number(account.balance), 0);
  document.querySelector("#total-balance").textContent = formatMoney(totalBalance);
  document.querySelector("#active-account-count").textContent = String(activeAccounts.length);
  document.querySelector("#account-count-caption").textContent = `${accounts.length} account${accounts.length === 1 ? "" : "s"} in the register`;
  document.querySelector("#accounts-table-count").textContent = String(accounts.length);

  if (!accounts.length) {
    makeEmptyRow(accountsBody, 5, "No account records found in the demo data.");
    setAccountOptions(accounts);
    return;
  }

  const rows = accounts.map((account) => {
    const row = document.createElement("tr");
    const number = document.createElement("td");
    const numberText = document.createElement("span");
    numberText.className = "account-number";
    numberText.textContent = String(account.accountNumber);
    const sub = document.createElement("span");
    sub.className = "account-sub";
    sub.textContent = `Account ${account.accountNumber}`;
    number.append(numberText, sub);
    row.append(number);
    row.append(makeCell(String(account.customerSerial)));
    row.append(makeCell(account.type));
    row.append(makeCell(formatMoney(account.balance)));

    const statusCell = document.createElement("td");
    const status = document.createElement("span");
    status.className = `status-pill${account.active ? "" : " is-inactive"}`;
    status.textContent = account.active ? "Active" : "Closed";
    statusCell.append(status);
    row.append(statusCell);
    return row;
  });
  accountsBody.replaceChildren(...rows);
  setAccountOptions(accounts);
}

function displayTimestamp(timestamp) {
  if (!timestamp || timestamp === "unknown")
    return "—";
  return timestamp;
}

function renderTransactions(transactions) {
  document.querySelector("#transaction-count").textContent = String(transactions.length);
  document.querySelector("#activity-table-count").textContent = String(transactions.length);
  if (!transactions.length) {
    makeEmptyRow(transactionsBody, 4, "No transactions recorded yet.");
    return;
  }

  const rows = [...transactions].reverse().slice(0, 8).map((transaction) => {
    const row = document.createElement("tr");
    const type = makeCell(transaction.type.replaceAll("_", " "), "transaction-type");
    type.textContent += `  #${transaction.transactionId}`;

    let accountText = "—";
    if (transaction.type === "TRANSFER")
      accountText = `${transaction.fromAccount} → ${transaction.toAccount}`;
    else if (transaction.type === "DEPOSIT")
      accountText = `To ${transaction.toAccount}`;
    else
      accountText = `From ${transaction.fromAccount}`;

    const amountClass = transaction.type === "DEPOSIT"
      ? "amount-positive"
      : transaction.type === "TRANSFER" ? "" : "amount-negative";
    row.append(type, makeCell(accountText), makeCell(formatMoney(transaction.amount), amountClass), makeCell(displayTimestamp(transaction.timestamp)));
    return row;
  });
  transactionsBody.replaceChildren(...rows);
}

async function getJson(url, options) {
  const response = await fetch(url, options);
  const payload = await response.json();
  if (!response.ok)
    throw new Error(payload.error || "The local server could not complete the request.");
  return payload;
}

async function refreshDashboard() {
  try {
    const [accountData, transactionData] = await Promise.all([
      getJson("/api/accounts"),
      getJson("/api/transactions"),
    ]);
    renderAccounts(accountData.accounts);
    renderTransactions(transactionData.transactions);
  } catch (error) {
    makeEmptyRow(accountsBody, 5, "Could not load accounts.");
    makeEmptyRow(transactionsBody, 4, "Could not load transactions.");
    showMessage(error.message, true);
  }
}

function showMessage(message, isError = false) {
  formMessage.textContent = message;
  formMessage.classList.toggle("is-error", isError);
}

function updateTransactionFields() {
  const isTransfer = typeSelect.value === "TRANSFER";
  destinationField.classList.toggle("is-hidden", !isTransfer);
  accountLabel.textContent = typeSelect.value === "DEPOSIT"
    ? "Deposit to account"
    : typeSelect.value === "WITHDRAWAL" ? "Withdraw from account" : "Transfer from account";
  destinationSelect.required = isTransfer;
  submitButton.disabled = accountSelect.disabled || (isTransfer && destinationSelect.disabled);
}

typeSelect.addEventListener("change", updateTransactionFields);
document.querySelector("#refresh-button").addEventListener("click", refreshDashboard);

form.addEventListener("submit", async (event) => {
  event.preventDefault();
  showMessage("");
  submitButton.disabled = true;
  submitButton.textContent = "Saving…";
  try {
    const formData = new FormData(form);
    const result = await getJson("/api/transactions", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({
        type: formData.get("type"),
        accountNumber: formData.get("accountNumber"),
        destinationAccountNumber: formData.get("destinationAccountNumber"),
        amount: formData.get("amount"),
      }),
    });
    showMessage(result.message || "Transaction saved.");
    document.querySelector("#amount").value = "";
    await refreshDashboard();
  } catch (error) {
    showMessage(error.message, true);
  } finally {
    submitButton.innerHTML = "Submit transaction <span aria-hidden=\"true\">→</span>";
    updateTransactionFields();
  }
});

document.querySelector("#today-label").textContent = new Intl.DateTimeFormat("en", {
  weekday: "long",
  month: "long",
  day: "numeric",
  year: "numeric",
}).format(new Date()).toUpperCase();

updateTransactionFields();
refreshDashboard();
