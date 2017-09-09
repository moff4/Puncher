#include <string>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>

#include "puncher.h"
#include "lord.h"
#include "types.h"

using namespace std;

/**
 * init
 * arg - name of file with code
 */
Puncher::Puncher(string st)
{
    this->old_filename = st;
    this->new_filename = "test/test.tmp";
    this->bytes = NULL;
}

/**
 * destruct
 */
Puncher::~Puncher()
{
    if (this->bytes!=NULL)
    {
        for (int i=0;this->bytes[i]!=NULL;i++)
        {
            delete this->bytes[i];
        }
        free(this->bytes);
    }
}
    
/**
 * loads code from file
 * arg - file encording {hex/boo/bin}
 */
bool Puncher::load(string mod)
{
    int fe = open(this->old_filename.c_str(),O_RDONLY);
    if (fe<0)
    {
        cout<<"Puncher: couldn't open file \""<<this->old_filename<<"\""<<endl;
        return true;
    }
    
    Lord* Vader = new Lord();
    Bytes ** bt;
    if (mod == "hex")
    {
        bt = Vader -> hex(fe);
    }
    else if (mod == "bin")
    {
        bt = Vader -> bin(fe);
    }
    else if (mod == "boo")
    {
        bt = Vader -> boo(fe);
    }
    
    this->bytes = bt;
    
    /*for(int i = 0;this->bytes[i]!=NULL;i++)
    {
        cout<<"load: "<<this->bytes[i]->val<<endl;
    }//*/
    
    close(fe);
    delete Vader;
    return bt == NULL;
}
    
/**
 * run code
 */
bool Puncher::start()
{
    for(int i = 0;this->bytes[i]!=NULL;i++)
    {
        cout<<"RUN: "<<this->bytes[i]->val<<endl;
    }
    return false;
}

/**
 * converts code to new encording
 * args - new filename ; new encording {hex/boo/bin}
 */
bool Puncher::convert(string filename,string mod)
{
    int fe = open(filename.c_str(),O_WRONLY|O_CREAT|O_TRUNC,0644);
    if (fe<0)
    {
        cout<<"couldn't open file to write \""<<filename<<"\""<<endl;
        return true;
    }
    if (mod == "hex")
    {
        char st[2048];
        #ifdef __linux__
        const char * __mod = "%s%02x";
        #else
        const char * __mod = "%s%02x";
        #endif
        for (int i=0;this->bytes[i]!=NULL;i++)
        {
            strcpy(st,"");
            for (int j=0;j<8;j++)
            {
                
                sprintf(st,__mod,st,_byte(this->bytes[i]->val,j));
                if (j!=7)
                {
                    sprintf(st,"%s ",st);
                }
            }
            sprintf(st,"%s\n",st);
            write(fe,st,strlen(st));
        }
    }
    else if (mod == "boo")
    {
        const char * __mod = "%d%s";
        string st;
        for (int i=0;this->bytes[i]!=NULL;i++)
        {
            st = "";
            _u64 x = this->bytes[i]->val;
            for (int j=0;j<64;j++)
            {
                st = (char)(x%2+'0') + st;
                if (((j%4) == 3)&&(j!=63))
                {
                    st = " " + st;
                }
                x=x>>1;
            }
            st += "\n";
            write(fe,st.c_str(),st.length());
        }
    }
    else if (mod == "bin")
    {
        for (int i=0;this->bytes[i]!=NULL;i++)
        {
            for (int j=7;j>=0;j--)
            {
                _u8 x = _byte(this->bytes[i]->val,j);
                write(fe,&x,1);
            }
        }
    }
    
    return false;
}