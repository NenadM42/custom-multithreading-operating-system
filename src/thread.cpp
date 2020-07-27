/*
 * thread.cpp
 *
 *  Created on: Apr 25, 2020
 *      Author: OS1
 */
#include "pcb.h"
#include "Vars.h"

#ifndef BCC_BLOCK_IGNORE


extern PCB *PCB_arr[max_number_of_threads];

extern volatile unsigned int lockFlag;


void signal0()
{
	//cout << "Uslo u signal 0!\n";
	running->done = 1;
	running->removed = 1;
}

void Thread::start()
{
	//cout << "Ubacujemo ovaj id scheduler: " << myPCB->id << "\n";
	lockFlag = 0;
	Scheduler::put((PCB*)myPCB);
	lockFlag = 1;
}

void Thread::signal(SignalId signal)
{
	// Prodjemo kroz listu i pozovemo sve funkcije
	//lockFlag = 0;
	//signalList[signal].signal();
	//lockFlag = 1;

	//cout << "Ubacujemo na kraj signal id \n";

	lockFlag = 0;
	myPCB->doSignalsList.insertAtEndSignalId(signal);
	lockFlag = 1;

	//myPCB->doSignalsList.print();


}


void Thread::blockSignal(SignalId signal)
{
	myPCB->setBlockedSignal(signal,1);
}

void Thread::blockSignalGlobally(SignalId signal)
{
	globalBlockedSignals[signal] = 1;
}


void Thread::unblockSignal(SignalId signal)
{
	myPCB->setBlockedSignal(signal,0);
}

void Thread::unblockSignalGlobally(SignalId signal)
{
	globalBlockedSignals[signal] = 0;
}

void Thread::registerHandler(SignalId signal, SignalHandler handler)
{
	//cout << "Registruej signal " << signal <<  " za ovaj id: " << myPCB->id <<"\n";
	lockFlag = 0;
	myPCB->signalList[signal].insertAtEndHandler(handler);
	lockFlag = 1;
}

void Thread::unregisterAllHandlers(SignalId id)
{
	lockFlag = 0;
	myPCB->signalList[id].removeAll();
	lockFlag = 1;
}

void Thread::swap(SignalId id, SignalHandler hand1, SignalHandler hand2)
{
	lockFlag = 0;
	myPCB->signalList[id].swap(hand1,hand2);
	lockFlag = 1;
}

 void Thread::waitToComplete()
 {
	 // Ako je vec zavrsena onda ne radimo nista ovde
	 if(myPCB->done == 1)
		 return;

	 lockFlag = 0;
	 running->blocked = 1;
	 myPCB->insertToBlockedList(running);

	// cout <<"ID KOJI SE CEKA JE: "<<myPCB->id << " A ELEMENTI KOJi GA CEKAJU SU: ";
	 //myPCB->blockedList.print();

	// cout << "-------------\n";
	 lockFlag = 1;
	 dispatch();
 }



 ID Thread::getId()
 {
	 return this->myPCB->id;
 }


 ID Thread::getRunningId()
 {
	 return running->id;
 }

 Thread * Thread::getThreadById(ID id)
 {
	 return PCB_arr[id]->myThread;
 }

 Thread::~Thread()
 {
	 this->waitToComplete();
	 // Znaci pcb brisemo u prekidnoj rutini
	 myPCB->removed = 1;
	 //delete myPCB;
 }




//protected:
 Thread::Thread(StackSize stackSize, Time timeSlice)
 {
	 lockFlag = 0;
	 myPCB = new PCB(stackSize,timeSlice,this);
	 lockFlag = 1;
	 registerHandler(0,signal0);
}


 void Thread::run()
 {
	//cout << "ELEMENT KOJI POZIVA waitToComplete je id: " << PCB_arr[1]->id << "\n";
	 //PCB_arr[1]->myThread->waitToComplete();

	 for(int i = 0;i<7;i++)
	 			 	{

	 			 	cout << "U runu threada , id: " << running->id <<" i: " << i <<"\n";
	 			 		for(int k = 0;k<30000;k++)
	 			 			for(int j = 0;j<30000;j++);
	 			 	}
 }


#endif
