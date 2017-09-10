#include <stdlib.h>

#include "stack.h"

/**
 * init
 */
block::block(block*bl,_u64 y)
{
	this->pr = bl;
	this->x = y;
}

/**
 * destruct
 */
block::~block()
{
	if (this->pr != NULL)
	{
		delete this->pr;
	}
}

/**
 * init
 */
Stack::Stack()
{
	this->bl = NULL;
}

/**
 * destruct
 */
Stack::~Stack()
{
	delete this->bl;
}

/**
 * push to Stack
 */
void Stack::push(_u64 x)
{
	block * cx = new block(this->bl,x);
	this->bl = cx;
}

/**
 * pop out of Stack
 */
_u64 Stack::pop()
{
	block*cx = this->bl->pr;
	this->bl->pr = NULL;
	_u64 y = this->bl->x;
	delete this->bl;
	this->bl = cx;
	return y;
}