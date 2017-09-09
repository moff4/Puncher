#include <stdlib.h>
#include <iostream>
#include <string>

#include "puncher.h"

using namespace std;

void _print_help(void);
void parse_argv(int w,const char ** stt, string& filename,string& mod, string& conv);


/**
 * main; we start here;
 * That's all u need to know
 */
int main(int w,const char** stt)
{
    string filename = "test/test.hex";
    string mod = "hex";
    string conv = "";
    Puncher* machine = new Puncher(filename);
    
    machine -> load(mod);		// load code
    
    if (conv == "")
    {
        machine -> start();	// run code
    }
    else
    {
        machine -> convert("test/test.tmp",conv);
    }
    
    delete machine;
    return 0;
}
