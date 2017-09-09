#include <stdlib.h>
#include <iostream>
#include <string>

#include "puncher.h"

using namespace std;

void _print_help(void);
void parse_argv(int w,const char ** stt, string& filename,string& mod, string& conv, string& new_filename);


/**
 * main; we start here;
 * That's all u need to know
 */
int main(int w,const char** stt)
{
    
    string filename     = "";
    string new_filename = "";
    string mod          = "";
    string conv         = "";
    
    parse_argv(w,stt,filename,mod,conv,new_filename);
    if (filename == "")
    {
        cout<<"expected filename"<<endl;
        exit(1);
    }
    
    Puncher* machine = new Puncher(filename);
    
    machine -> load(mod);		// load code
    
    if (conv == "")
    {
        machine -> start();	// run code
    }
    else
    {
        machine -> convert(new_filename,conv); // convert code
    }
    
    delete machine;
    
    return 0;
}

/**
 * argv parser;
 * change input args to special meanings
 */
void parse_argv(int w,const char ** stt, string& filename,string& mod, string& conv, string& new_filename)
{
    mod = "hex";
    bool _mod 		= false;
    bool _filename  = false;
    bool _conv		= false;
    string st;
    for (int i =1;i<w;i++)
    {
        st = string(stt[i]);
        if (st == "-h")
        {
            if (!_mod)
            {
                mod = "hex";
                _mod = true;
            }
            else
            {
                cout<<"unexpected flag: "<<stt[i]<<endl;
                exit(1);
            }
        }
        else if (st == "-0")
        {
            if (!_mod)
            {
                mod = "boo";
                _mod = true;
            }
            else
            {
                cout<<"unexpected flag: "<<stt[i]<<endl;
                exit(1);
            }
        }
        else if (st == "-b")
        {
            if (!_mod)
            {
                mod = "bin";
                _mod = true;
            }
            else
            {
                cout<<"unexpected flag: "<<stt[i]<<endl;
                exit(1);
            }
        }
        else if (st == "--conv-hex")
        {
            if (!_conv)
            {
                conv = "hex";
                _conv = true;
                if ((i +1) != w)
                {
                    i++;
                    new_filename = stt[i];
                    if (new_filename.length()<=0)
                    {
                        cout<<"expected new filename; got nothing; try flag \"-?\" or \"--help\""<<endl;
                        exit(1);
                    }
                    if (new_filename[0]=='-')
                    {
                        cout<<"expected new filename; got nothing; try flag \"-?\" or \"--help\""<<endl;
                        exit(1);
                    }
                }
                else
                {
                    cout<<"expected new filename; got nothing; try flag \"-?\" or \"--help\""<<endl;
                    exit(1);
                }
            }
            else
            {
                cout<<"unexpected flag: "<<stt[i]<<endl;
                exit(1);
            }
        }
        else if (st == "--conv-boo")
        {
            if (!_conv)
            {
                conv = "boo";
                _conv = true;
                if ((i +1) != w)
                {
                    i++;
                    new_filename = stt[i];
                    if (new_filename.length()<=0)
                    {
                        cout<<"expected new filename; got nothing; try flag \"-?\" or \"--help\""<<endl;
                        exit(1);
                    }
                    if (new_filename[0]=='-')
                    {
                        cout<<"expected new filename; got nothing; try flag \"-?\" or \"--help\""<<endl;
                        exit(1);
                    }
                }
                else
                {
                    cout<<"expected new filename; got nothing; try flag \"-?\" or \"--help\""<<endl;
                    exit(1);
                }
            }
            else
            {
                cout<<"unexpected flag: "<<stt[i]<<endl;
                exit(1);
            }
        }
        else if (st == "--conv-bin")
        {
            if (!_conv)
            {
                conv = "bin";
                _conv = true;
                if ((i +1) != w)
                {
                    i++;
                    new_filename = stt[i];
                    if (new_filename.length()<=0)
                    {
                        cout<<"expected new filename; got nothing; try flag \"-?\" or \"--help\""<<endl;
                        exit(1);
                    }
                    if (new_filename[0]=='-')
                    {
                        cout<<"expected new filename; got nothing; try flag \"-?\" or \"--help\""<<endl;
                        exit(1);
                    }
                }
                else
                {
                    cout<<"expected new filename; got nothing; try flag \"-?\" or \"--help\""<<endl;
                    exit(1);
                }
            }
            else
            {
                cout<<"unexpected flag: "<<stt[i]<<endl;
                exit(1);
            }
        }
        else if ((st == "-?") || (st == "--help"))
        {
            _print_help();
            exit(0);
        }
        else
        {
            if (!_filename)
            {
                filename = stt[i];
                _filename = true;
            }
            else
            {
                cout<<"expect only one filename: "<<stt[i]<<endl;
                exit(1);
            }
        }
    }
}

/**
 * print help info
 */
void _print_help(void)
{
    cout<<"Puched card emulator"<<endl;
    cout<<"[filename] [ -h | -0 | -b ] [ [ --conv-hex | --conv-boo | --conv-bin ] new_filename ]"<<endl;
    cout<<" filename - name of file with code"<<endl;
    cout<<" -h - code encording is hex (default)"<<endl;
    cout<<" -0 - code encording contains only 0 and 1"<<endl;
    cout<<" -b - code has binary encording"<<endl;
    cout<<" --conv-hex - convert code to hex"<<endl;
    cout<<" --conv-boo - convert code to {0|1}"<<endl;
    cout<<" --conv-bin - convert code to binary"<<endl;
    cout<<"new_filename - name of file for convertor input"<<endl;
    cout<<"Extra info at https://github.com/moff4/Puncher"<<endl;
}