#include <string>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>

#include "puncher.h"
#include "lord.h"

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
	this->stack = NULL;
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
	if (this->stack!=NULL)
	{
		delete stack;
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

//#define EXTRA_OUTPUT_2
//#define EXTRA_OUTPUT_1
#define COMMAND(x)						((x & 0xFFFF000000000000) >> 48)
#define _1arg(x)						((x & 0x0000FFFFFF000000) >> 24)
#define _2arg(x)						((x & 0x0000000000FFFFFF))
#define check_addrs(x,y,line,line_num)	{if (x>=line_num) {cout<<"addr1 err; line: "<<line<<endl; return true;}if (y>=line_num) {cout<<"addr2 err; line: "<<line<<endl; return true;}}
#define check_addr(x,line,line_num)		{if (x>=line_num) {cout<<"addr err; line: "<<line<<endl; return true;}}
#define check_type(x,line,line_num)		{if (x>5) {cout<<"type err; line: "<<line<<endl; return true;}}
/**
 * run code
 */
bool Puncher::start()
{

	stack = new Stack; 
	_u64 line_num;
	for(line_num = 0;this->bytes[line_num]!=NULL;line_num++);

	_u64 x, _1, _2;
	for(_u64 line = 0;this->bytes[line]!=NULL;line++)
	{
		x = this->bytes[line]->val;

		#ifdef EXTRA_OUTPUT_2
		for (int i=0;this->bytes[i]!=NULL;i++)
		{
			x = this->bytes[i]->val;
			cout<<"\tcmd: "<<COMMAND(x)<<" 1arg: "<<_1arg(x)<<" 2arg: "<<_2arg(x)<<endl;
		}
		cout<<endl;
		#endif /* EXTRA_OUTPUT_2 */
		
		#ifdef EXTRA_OUTPUT_1
		cout<<"cmd: "<<COMMAND(x)<<" 1arg: "<<_1arg(x)<<" 2arg: "<<_2arg(x)<<endl;
		#endif /* EXTRA_OUTPUT_1 */

		_1 = _1arg(x);
		_2 = _2arg(x);
		switch (COMMAND(x))
		{
			/**
			 * mov
			 */
			case 1:
			{
				check_addrs(_1,_2,line,line_num)
				this->bytes[_1]->val = this->bytes[_2]->val;

			}break;
				
			/**
			 * add
			 */
			case 2:
			{
				check_addrs(_1,_2,line,line_num)
				this->bytes[_1]->val = this->bytes[_1]->val + this->bytes[_2]->val;
			}break;
			
			/**
			 * sub
			 */
			case 3:
			{
				check_addrs(_1,_2,line,line_num)
				this->bytes[_1]->val = this->bytes[_1]->val - this->bytes[_2]->val;
			}break;
		
			/**
			 * mul
			 */
			case 4:
			{
				check_addrs(_1,_2,line,line_num)
				this->bytes[_1]->val = this->bytes[_1]->val * this->bytes[_2]->val;
			}break;

			/**
			 * div
			 */
			case 5:
			{
				check_addrs(_1,_2,line,line_num)
				this->bytes[_1]->val = this->bytes[_1]->val / this->bytes[_2]->val;
			}break;

			/**
			 * jmp
			 */
			case 6:
			{
				check_addr(_1,line,line_num)
				line = _1 - 1;
			}break;

			/**
			 * push
			 */
			case 7:
			{
				check_addr(_1,line,line_num)
				stack -> push(this->bytes[_1]->val);
			}break;

			/**
			 * pop
			 */
			case 8:
			{
				check_addr(_1,line,line_num)
				this->bytes[_1]->val = stack -> pop();
			}break;

			/**
			 * write
			 */
			case 9:
			{
				check_addr(_1,line,line_num)
				check_type(_2,line,line_num)
				switch (_2)
				{
					/**
					 * usigned int (dec)
					 */
					case 0:
					{
						const char* mod;
						#ifdef __linux__
						mod = "%ld";
						#else
						mod = "%lld";
						#endif
						printf(mod,this->bytes[_1]->val);
					}break;

					/**
					 * signed int (dec)
					 */
					case 1:
					{
						const char* mod;
						#ifdef __linux__
						mod = "%ld";
						#else
						mod = "%lld";
						#endif
						_i64 i,j;
						i = 0x800000 & this->bytes[_1]->val;
						j = 0x0FFFFF & this->bytes[_1]->val;
						if (i==0)
						{
							printf(mod,(_u64)this->bytes[_1]->val);
						}
						else
						{
							printf(mod,(_i64)this->bytes[_1]->val);
						}
					}break;
					
					/**
					 * string
					 */
					case 2:
					{

					}break;
					
					/**
					 * char
					 */
					case 3:
					{
						char y = (char)(0xFF & this->bytes[_1]->val);
						putchar(y);
					}break;
					
					/**
					 * hex
					 */
					case 4:
					{
						const char* mod;
						#ifdef __linux__
						mod = "%lx";
						#else
						mod = "%llx";
						#endif
						printf(mod,this->bytes[_1]->val);
					}break;
					
					/**
					 * bin
					 */
					case 5:
					{
						string st = "";
						for (int j=0;j<64;j++)
						{
							st = (char)(x%2+'0') + st;
							x=x>>1;
						}
						cout<<st;
					}break;
					
					/**
					 * unknown
					 */
					default:
					{
						cout<<"UNKNOWN TYPE FOR STDOUT: "<<_2<<endl;
						return true;
					}break;
				}
			}break;

			/**
			 * read
			 */
			case 10:
			{
				check_addr(_1,line,line_num)
				check_type(_2,line,line_num)
				switch (_2)
				{
					/**
					 * unsigned int (dec)
					 */
					case 0:{}break;

					/**
					 * signed int (dec)
					 */
					case 1:{}break;
					
					/**
					 * string
					 */
					case 2:{}break;
					
					/**
					 * char
					 */
					case 3:{}break;
					
					/**
					 * hex
					 */
					case 4:{}break;
					
					/**
					 * bin
					 */
					case 5:{}break;
					
					/**
					 * unknown
					 */
					default:
					{
						cout<<"UNKNOWN TYPE FOR STDIN: "<<_2<<endl;
						return true;
					}break;
				}
			}break;

			/**
			 * shl
			 */
			case 11:
			{
				check_addr(_1,line,line_num)
				this->bytes[_1]->val = this->bytes[_1]->val << this->bytes[_2]->val;
			}break;

			/**
			 * shr
			 */
			case 12:
			{
				check_addr(_1,line,line_num)
				this->bytes[_1]->val = this->bytes[_1]->val >> this->bytes[_2]->val;
			}break;

			/**
			 * shl8
			 */
			case 13:
			{
				check_addr(_1,line,line_num)
				this->bytes[_1]->val = this->bytes[_1]->val << (this->bytes[_2]->val * 8);
			}break;

			/**
			 * shr8
			 */
			case 14:
			{
				check_addr(_1,line,line_num)
				this->bytes[_1]->val = this->bytes[_1]->val >> (this->bytes[_2]->val * 8);
			}break;
				
			/**
			 * exit
			 */
			case 65535:{return false;}break;
			default:{
				cout<<"UNKNOWN COMMAND: "<<COMMAND(x)<<" line: "<<line<<endl;
			}break;
		}
	}
	return true;
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