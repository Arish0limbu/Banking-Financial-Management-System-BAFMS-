#ifndef DESGIN_H
#define DESGIN_H

#include <string>
#include <iostream>

using namespace std;

void line(int l = 100);

void center(int c = 45);

void ps();

void cls();

void heading(string header);

void centertxt(string ct);

void footer(string footbar);

void fileline(ostream &out, int l = 120);

void filecenter(ostream &out, int c = 45);

void fileheader(ostream &out, string heading);

void filesubheader(ostream &out, int r ,string heading);
#endif