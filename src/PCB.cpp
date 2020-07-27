/*
 * PCB.cpp
 *
 *  Created on: Apr 25, 2020
 *      Author: OS1
 */



#include "PCB.h"

#ifndef BCC_BLOCK_IGNORE

extern volatile unsigned int lockFlag;

PCB *PCB_arr[max_number_of_threads];

PCB::PCB(StackSize stackSize, Time timeSlice, Thread *thr,int isIdle)
{
	if(stackSize > 64000)
		stackSize = 64000;
	for(int i = 0;i<signals_N;i++)
	{
		blockedSignals[i] = 0;
	}

	stackSize/=2;
	this->stack = new unsigned[stackSize];

	this->stack[stackSize-5] = 0x200;
	this->stack[stackSize-6] = FP_SEG(PCB::wrapper);
	this->stack[stackSize-7] = FP_OFF(PCB::wrapper);


	this->ss = FP_SEG(stack+stackSize - 16);
	this->sp = FP_OFF(stack+stackSize - 16);
	this->bp = this->sp;

	this->myThread = thr;
	this->done = 0;
	this->kvant= timeSlice;
	this->id = idCounter++;
	this->blocked = 0;
	this->blockedState = RUNNING;
	this->myNodeInSemaphore = NULL;
	this->mySem = NULL;
	this->unblockedInSem = 1;
	this->isIdle = isIdle;
	this->removed = 0;

	PCB_arr[id] = this;

	if(running != NULL)
	{
		this->myParentPCB = running;
	}

	//cout << "id runninga je: " << running->id << " ,a sad treba da bude  " << this->id << ", a preko threada je " << this->myThread->myPCB->id << "\n";

	if(running != NULL)
	{
		for(i = 0;i<signals_N;i++)
		{
			this->blockedSignals[i] = running->blockedSignals[i];
			signalList[i].createCopy(running->signalList[i].getFirstNode());
		}
	}else
	{
		for(i = 0;i<signals_N;i++)
		{
			this->blockedSignals[i] = 0;
		}
	}
}


void PCB::doSignals()
{
	// Prodjemo kroz sve signale i pokrenemo sve njihove funkcije

	//lockFlag = 0;
	doSignalsList.callFunctions();
	//lockFlag = 1;

/*
	while(doSignalsList.getNumberOfElements() != 0)
	{
		unsigned id = doSignalsList.getFirst();
		//cout << "Sad ce da obradi signal: " << id << "\n";
		signalList[id].signal();
	}*/
}


void PCB::setBlockedSignal(SignalId id,int value)
{
	blockedSignals[id] = value;
}

void PCB::setUnblockedInGlobal()
{
	unblockedInSem = 0;
}

void PCB::setSemaphore(KernelSem *sem)
{
	mySem = sem;
}

KernelSem* PCB::getSemaphore()
{
	return mySem;
}


void PCB::setNodeInSemaphore(DoubleList::Node *node)
{
	myNodeInSemaphore = node;
	blockedState = SEMAPHORE;
}


DoubleList::Node* PCB::getNodeInSemaphore()
{
	return myNodeInSemaphore;
}

void PCB::setBlockedTimeLeft(int value)
{
	blockedTimeLeft = value;
}

int PCB::getBlockedTimeLeft()
{
	return blockedTimeLeft;
}

void PCB::decBlockedTimeLeft()
{
	if(blockedTimeLeft >= 0)
		blockedTimeLeft--;
}

void PCB::wrapper()
{
	if(running->isIdle == 1)
		running->runIdle();
	else
	running->myThread->run();
	running->myParentPCB->myThread->signal(1);
	running->myThread->signal(2);
	//running->doSignalsList.print();

	running->doSignals();

	lockFlag = 0;

	//cout << "running je:  ";
	//PCB_arr[1]->blockedList.print();

	  //cout << "Ovo  je lista:\n";



	//running->blockedList.print();

	// U scheaduler ubacimo sve koje su cekale na ovu nit
	while(running->blockedList.isEmpty() == 0)
	{
		PCB *pcb = running->blockedList.getFirst();
		pcb->blocked = 0;
		Scheduler::put((PCB*)pcb);
	}
	running->done = 1;

	//Kada se zavrsi saljemo roditelju


	lockFlag = 1;
	dispatch();

}

int PCB::getUnblockedInSem()
{
	return unblockedInSem;
}

void PCB::runIdle()
{
	//cout << "U idle runu!\n";
while(1);
}

int PCB::getId()
{
	return this->id;
}

int PCB::isBlocked()
{
	return blocked;
}

void PCB::insertToBlockedList(PCB *pcb)
{
	lockFlag = 0;
	//lockFlag = 0;
	blockedList.insertAtEnd(pcb);
	lockFlag = 1;
	//lockFlag = 1;
	//blockedList.insert(pcb);
}

#endif
