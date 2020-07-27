/*
 * IVTEntry.cpp
 *
 *  Created on: Apr 30, 2020
 *      Author: OS1
 */

#include "IVTEntry.h"
#include <dos.h>
#include <iostream.h>
#include "Event.h"
#include "KernelEv.h"


extern volatile unsigned int lockFlag;

IVTEntry::IVTEntry(int num, pInterrupt interruptRoutine)
{
	numEntry = num;
	lockFlag = 0;
	//asm cli
	oldRout = getvect(numEntry);
	setvect(numEntry,interruptRoutine);
	lockFlag = 1;
	//asm sti
}

IVTEntry::~IVTEntry()
{
	lockFlag = 0;
	//asm cli
	setvect(numEntry,oldRout);
	lockFlag = 1;
	//asm sti
}


void IVTEntry::signal()
{
	//asm cli
	event_arr[numEntry]->signal();
	//asm sti
}

void IVTEntry::callReal()
{
	lockFlag = 0;
	//asm cli
	oldRout();
	lockFlag = 1;
	//asm sti
}
