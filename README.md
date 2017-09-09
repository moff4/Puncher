# Puncher
Punched card emulater

## How it works?

64bit in line in hexcode with spaces/tabs and comments after '#' (or binary code)  
Example: `00 02  00 00 02  00 00 03 # ADD (*2) (*3)` 

### Code
each 32bit is word:
* 1st word - command;
* 2nd and 3rd - args;

### Data
64bit in line is one memory cell  
Memory cell can be data or command with args and there differens between them only in runtime.  
So here we get:
* Maximum usigned integer value = 2^64-1
* Maximum address length = 2^24-1 (That's also max size of ur program)
* Maximum number of different commands = 2^16-1


### Commands
| number(2bytes)	|	command	| 1st arg (3bytes)  | 2nd arg (3bytes)  |
|-------------------|-----------|-------------------|-------------------|
|  -1.				|	exit	|	-1				|	-1				|
|	1.				|	mov		|	from			|	to				|
|	2.				|	add		|	address			|	address			|
|	3.				|	sub		|	address			|	address			|
|	4.				|	mul		|	address			|	address			|
|	5.				|	div		|	address			|	address			|
|	6.				|	jmp		|	address			|	nothing			|
|	7.				|	push	|	address			|	nothing			|
|	8.				|	pop		|	address			|	nothing			|
|	9.				|	echo	|	address			|	type			|
|	10.				|	read	|	address			|	type			|
|	11.				|	shl		|	address			|	number			|
|	12.				|	shr		|	address			|	number			|
|	13.				|	shl8	|	address			|	number			|
|	14.				|	shr8	|	address			|	number			|

* "1st" means "data in 1st addreess"
* "2nd" means "data in 2st addreess"

#### -1. exit  
Just stops program  
That's only way to stop running ur program

#### 1. mov  
Just move data from one cell to another

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

#### 8. pop  
pop data from stack to cell

#### 9. echo  
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

#### Types of input and output data  
1) int(dec)  
2) string  
3) hex  
4) octal  
5) binary  


### What is string?
String is sequence of bytes ended with zero byte.  
String starts from first (left) byte that is not zero in cell (64bit) and can take as much bytes as it needs and the address of this string will be number of the first line that this string uses.  
Long strings can have first byte on one line and the last on the next one (or even any longer), the value will be between them even if its more then 100500 bytes  

## How to run program?
Linux/Unix: write magic number to this interpreter: `#!/path/pucher`  
Windows: ¯\\_(ツ)_/¯




