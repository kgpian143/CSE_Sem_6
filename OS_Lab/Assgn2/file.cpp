#include <fstream>
#include <iostream>
#include <string>
#include <bits/stdc++.h>
using namespace std;
int main()
{
    vector<string> history;
    fstream new_file;
    new_file.open("/home/lenovo/Semester6/OS_lab/A2/shell_history.txt", ios::in);

    if (new_file.is_open())
    {
        string sa;
        while (getline(new_file, sa))
            history.push_back(sa);

        new_file.close();

        for (auto it : history)
            cout << it << endl;
    }

    new_file.open("/home/lenovo/Semester6/OS_lab/A2/shell_history.txt", ios::out);

    if (new_file.is_open())
    {
        for(auto it : history)
            new_file << (it+"\n"); 

        new_file.close();             
    }


    return 0;
}