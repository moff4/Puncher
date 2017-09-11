#ifndef PUNCHER_H
#define PUNCHER_H

#include <string>

#include "types.h"
#include "bytes.h"
#include "stack.h"

class Puncher
{

	/**
	 * file name to load code
	 */
	std::string old_filename;
	
	/**
	 * file name to export code
	 */
	std::string new_filename;
	
	/**
	 * code to be runned
	 */
	Bytes ** bytes;

	/**
	 * stack
	 */
	Stack* stack;
	

	/**
	 * search Bytes for string 
	 */
	std::string get_string(_u64,_u64);

	/**
	 * convert string to _u64
	 */
	_u64 _str_to_u64(std::string,_u64);

	/**
	 * convert string to _i64
	 */
	_i64 _str_to_i64(std::string,_u64);

	/**
	 * put string in memory
	 * the first byte is the leftest in line # _u64
	 * the last byte '\0' can be alloceted on another line (dependes on string lenght)
	 */
	void _put_string(_u64,std::string);
	
public:

	/**
	 * init
	 * arg - name of file with code
	 */
	Puncher(std::string);

	/**
	 * destruct
	 */
	~Puncher();

	/**
	 * loads code from file
	 * arg - file encording {hex/boo/bin}
	 */
	bool load(std::string);

	/**
	 * run code
	 */
	bool start();

	/**
	 * converts code to new encording
	 * args - new filename ; new encording {hex/boo/bin}
	 */
	bool convert(std::string,std::string);

};

#endif /* PUNCHER_H */