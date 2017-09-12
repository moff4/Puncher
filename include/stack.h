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
	block*old,*_new;

	/**
	 * value of block
	 */
	_u64 x;

	/**
	 * init
	 */
	block(_u64);

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
	_u64 pop(_u64);

	/**
	 * return size of stack
	 */
	_u64 size();
};
#endif /* STACK_H */