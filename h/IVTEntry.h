/*
 * IVTEntry.h
 *
 *  Created on: Apr 30, 2020
 *      Author: OS1
 */

#ifndef IVTENTRY_H_
#define IVTENTRY_H_

typedef void interrupt (*pInterrupt)(...);

class IVTEntry {

	int numEntry;
public:

	pInterrupt oldRout;
	pInterrupt newRout;

	IVTEntry(int num, pInterrupt interruptRoutine);
	virtual ~IVTEntry();

	void signal();

	void callReal();

};

#endif /* IVTENTRY_H_ */
