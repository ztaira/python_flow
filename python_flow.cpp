#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

vector<string> line_array;

int main(int argc, char *argv[])
{

    // read in file to vector of strings
    ifstream workfile (argv[1]);
    string line;
    while ( getline (workfile,line) )
    {   
        line_array.push_back(line);
    }

    // equalize lengths of vector strings
    // if string is less than 80, add spaces
    // if string is greater than 80, cut to 77 and add '...' to the end
    for (vector<string>::iterator itr = line_array.begin();
         itr != line_array.end(); ++itr)
    {
        while (itr->length() < 80)
        {
            itr->push_back(' ');
        }
        if (itr->length() > 80)
        {
            while (itr->length() > 77)
            {
                itr->pop_back();
            }
            itr->push_back('.');
            itr->push_back('.');
            itr->push_back('.');
        }
    }

    for (vector<string>::iterator itr = line_array.begin();
         itr != line_array.end(); ++itr)
    {
        if (!itr->empty())
        {
            cout << itr->length() << ' ' << *itr << 'x' << endl;
        }
    }

    workfile.close();
    return 1;
}
