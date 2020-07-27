/*
 * KernSem.cpp
 *
 *  Created on: Apr 29, 2020
 *      Author: OS1
 */

#include "KernSem.h"
#include "PCB.h"

//extern volatile unsigned int lockFlag;


#ifndef BCC_BLOCK_IGNORE

extern volatile unsigned int lockFlag;


KernelSem *kernelSem_arr[max_number_of_sem];
int semCounter;

KernelSem::KernelSem(int initValue)
{
	value = initValue;
	this->id = semCounter++;

	kernelSem_arr[id] = this;
}

KernelSem::~KernelSem()
{

}

void KernelSem::incValue()
{
	value++;
}


int KernelSem::signal(int n)
{
	//cout << "signal\n";

	//asm cli
	value += n;
	int num_block;
	if(n > blockedList.getNumberOfElements())
	{
		num_block = blockedList.getNumberOfElements();
	}else
	{
		num_block = n;
	}



	for(int i = 0;i<num_block;i++)
	{

		PCB *pcb = blockedList.getFirst();

		// Brisemo je iz globalne liste
		DoubleList::Node *nodeToDelete = pcb->getNodeInSemaphore();
		globalSemaphoreList.removeNode(nodeToDelete);

		pcb->blocked = 0;
		Scheduler::put((PCB*)pcb);

		//cout << "odblokirali smo: " << pcb->id << "\n";
	}
	//asm sti

	return num_block;
}

int KernelSem::val()
{
	return value;
}

int KernelSem::wait(Time maxTimeToWait)
{
	//cout << "wait\n";
	//asm cli
	//cout << lockFlag << "OVO JE LOCK FLAG!\n";
	lockFlag = 0;
	value--;
	if(value < 0)
	{
		//cout << "BLOKIRANI RUNNING JE: " << running->id << "\n";
		//asm cli
		for(int i = 0;i<100;i++);
		running->blocked = 1;
		running->setBlockedTimeLeft(maxTimeToWait);

		// insertAtEnd nam takodje daje taj cvor koji smo umetnuli da bi smo imali vezu izmedju njega i cvora u globalnoj listi semafora

		DoubleList::Node *nodeInSem = blockedList.insertAtEnd(running);
		// Ubacimo running u globalnu listu semafora i ubacimo i pokazivac na cvor koji je u listi semafora da bismo moglida izbaicmo oba odjednom
		DoubleList::Node *globalNode = globalSemaphoreList.insertAtEnd(running,nodeInSem);
		// Postavimo sad pokazivac na cvor u globalnoj da bude u blockedListi ovog semafora
		blockedList.setOtherNodeAtEnd(globalNode);


		running->setSemaphore(this);
		running->setNodeInSemaphore(globalNode);
		//asm sti
		lockFlag = 1;
		dispatch();
		return running->getUnblockedInSem();
	}else
	{
		lockFlag = 1;
		//asm sti
		return 1;
	}
	//asm sti
}

#endif
