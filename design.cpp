#include <iostream>
#include <iomanip>
#include "include/design.h"
#include <string>
#include <limits>

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
    for (int i = 0; i < c; i++)
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
#if WIN32
    system("cls");
#else
    system("clear");
#endif
}

void heading(string header,int c)
{
    cls();
    line(c);
    center((c - header.length()) / 2);
    cout << header << endl;
    line(c);
}

void centertxt(string ct,int l)
{
    center((l - ct.length()) / 2);
    cout << ct << endl;
    line(l);
}

void footer(string footbar)
{
    line();
    center((100 - footbar.length()) / 2);
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
        if (cin >> value && value >= minimum && value <= maximum)
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return true;
        }

        if (cin.eof())
            return false;

        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Enter a number from " << minimum << " to " << maximum << ".\n";
    }
}

bool readdouble(const string &prompt, double &value, double minimum, double maximum)
{
    while (true)
    {
        cout << prompt;
        if (cin >> value && value >= minimum && value <= maximum)
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return true;
        }

        if (cin.eof())
            return false;

        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Enter a value from " << minimum << " to " << maximum << ".\n";
    }
}
