#include <iostream>
#include <iomanip>
#include <cmath>
#include <algorithm>
#include "include/design.h"
#include <string>
#include <sstream>

using namespace std;

void line(int l)
{
    for (int i = 0; i < l; i++)
    {
        cout << "=";
    }
    cout << endl;
}

void center(int c)
{
    for (int i = 0; i < max(0, c); i++)
    {
        cout << " ";
    }
}

void ps()
{
    system("pause");
}

void cls()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void heading(string header,int c)
{
    cls();
    line(c);
    center(max(0, (c - static_cast<int>(header.length())) / 2));
    cout << header << endl;
    line(c);
}

void centertxt(string ct,int l)
{
    center(max(0, (l - static_cast<int>(ct.length())) / 2));
    cout << ct << endl;
    line(l);
}

void footer(string footbar)
{
    line();
    center(max(0, (100 - static_cast<int>(footbar.length())) / 2));
    cout << footbar << endl;
    line();
}

void fileline(ostream &out, int l)
{
    if (out.tellp() != 0)
    {
        out << endl;
    }
    for (int i = 0; i < l; i++)
    {
        out << "=";
    }
    out << endl;
}

void filecenter(ostream &out, int c)
{
    for (int i = 0; i < c; i++)
    {
        out << " ";
    }
}

void fileheader(ostream &out, string heading)
{
    fileline(out);
    filecenter(out, (120 - heading.length()) / 2);
    out << heading;
    fileline(out);
}

void filesubheader(ostream &out, int r, string subheading)
{
    out << right << setw(subheading.length() + r) << subheading << right << setw(r + 1) << "|";
}

bool readint(const string &prompt, int &value, int minimum, int maximum)
{
    while (true)
    {
        cout << prompt;
        string input;
        if (!getline(cin >> ws, input))
            return false;

        istringstream parser(input);
        int parsedValue;
        if (parser >> parsedValue)
        {
            parser >> ws;
            if (parser.eof() && parsedValue >= minimum && parsedValue <= maximum)
            {
                value = parsedValue;
                return true;
            }
        }

        cout << "Enter a number from " << minimum << " to " << maximum << ".\n";
    }
}

bool readmoney(const string &prompt, double &value, double minimum, double maximum)
{
    while (true)
    {
        cout << prompt;
        string input;
        if (!getline(cin >> ws, input))
            return false;

        size_t last = input.find_last_not_of(" \t\r");
        if (last != string::npos)
            input.erase(last + 1);
        size_t decimal = input.find('.');
        bool validPrecision = input.find_first_of("eE") == string::npos &&
                              (decimal == string::npos ||
                               (input.find('.', decimal + 1) == string::npos &&
                                input.size() - decimal - 1 <= 2));

        istringstream parser(input);
        double parsedValue;
        if (validPrecision && (parser >> parsedValue))
        {
            parser >> ws;
            if (parser.eof() && isfinite(parsedValue) &&
                parsedValue >= minimum && parsedValue <= maximum)
            {
                value = parsedValue;
                return true;
            }
        }

        cout << "Enter an amount with no more than two decimal places, from "
             << minimum << " to " << maximum << ".\n";
    }
}
