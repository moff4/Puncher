#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>

#include "lord.h"

using namespace std;

/**
 * read line from fd;
 * if fd is empty return ""
 */
string Lord::readline(int fe)
{
    string st = "";
    unsigned char q;
    int i;
    while ((i = read(fe,&q,1))!=0)
    {
        if (i==0)
        {
            if (st.length()>0)
            {
                if (st[0]!='#')
                {
                    return st;
                }
                else
                {
                    return "";
                }
            }
            else
            {
                return "";
            }
        }
        if ((q != ' ') and (q != '\t'))
        {
            if ((q != '\n')and(q!='\r'))
            {
                if (q=='#')
                {
                    while ((read(fe,&q,1)!=0) && ((q!='\r')&&(q!='\n')));
                    if (st.length()>0)
                    {
                        return st;
                    }
                    else
                    {
                        return this->readline(fe);
                    }
                }
                
                if (((q>='A')and(q<='F'))or((q>='a')and(q<='f'))or((q>='0')and(q<='9')))
                {
                    st += q;
                }
                else
                {
                    cout<< string("got unexpected char: ") << (int)q << endl;
                    return st;
                }
            }
            else if (st[0] == '#')
            {
                return this->readline(fe);
            }
            else if (st.length()>0)
            {
                return st;
            }
            else
            {
                return this->readline(fe);
            }
        }
    }
    return st;
}

/**
 * load data with different encordings
 * arg - file discriptor with read right
 */
Bytes ** Lord::hex(int fe)
{
    if (fe<0)
    {
        cout<<"lord hex: pipe error"<<endl;
        return NULL;
    }
    
    _u64 all=4,num=0;
    Bytes ** bt = (Bytes**)calloc(all,sizeof(Bytes*));
    string st;
    int i=0;
    while ((st = readline(fe))!="")
    {
        //cout << "lord_hex: " << st << endl;
        
        bt[num] = new Bytes();
        
        _u8 q;
        _u64 k = 1,_k = 16;
        for (int j=st.length()-1;j>=0;j--)
        {
            if ((st[j]>='0')and(st[j]<='9'))
            {
                q = st[j] - '0';
            }
            else
            {
                q = st[j] - 'A' + 10;
            }
            bt[num]->val += (q * k);
            k*=_k;
        }
        bt[num]->empty = false;
        
        bt[num+1] = NULL;
        
        num +=1;
        if ((num + 1) == all)
        {
            all *= 2;
            bt = (Bytes**)realloc(bt,all*sizeof(Bytes*));
        }
        i++;
    }
    /*for (i=0;bt[i]!=NULL;i++)
    {
        cout << "LORD_HEX (bt): "<<bt[i]->val<<endl;
    }//*/
    
    if (num == 0)
    {
        free(bt);
        bt = NULL;
    }
    return bt;
}

/**
 * load data with different encordings
 * arg - file discriptor with read right
 */
Bytes ** Lord::boo(int fe)
{
    if (fe<0)
    {
        cout<<"lord hex: pipe error"<<endl;
        return NULL;
    }
    
    _u64 all=4,num=0;
    Bytes ** bt = (Bytes**)calloc(all,sizeof(Bytes*));
    string st;
    int i=0;
    while ((st = readline(fe))!="")
    {
        //cout << "lord_hex: " << st << endl;
        
        bt[num] = new Bytes();
        
        _u8 q;
        _u64 k = 1,_k = 2;
        for (int j=st.length()-1;j>=0;j--)
        {
            if ((st[j]=='0')or(st[j]=='1'))
            {
                q = st[j] - '0';
            }
            else
            {
                cout << "expected 0 or 1; got #"<<(int)st[j]<<endl;
                delete bt[num];
                bt[num] = NULL;
                for (int i=0;bt[i]!=NULL;i++)
                {
                    delete bt[i];
                }
                free(bt);
                bt = NULL;
                return bt;
            }
            bt[num]->val += (q * k);
            k*=_k;
        }
        bt[num]->empty = false;
        
        bt[num+1] = NULL;
        
        num +=1;
        if ((num + 1) == all)
        {
            all *= 2;
            bt = (Bytes**)realloc(bt,all*sizeof(Bytes*));
        }
        i++;
    }
    /*for (i=0;bt[i]!=NULL;i++)
    {
        cout << "LORD_HEX (bt): "<<bt[i]->val<<endl;
    }//*/
    
    if (num == 0)
    {
        free(bt);
        bt = NULL;
    }
    return bt;
}

/**
 * load data with different encordings
 * arg - file discriptor with read right
 */
Bytes ** Lord::bin(int fe)
{
    _u64 all=4,num=0,_dat;
    _u8 q, j = 0;
    Bytes ** bt = (Bytes**)calloc(all,sizeof(Bytes*));
    while (read(fe,&q,1)>0)
    {
        _dat = ( _dat << 8 ) + q;
        j++;
        if (j==8)
        {
            bt[num] = new Bytes();
            bt[num] -> val = _dat;
            num++;
            bt[num] = NULL;
            if ((num + 1) == all)
            {
                bt = (Bytes**)realloc(bt,(all*=2)*sizeof(Bytes*));
            }
            j=0;
        }
    }
    if (j!=0)
    {
        for (int i=0;bt[i]!=NULL;i++)
        {
            delete bt[i];
        }
        free(bt);
        bt = NULL;
        cout<<"Wrong encording; got missing bytes"<<endl;
    }
    return bt;
}