#include <iostream>
#include <iomanip>
#include "include/design.h"
#include <string>

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

void heading(string header)
{
    cls();
    line();
    center((100 - header.length()) / 2);
    cout << header << endl;
    line();
}

void centertxt(string ct)
{
    center((100 - ct.length()) / 2);
    cout << ct << endl;
    line();
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
    filecenter(out, (100 - heading.length()) / 2);
    out << heading << endl;
    fileline(out);
}

void filesubheader(ostream &out, int r, string subheading)
{
    out << right << setw(subheading.length() + r) << subheading << right << setw(r + 1) << "|";
}
