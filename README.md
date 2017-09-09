# Puncher
Punched card emulater

## How it works?

3 x 32bit in line in hexcode with spaces/tabs and comments after '#'  
Example: `00 00 00 02  00 00 00 02  00 00 00 03 # ADD (*2) (*3)` 

### Code
each 32bit is word:
* 1st word - command;
* 2nd and 3rd - args;

### Data
96bit in line is one memory cell

### Commands
| number	|	command	|1st arg 	| 2nd arg 	|
|-----------|-----------|-----------|-----------|
|	-1		|	exit	|	-1		|	-1  	|
|	1.		|	mov		|	from	|	to  	|
|	2.		|	add 	|	address	|	address	|
|	3.		|	sub 	|	address	|	address	|
|	4.		|	mul 	|	address	|	address	|
|	5.		|	div 	|	address	|	address	|
|	6.		|	jmp 	|	address	|	nothing	|
|	7.		|	push	|	address	|	nothing	|
|	8.		|	pop 	|	address	|	nothing	|
|	9.		|	echo	|	address	|	type	|
|	10.		|	read	|	address	|	type	|

* "1st" means "data in 1st addreess"
* "2nd" means "data in 2st addreess"

**mov** (MOVE)  
Just move data from one cell to another

**add** (ADD)  
1st  = (1st + 2nd)

**sub** (SUBVIDE)  
1st = (1st - 2nd)

**mul** (MULTIPLY)  
1st = (1st * 2nd)

**div** (DIVIDE)  
1st = (1st / 2nd)

**jmp** (JUMP)  
goto another address

**push** (PUSH)  
push to stack data from cell

**pop** (POP)  
pop data from stack to cell

**echo** (ECHO)  
write stdout data according to type:
* type == 1 -> int 
* type == 2 -> string

**read** (WRITE)  
read stdin from data according to type:
* type == 1 -> int 
* type == 2 -> string

### What is string?
String is sequence of bytes ended with zero byte.  
String starts from first (left) bit in cell and can take as much bytes as it needs and the address of this string will be number of the first line that this string uses

## How to run program?
Linux/Unix: write magic number to this interpreter: `#!/path/pucher`  
Windows: ¯\\_(ツ)_/¯




