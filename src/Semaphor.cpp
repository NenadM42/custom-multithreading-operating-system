/*
 * Semaphor.cpp
 *
 *  Created on: Apr 29, 2020
 *      Author: OS1
 */

#include "Semaphor.h"
#include "KernSem.h"

#ifndef BCC_BLOCK_IGNORE

extern volatile unsigned int lockFlag;

void dispatch();

Semaphore::Semaphore(int init)
{
	lockFlag = 0;
	myImpl = new KernelSem(init);
	lockFlag = 1;
}

Semaphore::~Semaphore()
{
	delete myImpl;
}

int Semaphore::wait (Time maxTimeToWait)
{
	//asm cli
	//lockFlag = 0;
	int val = myImpl->wait(maxTimeToWait);
	//asm sti
	//lockFlag = 1;
	//cout << myImpl->val() << "\n";
	//if(myImpl->val() < 0)
		//dispatch();

	return val;
}

int Semaphore::signal(int n)
{
	lockFlag = 0;
	if(n == 0)
		n = 1;
	int val = myImpl->signal(n);
	lockFlag = 1;
	return val;
}

int Semaphore::val () const // Returns the current value of the semaphore
{
	return myImpl->val();
}

#endif
