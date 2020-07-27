/*
 * PCBLists.h
 *
 *  Created on: Apr 27, 2020
 *      Author: OS1
 */

#ifndef PCBLISTS_H_
#define PCBLISTS_H_


#include <iostream.h>
class PCB;

class PCBList
{
private:
	int number_of_elements;
public:
	struct Node
	{
		PCB *myPCB;
		Node *next;

		Node(PCB *p,Node *n = NULL) : myPCB(p), next(n)
		{

		}
	};

	Node *first;
	Node *last;

	PCBList();

	void insert(PCB *pcb);

	void print();

	int isEmpty();

	PCB *getFirst();

	void decBlockedTime();

	int getNumberOfElements()
	{
		return number_of_elements;
	}

};


#endif /* PCBLISTS_H_ */
