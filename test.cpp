#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

int main()
{
    ofstream file("data/customer.txt", ios::app);

    file << right << setw(3) << "SN" << right <<setw(3) <<'|';

        

    return 0;
}