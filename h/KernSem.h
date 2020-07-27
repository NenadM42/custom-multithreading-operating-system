/*
 * KernSem.h
 *
 *  Created on: Apr 29, 2020
 *      Author: OS1
 */

#ifndef KERNSEM_H_
#define KERNSEM_H_


#include "PCBLists.h"
#include "Semaphor.h"
#include "DList.h"




static const int max_number_of_sem = 1000;

extern KernelSem *kernelSem_arr[max_number_of_sem];
//KernelSem *kernelSem_arr[number_of_sem];
extern int semCounter;

// Ovde stoje sve niti koje se nalaze u nekim semaforima
extern DoubleList globalSemaphoreList;


class KernelSem {
private:
	int value;
	int id;
public:
	KernelSem(int initValue = 0);
	virtual ~KernelSem();

	void incValue();


	int wait(Time maxTimeToWait);

	int signal(int n);

	int val();

	DoubleList blockedList;

};

#endif /* KERNSEM_H_ */
