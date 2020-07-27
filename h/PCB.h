/*
 * PCB.h
 *
 *  Created on: Apr 2, 2020
 *      Author: OS1
 */

#ifndef PCB_H_
#define PCB_H_


#include "thread.h"
#include "PCBLists.h"
#include "KernSem.h"
#include "IVTEntry.h"
#include "DList.h"


#ifndef BCC_BLOCK_IGNORE

static int idCounter = 0;
const int max_number_of_threads = 1000;

extern PCB *PCB_arr[max_number_of_threads];
extern PCB *running;



class PCB
{

public:
	enum State{RUNNING,WAITING,SEMAPHORE};
private:
	int blockedTimeLeft;
	int unblockedInSem;
	State blockedState;
	DoubleList::Node *myNodeInSemaphore;
	KernelSem *mySem;

public:
	unsigned sp;
	unsigned ss;
	unsigned bp;
	unsigned *stack;
	int isIdle;
	int kvant;
	int done;
	int id;
	int blocked;
	int removed;
	Thread *myThread;
	DoubleListSignalHandler signalList[signals_N];
	DoubleListSignalId doSignalsList;
	int blockedSignals[signals_N];
	PCB *myParentPCB;


	DoubleList blockedList;


	PCB(StackSize stackSize, Time timeSlice, Thread *thr, int isIdle = 0);
	PCB()
	{
		id = idCounter++;
	}

	~PCB()
	{
		// Obrisace se onda u prekidnoj rutini!
		//this->removed = 1;
		delete[] stack;
	}


	// Znaci u globalnoj listi je odblokirana
	void setUnblockedInGlobal();

	void setBlockedSignal(SignalId id,int value);


	int getUnblockedInSem();

	void setSemaphore(KernelSem *sem);

	KernelSem *getSemaphore();

	void setNodeInSemaphore(DoubleList::Node *node);

	DoubleList::Node *getNodeInSemaphore();

	void setBlockedTimeLeft(int value);

	int getBlockedTimeLeft();

	void decBlockedTimeLeft();

	int getId();

	int isBlocked();

	void insertToBlockedList(PCB *pcb);

	static void wrapper();

	void runIdle();

	void doSignals();

};



#endif /* PCB_H_ */
#endif
