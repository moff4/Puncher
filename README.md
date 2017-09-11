# Puncher
Punched card emulator

Hello Word, Bro!  
`0000 0000 0000 0110 0000 0000 0000 0000 0000 0100 0000 0000 0000 0000 0000 0000`  
`0100 1000 0110 0101 0110 1100 0110 1100 0110 1111 0010 0000 0101 0111 0110 1111`  
`0111 0010 0110 1100 0110 0100 0010 1100 0010 0000 0100 0010 0111 0010 0110 1111`  
`0010 0001 0000 1010 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000`  
`0000 0000 0000 1001 0000 0000 0000 0000 0000 0001 0000 0000 0000 0000 0000 0010`  
`1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1111`  

## How it works?

64bit in line in hexcode with spaces/tabs and comments after '#' (or binary code)  
Example: `00 02  00 00 02  00 00 03 # ADD (*2) (*3)` 

### Code  
each 32bit is a memory cell that contains:  
* 1 command (2 bytes);  
* 2 args (2 x 3 bytes);  
cell: `[00 02] [00 00 02] [00 00 03]`  

### Data
64bit in line is one memory cell  
Memory cell can be data or command with args and there is differens between them only in runtime.  
So then we get:
* Maximum usigned integer value = 2^64-1
* Maximum address length = 2^24-1 (That's also max size of ur program)
* Maximum number of different commands = 2^16-1


### Commands
| number(2bytes)	|	command												| 1st arg (3bytes)  | 2nd arg (3bytes)															|
|-------------------|-------------------------------------------------------|-------------------|---------------------------------------------------------------------------|
|  -1.				|	[exit](https://github.com/moff4/Puncher#-1-exit)	|	-1				|	-1																		|
|	1.				|	[mov](https://github.com/moff4/Puncher#1-mov)		|	address			|	address																	|
|	2.				|	[add](https://github.com/moff4/Puncher#2-add)		|	address			|	address																	|
|	3.				|	[sub](https://github.com/moff4/Puncher#3-sub)		|	address			|	address																	|
|	4.				|	[mul](https://github.com/moff4/Puncher#4-mul)		|	address			|	address																	|
|	5.				|	[div](https://github.com/moff4/Puncher#5-div)		|	address			|	address																	|
|	6.				|	[jmp](https://github.com/moff4/Puncher#6-jmp)		|	address			|	nothing																	|
|	7.				|	[push](https://github.com/moff4/Puncher#7-push)		|	address			|	nothing																	|
|	8.				|	[pop](https://github.com/moff4/Puncher#8-pop)		|	address			|	nothing																	|
|	9.				|	[write](https://github.com/moff4/Puncher#9-write)	|	address			|	[iotype](https://github.com/moff4/Puncher#types-of-input-and-output-data)	|
|	10.				|	[read](https://github.com/moff4/Puncher#10-read)	|	address			|	[iotype](https://github.com/moff4/Puncher#types-of-input-and-output-data)	|
|	11.				|	[shl](https://github.com/moff4/Puncher#11-shl)		|	address			|	number																	|
|	12.				|	[shr](https://github.com/moff4/Puncher#12-shr)		|	address			|	number																	|
|	13.				|	[shl8](https://github.com/moff4/Puncher#13-shl8)	|	address			|	number																	|
|	14.				|	[shr8](https://github.com/moff4/Puncher#14-shr8)	|	address			|	number																	|
|	15.				|	[je](https://github.com/moff4/Puncher#15-je)		|	address			|	address																	|
|	16.				|	[jne](https://github.com/moff4/Puncher#16-jne)		|	address			|	address																	|
|	17.				|	[jg](https://github.com/moff4/Puncher#17-jg)		|	address			|	address																	|
|	18.				|	[jge](https://github.com/moff4/Puncher#18-jge)		|	address			|	address																	|
|	19.				|	[ja](https://github.com/moff4/Puncher#19-ja)		|	address			|	address																	|
|	20.				|	[jae](https://github.com/moff4/Puncher#20-jae)		|	address			|	address																	|
|	21.				|	[jl](https://github.com/moff4/Puncher#21-jl)		|	address			|	address																	|
|	22.				|	[jle](https://github.com/moff4/Puncher#22-jle)		|	address			|	address																	|
|	23.				|	[jb](https://github.com/moff4/Puncher#23-jb)		|	address			|	address																	|
|	24.				|	[jbe](https://github.com/moff4/Puncher#24-jbe)		|	address			|	address																	|
|	25.				|	[alloc](https://github.com/moff4/Puncher#25-alloc)	|	number			|	nothing																	|
|	26.				|	[loop](https://github.com/moff4/Puncher#26-loop)	|	address			|	address																	|


"1st" and "2nd" means:
* data at memmory cell with address == ith arg if arg == "address"
* number == ith arg if arg == "number"
* number in range(0,5) if arg == "oitype"

#### -1. exit  
Just stops program  
That's only way to stop running ur program

#### 1. mov  
Just move data from one cell to another  
1st = 2nd

#### 2. add  
1st  = (1st + 2nd)

#### 3. sub  
1st = (1st - 2nd)

#### 4. mul  
1st = (1st * 2nd)

#### 5. div  
1st = (1st / 2nd)

#### 6. jmp  
goto another `address`

#### 7. push  
push to stack data from cell
push(1st)

#### 8. pop  
pop data from stack to cell
1st = pop()

#### 9. write  
write stdout data according to [type](https://github.com/moff4/Puncher#types-of-input-and-output-data):

#### 10. read  
read stdin from data according to [type](https://github.com/moff4/Puncher#types-of-input-and-output-data):

#### 11. shl  
1st = 1st * ( 2 * 2nd)

#### 12. shr  
1st = 1st / ( 2 * 2nd)

#### 13. shl8  
1st = 1st * ( 16 * 2nd)

#### 14. shr8  
1st = 1st / ( 16 * 2nd)

#### 15. je  
addr = stack.pop()  
jump to addr if 1st == 2nd  

#### 16. jne  
addr = stack.pop()  
jump to addr if 1st != 2nd  

#### 17. jg  
addr = stack.pop()  
jump to addr if (signed)1st > (signed)2nd  

#### 18. jge  
addr = stack.pop()  
jump to addr if (signed)1st >= (signed)2nd  

#### 19. ja  
addr = stack.pop()  
jump to addr if (unsigned)1st > (unsigned)2nd  

#### 20. jae  
addr = stack.pop()  
jump to addr if (unsigned)1st >= (unsigned)2nd  

#### 21. jl  
addr = stack.pop()
jump to addr if (signed)1st < (signed)2nd  

#### 22. jle  
addr = stack.pop()
jump to addr if (signed)1st <= (signed)2nd  

#### 23. jb  
addr = stack.pop()
jump to addr if (unsigned)1st < (unsigned)2nd  

#### 24. jbe  
addr = stack.pop()
jump to addr if (unsigned)1st < (unsigned)2nd  

#### 25. alloc  
Allocate number lines at the end of code  
Fill it by zero  
U can fill it with code or data  

#### 26. loop  
if value in 1st address != 0  
then value in 1st address -= 1  
and goto 2nd address  
else do nothing

#### Types of input and output data  
0) unsigned int (dec) 
1) signed int (dec) 
2) [string](https://github.com/moff4/Puncher#what-is-string)
3) [char](https://github.com/moff4/Puncher#what-is-char)
4) hex  
5) binary  


### What is string?
String is sequence of bytes ended with zero byte.  
String starts from first (left) byte that is not zero in cell (64bit) and can take as much bytes as it needs and the address of this string will be number of the first line that this string uses.  
Long strings can have first byte on one line and the last on the next one (or even any longer), the value will be between them even if its more then 100500 bytes  
Example:  
This
`1| 00 48  69 2C 20  6D 61 6E # st := "Hi, man"`  
`2| 21 21  21 0A 00  00 00 00 # .st := "!!!\n"`
Same to this  
`1| 48 69  2C 20 6D  61 6E 21 # st := "Hi, man!"`
`2| 21 21  0A 00 00  00 00 00 # .st := "!!\n"`
Also this:  
`1| 00 00  00 00 00  00 00 48 # st := "H"`
`2| 69 2C  20 6D 61  6E 21 21 # .st := "i, man!!"`
`3| 21 0A  00 00 00  00 00 00 # .st := "!\n"`

But not this:  
`1| 00 00  00 00 00  00 00 00 # nothing here`
`2| 48 69  2C 20 6D  61 6E 21 # st := "Hi, man!"`
`3| 21 21  0A 00 00  00 00 00 # .st := "!!\n"`


### What is char?
Char is the lowest (rightest) byte in cell.  
Read or write char means that data will be written or read from only one byte in cell
Example:  
This
`1| 00 00  00 00 00  00 00 0A # C := '\n'`
Same to this  
`1| 00 00  00 00 00  00 FF 0A # C := '\n'`
But not this  
`1| 00 00  00 00 00  00 0A 00 # C := '\0'`
or this
`1| 0A 00  00 00 00  00 00 00 # C := '\0'`

## How to run program?
**Windows**: ¯\\_(ツ)_/¯  
**Linux/Unix**: write magic number to this interpreter: `#!/path/pucher`  

Alternative variant:  
Example:  
`$ make -j && ./build/puncher test/hello.hex`  

Flags:  
`pucher [filename] [ -h | -0 | -b ] [ [ --conv-hex | --conv-boo | --conv-bin ] new_filename ]`  
` filename - name of file with code`  
` -h - code encording is hex (default)`  
` -0 - code encording contains only 0 and 1`  
` -b - code has binary encording`  
` --conv-hex - convert code to hex`  
` --conv-boo - convert code to {0|1}*`  
` --conv-bin - convert code to binary`  
` new_filename - name of file for convertor input`   
` if u put flag to wrong encoding u'll get unknown behaviour`  

## TO DO LIST  
- float
