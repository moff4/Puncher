#include <string>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>

#include "puncher.h"
#include "lord.h"

using namespace std;

#define _MAX 0xFFFFFFFFFFFFFFFF
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

/**
 * search Bytes for string 
 */
inline string Puncher::get_string(_u64 line,_u64 line_num)
{
	_i64 i=0;
	string st = "";
	for (;_byte(this->bytes[line+(i>>3)]->val,(7-(i&0x7)))!=0x00;i++)
	{
		st += _byte(this->bytes[line+(i>>3)]->val,(7-(i&0x7)));
	}
	return st;
}

/**
 * convert string to _u64
 */
inline _u64 Puncher::_str_to_u64(string st,_u64 line)
{
	_u64 x = 0;
	for (_u64 i=0;i<st.length();i++)
	{
		if ((st[i]<'0')or(st[i]>'9'))
		{
			cout<<"STR_TO_U64 ERROR: got unexpected char: "<<(int)st[i]<<"; line: "<<line<<endl;
			exit(1);
		}
		x *= 10;
		x += st[i] - '0';
	}
	return x;
}

/**
 * convert string to _i64
 */
inline _i64 Puncher::_str_to_i64(string st,_u64 line)
{
	_i64 x,i=st[0]=='-';
	for (;i<st.length();i++)
	{
		if ((st[i]<'0')or(st[i]>'9'))
		{
			cout<<"STR_TO_U64 ERROR: got unexpected char: "<<(int)st[i]<<"; line: "<<line<<endl;
			exit(1);
		}
		x *= 10;
		x += st[i] - '0';
	}
	x *= -1 * (st[0]=='-');
	return x;
}

/**
 * put string in memory
 * the first byte is the leftest in line # _u64
 * the last byte '\0' can be alloceted on another line (dependes on string length)
 */
inline void Puncher::_put_string(_u64 line,string st)
{
	/**
	 * It's fuckingly works!
	 */
	_u64 i=0;
	for (;i<st.length();i++)
	{
		if ((i & 0x7) == 0)
		{
			this->bytes[line+(i>>3)]->val = _MAX;
		}
		_u64 z = (~((_u64)0xFF << 8*(7 - (i & 0x7)))) | ((_u64)st[i] << 8*(7 - (i & 0x7)));
		this->bytes[line+(i>>3)]->val &= z; 
	}
	this->bytes[line+(i>>3)]->val &= (~((_u64)0xFF << 8*(7 - (i & 0x7))));
}



//#define CMD_EXTRA_OUTPUT
//#define STACK_EXTRA_OUTPUT 
#define COMMAND(x)						((x & 0xFFFF000000000000) >> 48)
#define _1arg(x)						((x & 0x0000FFFFFF000000) >> 24)
#define _2arg(x)						((x & 0x0000000000FFFFFF))
#define check_addrs(x,y,line,line_num)	{if (x>=line_num) {cout<<"addr1 err; line: "<<line<<endl; return true;}if (y>=line_num) {cout<<"addr2 err; line: "<<line<<endl; return true;}}
#define check_addr(x,line,line_num)		{if (x>=line_num) {cout<<"addr err; line: "<<line<<endl; return true;}}
#define check_iotype(x,line,line_num)	{if (x>6) {cout<<"type err; line: "<<line<<endl; return true;}}
/**
 * run code
 */
bool Puncher::start()
{

	this->stack = new Stack; 
	_u64 line_num;
	for(line_num = 0;this->bytes[line_num]!=NULL;line_num++);

	_u64 x, _1, _2;
	for(_u64 line = 0;this->bytes[line]!=NULL;line++)
	{
		x = this->bytes[line]->val;
		
		#ifdef CMD_EXTRA_OUTPUT
		cout<<"line:  "<<line<<"\tcmd: "<<COMMAND(x)<<"\t1arg: "<<_1arg(x)<<"\t2arg: "<<_2arg(x)<<endl;
		#endif /* CMD_EXTRA_OUTPUT */

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
				if (this->bytes[_2]->val == 0)
				{
					cout<<"DIVISION ERROR: division by zero; line: "<<line<<endl;
					return true;
				}
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
				#ifdef STACK_EXTRA_OUTPUT
				cout<<"--- STACK-PUSH: line: "<<line<<"; val: "<<this->bytes[_1]->val<<"; size: "<<this->stack->size()<<endl;
				#endif
				this->stack -> push(this->bytes[_1]->val);
			}break;

			/**
			 * pop
			 */
			case 8:
			{
				check_addr(_1,line,line_num)
				_u64 y = this->stack -> pop(line);
				#ifdef STACK_EXTRA_OUTPUT
				cout<<"--- STACK-POP: line: "<<line<<"; val: "<<y<<"; size: "<<this->stack->size()<<endl;
				#endif
				this->bytes[_1]->val = y;
			}break;

			/**
			 * write
			 */
			case 9:
			{
				#ifdef EXTRA_OUTPUT_1
				cout<<"STDOUT: ";
				#endif /* EXTRA_OUTPUT_1 */
				if (_2 != 6)
				{
					check_addr(_1,line,line_num)
				}
				check_iotype(_2,line,line_num)
				switch (_2)
				{
					/**
					 * usigned int (dec)
					 */
					case 0:
					{
						cout<<this->bytes[_1]->val;
					}break;

					/**
					 * signed int (dec)
					 */
					case 1:
					{
						cout<<(_i64)this->bytes[_1]->val;
					}break;
					
					/**
					 * string
					 */
					case 2:
					{
						string st = get_string(_1,line_num);
						cout << st;
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
						x = this->bytes[_1]->val;
						string st = "";
						for (int j=0;j<64;j++)
						{
							st = (char)(x%2+'0') + st;
							x=x>>1;
						}
						cout<<st;
					}break;

					/**
					 * fast char
					 */
					case 6:
					{
						_u8 q = (_1&0xFF);
						printf("%c",q);
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
				#ifdef EXTRA_OUTPUT_1
				cout<<"STDIN: ";
				#endif /* EXTRA_OUTPUT_1 */
				check_addr(_1,line,line_num)
				check_iotype(_2,line,line_num)
				switch (_2)
				{
					/**
					 * unsigned int (dec)
					 */
					case 0:
					{
						_u64 y;
						cin >> y;
						this->bytes[_1]->val = (_u64)y;
					}break;

					/**
					 * signed int (dec)
					 */
					case 1:
					{
						_i64 y;
						cin >> y;
						this->bytes[_1]->val = (_u64)y;
					}break;
					
					/**
					 * string
					 */
					case 2:
					{
						_u64 q;
						string st = "";
						while (((q=getchar())!=(_u64)EOF)and(q!='\r')and(q!='\n'))
						{
							st += q;
						}
						_put_string(_1,st);
					}break;
					
					/**
					 * char
					 */
					case 3:
					{
						_u8 q = getchar();
						this->bytes[_1]->val = (_u64)q;
					}break;
					
					/**
					 * hex
					 */
					case 4:
					{
						int j = 0;
						_u64 y = 0;
						_u8 q;
						while ((((q=getchar())>='0')and(q<='9')) or ((q>='A')and(q<='F')) or ((q>='a')and(q<='f')))
						{	
							/**
							 * if there'll be nore then 16 letters we will automaticaly get mod 2**64 because of overflow
							 */
							y = y << 4;
							if ((q>='A')and(q<='F'))
							{
								y+=q-'A' + 10;
							}
							else if ((q>='a')and(q<='f'))
							{
								y+=q-'a' + 10;
							}
							else if ((q>='0')and(q<='9'))
							{
								y+=q-'0';
							}
						}
						this->bytes[_1]->val = y;
					}break;
					
					/**
					 * bin
					 */
					case 5:
					{
						int j = 0;
						_u64 y = 0;
						_u8 q;
						while (((q=getchar())>='0')and(q<='1'))
						{	
							/**
							 * if there'll be nore then 64 letters we will automaticaly get mod 2**64 because of overflow
							 */
							y = y << 1;
							y+=q-'0';
						}
						this->bytes[_1]->val = y;
					}break;
					
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
				this->bytes[_1]->val = this->bytes[_1]->val << _2;
			}break;

			/**
			 * shr
			 */
			case 12:
			{
				check_addr(_1,line,line_num)
				this->bytes[_1]->val = this->bytes[_1]->val >> _2;
			}break;

			/**
			 * shl8
			 */
			case 13:
			{
				check_addr(_1,line,line_num)
				this->bytes[_1]->val = this->bytes[_1]->val << (_2 * 8);
			}break;

			/**
			 * shr8
			 */
			case 14:
			{
				check_addr(_1,line,line_num)
				this->bytes[_1]->val = this->bytes[_1]->val >> (_2 * 8);
			}break;

			/**
			 * je
			 */
			case 15:
			{
				_u64 addr = this->stack->pop(line);
				check_addr(addr,line,line_num)
				if (this->bytes[_1]->val == this->bytes[_2]->val)
				{
					line = addr - 1;
				}
			}break;

			/**
			 * jne
			 */
			case 16:
			{
				_u64 addr = this->stack->pop(line);
				check_addr(addr,line,line_num)
				if (this->bytes[_1]->val != this->bytes[_2]->val)
				{
					line = addr - 1;
				}
			}break;

			/**
			 * jg
			 */
			case 17:
			{
				_u64 addr = this->stack->pop(line);
				check_addr(addr,line,line_num)
				if ((_i64)this->bytes[_1]->val > (_i64)this->bytes[_2]->val)
				{
					line = addr - 1;
				}
			}break;

			/**
			 * jge
			 */
			case 18:
			{
				_u64 addr = this->stack->pop(line);
				check_addr(addr,line,line_num)
				if ((_i64)this->bytes[_1]->val >= (_i64)this->bytes[_2]->val)
				{
					line = addr - 1;
				}
			}break;

			/**
			 * ja
			 */
			case 19:
			{
				_u64 addr = this->stack->pop(line);
				check_addr(addr,line,line_num)
				if ((_u64)this->bytes[_1]->val > (_u64)this->bytes[_2]->val)
				{
					line = addr - 1;
				}
			}break;

			/**
			 * jae
			 */
			case 20:
			{
				_u64 addr = this->stack->pop(line);
				check_addr(addr,line,line_num)
				if ((_u64)this->bytes[_1]->val >= (_u64)this->bytes[_2]->val)
				{
					line = addr - 1;
				}
			}break;

			/**
			 * jl
			 */
			case 21:
			{
				_u64 addr = this->stack->pop(line);
				check_addr(addr,line,line_num)
				if ((_i64)this->bytes[_1]->val < (_i64)this->bytes[_2]->val)
				{
					line = addr - 1;
				}
			}break;

			/**
			 * jle
			 */
			case 22:
			{
				_u64 addr = this->stack->pop(line);
				check_addr(addr,line,line_num)
				if ((_i64)this->bytes[_1]->val <= (_i64)this->bytes[_2]->val)
				{
					line = addr - 1;
				}
			}break;
			
			/**
			 * jb
			 */
			case 23:
			{
				_u64 addr = this->stack->pop(line);
				check_addr(addr,line,line_num)
				if ((_u64)this->bytes[_1]->val < (_u64)this->bytes[_2]->val)
				{
					line = addr - 1;
				}
			}break;

			/**
			 * jbe
			 */
			case 24:
			{
				_u64 addr = this->stack->pop(line);
				check_addr(addr,line,line_num)
				if ((_u64)this->bytes[_1]->val < (_u64)this->bytes[_2]->val)
				{
					line = addr - 1;
				}
			}break;

			/**
			 * alloc
			 */
			case 25:
			{
				int i=0;
				_u64 _new_size = line_num + _1 + 2;
				this->bytes = (Bytes**)realloc(this->bytes,_new_size*sizeof(Bytes*));
				for (; i < _1 ; i++)
				{
					this->bytes[line_num + i] = new Bytes();
				}
				this->bytes[line_num + i] = NULL;
				//line_num += i;
				for(line_num = 0;this->bytes[line_num]!=NULL;line_num++);
			}break;

			/**
			 * loop
			 */
			case 26:
			{
				check_addrs(_1,_2,line,line_num)
				if (this->bytes[_1]->val != 0)
				{
					this->bytes[_1]->val -=1;
					line = _2 - 1;
				}
			}break;

			/**
			 * open
			 * 1  | O_RDONLY        open for reading only  
			 * 2  | O_WRONLY        open for writing only  
			 * 4  | O_APPEND        append on each write  
			 * 8  | O_CREAT         create file if it does not exist  
			 * 16 | O_TRUNC         truncate size to 0  
			 * 32 | O_NOFOLLOW      do not follow symlinks  
			 * 64 | O_SYMLINK       allow open of symlinks  
			 */
			case 27:
			{
				check_addr(_1,line,line_num)
				string st = get_string(_1,line_num);
				if (st=="")
				{
					cout<<"OPEN ERROR: filename empty; line"<<line<<endl;
					return true;
				}
				_u64 rights = (0xFF8000 & _2) >> 15 ;
				_u64 mod = 0;
				if (_2 & (1 )) mod += O_RDONLY;
				if (_2 & (2 )) mod += O_WRONLY;
				if (_2 & (4 )) mod += O_APPEND;
				if (_2 & (8 )) mod += O_CREAT;
				if (_2 & (16)) mod += O_TRUNC;
				if (_2 & (32)) mod += O_NOFOLLOW;
				this->stack->push((_u64)open(st.c_str(),mod,rights));
			}break;

			/**
			 * close
			 */
			case 28:
			{
				check_addr(_1,line,line_num)
				close(this->bytes[_1]->val);
			}break;

			/**
			 * fread
			 */
			case 29:
			{
				#ifdef EXTRA_OUTPUT_1
				cout<<"FILE INPUT: ";
				#endif /* EXTRA_OUTPUT_1 */
				check_addr(_1,line,line_num)
				check_iotype(_2,line,line_num)
				_u64 fd = this->stack->pop(line);

				string st = "";
				char q;
				if (_2 == 3)
				{
					read(fd,&q,1);
					st = q;
				}
				else
				{
					while (true)
					{
						read(fd,&q,1);
						if ((_2 == 2)and(q!='\n')and(q!='\r'))
						{
							st += q;
						}
						else if ((_2 != 2)and(q!=' ')and(q!='\n')and(q!='\r'))
						{
							st += q;
						}
						else
						{
							break;
						}
					}
				} 

				switch (_2)
				{
					/**
					 * unsigned int (dec)
					 */
					case 0:
					{
						this->bytes[_1]->val = this->_str_to_u64(st,line);
					}break;

					/**
					 * signed int (dec)
					 */
					case 1:
					{
						this->bytes[_1]->val = this->_str_to_i64(st,line);
					}break;
					
					/**
					 * string
					 */
					case 2:
					{
						this->_put_string(_1,st);
					}break;
					
					/**
					 * char
					 */
					case 3:
					{
						_u8 q = st[0];
						this->bytes[_1]->val = (_u64)q;
					}break;
					
					/**
					 * hex
					 */
					case 4:
					{
						int j = 0;
						_u64 y = 0;
						_u8 q;
						for (int i=0;i<st.length();i++)
						{	
							/**
							 * if there'll be nore then 16 letters we will automaticaly get mod 2**64 because of overflow
							 */
							q = st[i];
							y = y << 4;
							if ((q>='A')and(q<='F'))
							{
								y+=q-'A' + 10;
							}
							else if ((q>='a')and(q<='f'))
							{
								y+=q-'a' + 10;
							}
							else if ((q>='0')and(q<='9'))
							{
								y+=q-'0';
							}
							else
							{
								cout<<"FREAD ERROR: unexpected char: "<<(int)q<<"; line: "<<line<<endl;
								return true;
							}
						}
						this->bytes[_1]->val = y;
					}break;
					
					/**
					 * bin
					 */
					case 5:
					{
						int j = 0;
						_u64 y = 0;
						_u8 q;
						for (int i=0;i<st.length();i++)
						{	
							/**
							 * if there'll be nore then 64 letters we will automaticaly get mod 2**64 because of overflow
							 */
							y = y << 1;
							if ((q=='0')or(q=='1'))
							{
								y+=q-'0';
							}
							else
							{
								cout<<"FREAD ERROR: unexpected char: "<<(int)q<<"; line: "<<line<<endl;
								return true;
							}
						}
						this->bytes[_1]->val = y;
					}break;
					
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
			 * fwrite
			 */
			case 30:
			{
				#ifdef EXTRA_OUTPUT_1
				cout<<"FILE OUTPUT: ";
				#endif /* EXTRA_OUTPUT_1 */
				if (_2 != 6)
				{
					check_addr(_1,line,line_num)
				}
				check_iotype(_2,line,line_num)
				_u64 fd = this->stack->pop(line);
				switch (_2)
				{
					/**
					 * usigned int (dec)
					 */
					case 0:
					{
						#ifdef __linux__
						const char * mod = "%lu";
						#else
						const char * mod = "%llu";
						#endif
						char st[256];
						sprintf(st,mod,this->bytes[_1]->val);
						write(fd,st,strlen(st));
						write(1,st,strlen(st));
					}break;

					/**
					 * signed int (dec)
					 */
					case 1:
					{
						#ifdef __linux__
						const char * mod = "%ld";
						#else
						const char * mod = "%lld";
						#endif
						char st[256];
						sprintf(st,mod,this->bytes[_1]->val);
						write(fd,st,strlen(st));
						write(1,st,strlen(st));
					}break;
					
					/**
					 * string
					 */
					case 2:
					{
						string st = get_string(_1,line_num);
						write(fd,st.c_str(),st.length());
						cout<<"FOUT: "<<st<<endl;
					}break;
					
					/**
					 * char
					 */
					case 3:
					{
						char y = (char)(0xFF & this->bytes[_1]->val);
						write(fd,&y,1);
					}break;
					
					/**
					 * hex
					 */
					case 4:
					{
						char st[512];
						const char* mod;
						#ifdef __linux__
						mod = "%lx";
						#else
						mod = "%llx";
						#endif
						sprintf(st,mod,this->bytes[_1]->val);
						write(fd,st,strlen(st));
					}break;
					
					/**
					 * bin
					 */
					case 5:
					{
						x = this->bytes[_1]->val;
						string st = "";
						for (int j=0;j<64;j++)
						{
							st = (char)(x%2+'0') + st;
							x=x>>1;
						}
						write(fd,st.c_str(),st.length());
					}break;

					/**
					 * fast char
					 */
					case 6:
					{
						_u8 y = (char)(_2&0xFF);
						write(fd,&(y),1);
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
			 * call
			 */
			case 31:
			{
				check_addr(_1,line,line_num);
				this->stack->push(line);
				line = _1 - 1;
			}break;

			/**
			 * return
			 */
			case 32:
			{
				check_addr(_1,line,line_num);
				if (_2 != 0)
				{
					check_addr(_2,line,line_num);
					this->stack->push(this->bytes[_2]->val);
				}
				line = this->bytes[_1]->val;
			}break;

			/**
			 * exit
			 */
			case 65535:{return false;}break;
			default:
			{
				cout<<"UNKNOWN COMMAND: "<<COMMAND(x)<<" line: "<<line<<endl;
				return true;
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