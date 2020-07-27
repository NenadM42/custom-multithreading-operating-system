/*
 * DList.h
 *
 *  Created on: May 19, 2020
 *      Author: OS1
 */

//#include "PCB.h"
#include <stdio.h>

#ifndef DLIST_H_
#define DLIST_H_

class PCB;

typedef void (*SignalHandler)();


class DoubleListSignalId
{
public:
	struct Node
		{
			unsigned id;
			Node *next;
			Node *prev;

			Node(unsigned _id,Node *n = NULL,Node *prv = NULL) : id(_id), next(n), prev(prv)
			{

			}
		};

private:
	Node *first;
	Node *last;
	int numberOfElements;



public:

	DoubleListSignalId();
	~DoubleListSignalId();

	void removeNode(Node *node);

	int getNumberOfElements();
	void callFunctions();
	void insertAtEndSignalId(unsigned id);

	void print();


	unsigned getFirst();

};

class DoubleListSignalHandler
{
public:
	struct Node
		{
			SignalHandler mySignalHandler;
			Node *next;
			Node *prev;

			Node(SignalHandler sigHandler,Node *n = NULL,Node *prv = NULL) : mySignalHandler(sigHandler), next(n), prev(prv)
			{

			}
		};

private:
	Node *first;
	Node *last;
	int numberOfElements;

public:


	DoubleListSignalHandler();
	~DoubleListSignalHandler();


void print();

void callFunctions();

Node *getFirstNode();

void createCopy(Node *first);

void insertAtEndHandler(SignalHandler signalHandler);

void removeAll();

void swap(SignalHandler hand1, SignalHandler hand2);


void signal();

};



class DoubleList {
private:

public:

	struct Node
		{
			PCB *myPCB;
			Node *next;
			Node *prev;
			Node *otherNode;
			Node(PCB *p,Node *n = NULL,Node *prv = NULL,Node *otherN = NULL) : myPCB(p), next(n), prev(prv), otherNode(otherN)
			{

			}
		};



private:
	Node *first;
	Node *last;
	int numberOfElements;
	// Pokazuje na cvoru globalnoj listi ako je u semaforskoj i obrnutno


public:

	DoubleList();
	virtual ~DoubleList();



	void decSemaphores();

	void insertAtBeginning(PCB *pcb);

	Node* insertAtEnd(PCB *pcb,Node *otherNode = NULL);

	void setOtherNodeAtEnd(Node *otherNode);

	void print();

	void removeNode(Node *node);

	PCB *getFirst();

	int isEmpty();

	int getNumberOfElements();


};





#endif /* DLIST_H_ */

