#include <iostream>
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

