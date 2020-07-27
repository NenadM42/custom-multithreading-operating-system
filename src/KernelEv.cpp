/*
 * KernelEv.cpp
 *
 *  Created on: Apr 30, 2020
 *      Author: OS1
 */

#include "KernelEv.h"

KernelEv *event_arr[256];

KernelEv::KernelEv(IVTNo ivtNo, Event *event)
{
	//asm cli
	myEvent = event;
	event_arr[ivtNo] = this;
	value = 0;
	this->ivtNo = ivtNo;
	myPCB = running;
	//asm sti
}

KernelEv::~KernelEv()
{

}


void KernelEv::signal()
{
	//asm cli
	myEvent->signal();
	//asm sti
}

void KernelEv::wait()
{
	//asm cli
	myEvent->wait();
	//asm sti
}
