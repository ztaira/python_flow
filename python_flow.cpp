// cout
#include <iostream>
// file streams
#include <fstream>
// vectors and strings
#include <vector>
#include <string>
// regex
#include <regex>
// setw
#include <iomanip>
using namespace std;

// global variable with file in it
// q: but what if the file's too big?
// a: i'll deal with that if I have time
vector<string> line_array;

int scan_function(string func_name, int level);

int main(int argc, char *argv[])
{
    // where to start and end the scan
    int start;
    int end;
    int level=0;

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
    
    // search through vector for 'if __name__ == '__main__':
    // set the start to that line
    // set the end to the bottom of the file
    cout << "Main func string found on line(s):" << endl;
    regex main_func ("if __name__ == .__main__.:");
    smatch match_results;
    end = line_array.size();
    for (int i=0; i<line_array.size(); ++i)
    {
        if (regex_search(line_array[i], match_results, main_func))
        {
            start = i;
            cout << i << endl;
        }
    }
    cout << "Scan start: " << start << endl << "Scan end: " << end << endl;

    // scan from start to end for a function call
    // types to account for:
    // system call, e.g. len(string)
    // object declaration, e.g. x = object()
    // object method, e.g. x = object.obj_func()
    // if one is found, go scan that one too
    cout << "Starting scan:" << endl << endl;
    cout << level << "if __name__ == .__main__.:" << endl;
    regex func ("(\\S+)\\(.*\\)");
    for (int i=start; i<end; ++i)
    {
        if (regex_search(line_array[i], match_results, func))
        {
            cout << right << setw(10) << level+1 << match_results[1] << endl;
            scan_function(match_results[1], level+1);
        }
    }

    workfile.close();
    return 1;
}

int scan_function(string func_name, int level)
{
    // set variables to hold start, end
    int start = -1;
    int end = -1;
    // regex to match function declaration
    regex declaration ("def "+func_name+"\\(");
    // regex to match function call
    regex func ("(\\S+)\\(.*\\)");
    smatch match_results;

    // set the start variable to function declaration
    for (int i=0; i<line_array.size(); ++i)
    {
        if (regex_search(line_array[i], match_results, declaration))
        {
            start = i;
        }
    }
    if (start == -1)
    {
        return 0;
    }
    
    // regex to match end
    regex ender ('^'+match_results[1].str()+"\\S");
    // set the end variable to when the indent is less
    for (int i=start+1; i<line_array.size(); ++i)
    {
        if (regex_search(line_array[i], match_results, ender))
        {
            if (i < end || end == -1)
            {
                end = i;
            }
        }
    }

    // scan inside this function for any functions
    for (int i=start+1; i<end; ++i)
    {
        if (regex_search(line_array[i], match_results, func))
        {
            cout << right << setw(10) << level+1 << match_results[1] << endl;
            scan_function(match_results[1], level+1);
        }
    }
    
    return 1;
}
