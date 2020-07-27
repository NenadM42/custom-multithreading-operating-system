/*
 * Event.cpp
 *
 *  Created on: Apr 30, 2020
 *      Author: OS1
 */

#include "Event.h"
#include "KernelEv.h"

extern volatile unsigned int lockFlag;


Event::Event(IVTNo ivtNo)
{
	lockFlag = 1;
	//asm cli
	myImpl = new KernelEv(ivtNo,this);
	lockFlag = 0;
	//asm sti
}

Event::~Event()
{

}

void Event::signal()
{

	//cout << "Id je: " << myImpl->myPCB->id << ",a on je blocked =  " << myImpl->myPCB->blocked << "\n";
	lockFlag = 0;
	//asm cli
	myImpl->value = 1;
	// Ako je blokirano stavimo u scheduler
	if(myImpl->myPCB->blocked == 1)
	{
		//cout << ":(\n";

		myImpl->myPCB->blocked = 0;
		Scheduler::put((PCB*)myImpl->myPCB);
		lockFlag = 1;
		dispatch();
	}
	lockFlag = 1;
	//asm sti

	//myImpl->signal();
}

void Event::wait()
{
	lockFlag = 0;
	//asm cli
	if(myImpl->myPCB == running)
	{
		if(myImpl->value == 1)
		{
			myImpl->value = 0;
		}else if(myImpl->myPCB->blocked == 0) // Znaci blokiramo
		{
			myImpl->myPCB->blocked = 1;
			//asm sti
			lockFlag = 1;
			dispatch();
		}
	}
	lockFlag = 1;
	//asm sti
	//myImpl->wait();
}
