#include <iostream>

using namespace std;

void line(int l = 100)
{
    for (int i = 0; i < l; i++)
    {
        cout << "=";
    }
    cout << endl;
}

void center(int c = 45)
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
    cout << header;
    line();
}

