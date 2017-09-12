#include <stdlib.h>
#include <iostream>

#include "stack.h"

using namespace std;


#define STACK_EXTRA_OUTPUT

/**
 * init
 */
block::block(_u64 y)
{
	this->old = this->_new = NULL;
	this->x = y;
}

/**
 * destruct
 */
block::~block()
{
	// nothing
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
	if (this->bl!=NULL)
	{
		while (this->bl->old != NULL)
		{
			this->bl = this->bl->old;
		}
	}
}

/**
 * push to Stack
 */
void Stack::push(_u64 x)
{
	if (this->bl == NULL)
	{
		this->bl = new block(x);
	}
	else
	{
		this->bl->_new = new  block(x);
		this->bl->_new->old = this->bl;
		this->bl = this->bl->_new;
	}
}

/**
 * pop out of Stack
 */
_u64 Stack::pop(_u64 line)
{
	//cout<<"from stack: pop: line: "<<line<<endl;
	if (this->bl == NULL)
	{
		cout<<"STACK ERROR: pop when it's empty ; line: "<<line<<endl;
		exit(1);
		return 0;
	}
	while (this->bl->_new != NULL)
	{
		this->bl = this->bl->_new;
	}
	_u64 x = this->bl->x;
	if (this->bl->old != NULL)
	{
		this->bl = this->bl->old;
		delete this->bl->_new;
		this->bl->_new = NULL;
	}
	else
	{
		delete this->bl;
		this->bl = NULL;
	}
	return x;
}

/**
 * return size of stack
 */
_u64 Stack::size()
{
	if (this->bl == NULL)
	{
		return 0;
	}
	_u64 x = 1;
	while (this->bl->old != NULL)
	{
		this->bl = this->bl->old;
	}
	while (this->bl->_new != NULL)
	{
		x ++ ;
		this->bl = this->bl->_new;
	}
	return x;
}


