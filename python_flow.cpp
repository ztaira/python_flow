// cout
#include <iostream>
// file streams
#include <fstream>
// vectors and strings
#include <vector>
#include <string>
// regex
#include <regex>
// stack
#include <stack>
// setw
#include <iomanip>
using namespace std;

// global variable with file in it
// q: but what if the file's too big?
// a: i'll deal with that if I have time
vector<string> line_array;
stack<int> jump_back;

void read_in_workfile(string file_name);
void equalize_line_lengths(int length);
int get_start_line();
int scan_function(string func_name, int level, int lastpos);
int get_def_line(string func_name, int lastpos);
int get_def_end(int start);
int draw_execution(int start, int end);
int draw_jump(int start, int end);
void draw_jump_endpoints(int start, int end, char direction);
void draw_jump_trace(int start, int end);
void print_flow();

int main(int argc, char *argv[])
{
    // where to start and end the scan
    int start;
    int end;
    int level=0;
    int last_position;
    // read in file to vector of strings
    read_in_workfile(argv[1]);
    //equalize line lengths
    equalize_line_lengths(40);
    // search through vector for 'if __name__ == '__main__':
    // set the start to that line
    // set the end to the bottom of the file
    cout << "Main func string found on line(s):" << endl;
    end = line_array.size();
    start = get_start_line();
    cout << "Scan start: " << start+1 << endl << "Scan end: " << end << endl;
    // scan from start to end for a function call
    // if one is found, recursively scan that one too
    last_position = start;
    cout << "Starting scan:" << endl << endl;
    regex func ("(\\S+)\\(.*\\)");
    smatch match_results;
    for (int i=start; i<end; ++i)
    {
        if (regex_search(line_array[i], match_results, func))
        {
            draw_execution(last_position, i);
            last_position=i;
            // cout << level+1 << match_results[1] << endl;
            jump_back.push(i);
            scan_function(match_results[1], level+1, i);
        }
    }
    // trace to end of file
    draw_execution(last_position, end-1);
    // print out the line_array
    print_flow();
    return 1;
}

// returns the line where the "if __name__ == __main__:" function is at
int get_start_line()
{
    int start = 0;
    regex main_func ("if __name__ == .__main__.:");
    smatch match_results;
    for (int i=0; i<line_array.size(); ++i)
    {
        if (regex_search(line_array[i], match_results, main_func))
        {
            start = i;
            cout << i+1 << endl;
        }
    }
    return start;
}

// reads in a file and puts it in the global line_array
void read_in_workfile(string file_name)
{
    ifstream workfile (file_name);
    string line;
    while ( getline (workfile,line) )
    {   
        line_array.push_back(line);
    }
    workfile.close();
}

//makes all lengths in line_array equal
void equalize_line_lengths(int length)
{
    // equalize lengths of vector strings
    // if string is less than 80, add spaces
    // if string is greater than 80, cut to 77 and add '...' to the end
    for (vector<string>::iterator itr = line_array.begin();
         itr != line_array.end(); ++itr)
    {
        while (itr->length() < length)
        {
            itr->push_back(' ');
        }
        if (itr->length() > length)
        {
            while (itr->length() > length-3)
            {
                itr->pop_back();
            }
            itr->push_back('.');
            itr->push_back('.');
            itr->push_back('.');
        }
    }
}

int scan_function(string func_name, int level, int lastpos)
{
    // set variables to hold start, end
    int start = -1;
    int end = -1;
    // regex to match function call
    regex func ("(\\S+)\\(.*\\)");
    smatch match_results;

    // set the start to equal the line where func_name is defined
    start = get_def_line(func_name, lastpos);
    lastpos = start;
    if (start == -1)
    {
        return 0;
    }
    
    // set the end to the line  when the indent is less
    end = get_def_end(start);

    // scan inside func_name for any functions
    for (int i=start+1; i<end; ++i)
    {
        if (regex_search(line_array[i], match_results, func))
        {
            draw_execution(lastpos, i);
            lastpos = i;
            // cout << level+1 << match_results[1] << endl;
            jump_back.push(i);
            scan_function(match_results[1], level+1, i);
        }
    }

    draw_execution(lastpos, end-1);
    draw_jump(end-1, jump_back.top());
    jump_back.pop();
    
    return end;
}

int get_def_line(string func_name, int lastpos)
{
    int start = -1;
    // regex to match function declaration
    regex declaration ("def "+func_name+"\\(");
    smatch match_results;
    // set the start variable to function declaration
    for (int i=0; i<line_array.size(); ++i)
    {
        if (regex_search(line_array[i], match_results, declaration))
        {
            draw_jump(lastpos, i);
            lastpos = i;
            start = i;
        }
    }
    return start;
}

int get_def_end(int start)
{
    int end;
    // regex to match end
    smatch match_results;
    regex ender ("^\\S");
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
    return end;
}

// * 
// |
// |
// *
int draw_execution(int start, int end)
{
    for (int i=0; i<line_array.size(); ++i)
    {
        if (i > start and i < end)
        {
            line_array[i].push_back('|');
        }
        else if (i == start or i == end)
        {
            line_array[i].push_back('*');
        }
        else
        {
            line_array[i].push_back(' ');
        }
    }
    return 1;
}

// .v  >.
//  :  :
//  >..^
int draw_jump(int start, int end)
{
    // set direction
    char direction = ' ';
    int switchvar;
    if (start < end)
    {
        direction = 'v';
    }
    else
    {
        direction = '^';
    }
    
    // draw start and end lines
    draw_jump_endpoints(start, end, direction);

    if (end < start)
    {
        switchvar = start;
        start = end;
        end = switchvar;
    }

    // draw everything else
    draw_jump_trace(start, end);

    return 1;
}

void draw_jump_endpoints(int start, int end, char direction)
{
    line_array[start].push_back('.');
    line_array[start].push_back(direction);
    line_array[start].push_back(' ');

    line_array[end].push_back(' ');
    line_array[end].push_back('>');
    line_array[end].push_back('.');
}

void draw_jump_trace(int start, int end)
{
    for (int i=0; i<line_array.size(); ++i)
    {
        if (i != start and i != end)
        {
            if (i > start and i < end)
            {
                line_array[i].push_back(' ');
                line_array[i].push_back(':');
                line_array[i].push_back(' ');
            }
            else
            {
                line_array[i].push_back(' ');
                line_array[i].push_back(' ');
                line_array[i].push_back(' ');
            }
        }
    }
}

void print_flow()
{
    for (int i=0; i<line_array.size(); ++i)
    {
        cout << left << setw(4) << i+1 << line_array[i] << endl;
    }
}
