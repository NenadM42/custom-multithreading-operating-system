/*
 * KernelEv.h
 *
 *  Created on: Apr 30, 2020
 *      Author: OS1
 */

#ifndef KERNELEV_H_
#define KERNELEV_H_

#include "Event.h"
#include "PCB.h"

class KernelEv {

protected:
	int value;
	IVTNo ivtNo;
	PCB *myPCB;
	Event *myEvent;

	friend class Event;
public:
	KernelEv(IVTNo ivtNo, Event *event);
	virtual ~KernelEv();


	void signal();

	void wait();

};


extern KernelEv *event_arr[256];

#endif /* KERNELEV_H_ */
