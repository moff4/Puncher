#ifndef STACK_H
#define STACK_H

#include "types.h"

/**
 * chain block
 */
struct block
{
	/**
	 * previous block
	 */
	block*pr;

	/**
	 * value of block
	 */
	_u64 x;

	/**
	 * init
	 */
	block(block*,_u64);

	/**
	 * destruct
	 */
	~block();

};

class Stack
{
	block * bl;
public:
	/**
	 * init
	 */
	Stack();

	/**
	 * destruct
	 */
	~Stack();

	/**
	 * push to Stack
	 */
	void push(_u64);

	/**
	 * pop out of Stack
	 */
	_u64 pop();
};
#endif /* STACK_H */