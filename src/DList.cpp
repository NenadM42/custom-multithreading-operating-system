/*
 * DoubleList.cpp
 *
 *  Created on: May 19, 2020
 *      Author: OS1
 */

#include "DList.h"
#include "PCB.h"

extern volatile unsigned int lockFlag;

DoubleList::DoubleList() {
	first = NULL;
	last = NULL;
	numberOfElements = 0;
}

DoubleList::~DoubleList() {
	// TODO Auto-generated destructor stub

	// Izbrisemo listu
	Node *p = first;
	Node *prev = NULL;
	while(p != NULL)
	{
		prev = p;
		p = p->next;
		delete prev;
	}
}

DoubleListSignalId::DoubleListSignalId() {
	first = NULL;
	last = NULL;
	numberOfElements = 0;
}

DoubleListSignalId::~DoubleListSignalId() {
	// TODO Auto-generated destructor stub
		Node *p = first;
		Node *prev = NULL;
		while(p != NULL)
		{
			prev = p;
			p = p->next;
			delete prev;
		}

}

DoubleListSignalHandler::DoubleListSignalHandler() {
	first = NULL;
	last = NULL;
	numberOfElements = 0;
}

DoubleListSignalHandler::~DoubleListSignalHandler() {
	// TODO Auto-generated destructor stub
		Node *p = first;
		Node *prev = NULL;
		while(p != NULL)
		{
			prev = p;
			p = p->next;
			delete prev;
		}
}

void DoubleListSignalId::insertAtEndSignalId(unsigned id)
{
	numberOfElements++;
	Node *new_node = new Node(id);
	new_node->prev = last;
	new_node->next = NULL;
	if(last == NULL)
	{
		last = new_node;
		first = last;
	}else
		{
		last->next = new_node;
		last = new_node;
		}
}


void DoubleListSignalId::callFunctions()
{

	Node *p = first;
	Node *prev = NULL;

	while(p != NULL)
	{
		prev = p;

		if(running->blockedSignals[p->id] == 0 && globalBlockedSignals[p->id] == 0)
		{
			Node *nextNode = p->next;
			lockFlag = 0;
			running->signalList[p->id].signal();
			removeNode(p);
			//cout << "Globalna posle:\n";
			//globalSemaphoreList.print();

			lockFlag = 1;

			if(p->id == 0)
				dispatch();

			p = nextNode;


			continue;
		}
		p = p->next;
	}
}


void DoubleListSignalHandler::removeAll()
{
	Node *p = first;
	Node *prev = NULL;

	while(p != NULL)
	{
		if(prev != NULL)
			delete prev;
		prev = p;
		p = p->next;
	}
}



void DoubleListSignalHandler::swap(SignalHandler hand1, SignalHandler hand2)
{
	// Nadjemo prvi
	Node *handNode1 = first;

	while(handNode1 != NULL)
	{
		if(handNode1->mySignalHandler == hand1)
		{
			break;
		}
	}
	// Nadjemo drugi
	Node *handNode2 = first;
	while(handNode2 != NULL)
	{
		if(handNode2->mySignalHandler == hand2)
		{
			break;
		}
	}

	// Ako nije nasao neki onda se nista ne desava
	if(handNode1 == NULL || handNode2 == NULL)
		return;

	// Ako jeste onda samo zamenimo
	SignalHandler temp = handNode1->mySignalHandler;
	handNode1->mySignalHandler = handNode2->mySignalHandler;
	handNode2->mySignalHandler = temp;
}

void DoubleListSignalHandler::signal()
{
	Node *p = first;

	//cout << "Ima li:\n";
	while(p != NULL)
	{
		//cout << "Ima nesco\n";
		p->mySignalHandler();
		p = p->next;
	}
}


DoubleListSignalHandler::Node* DoubleListSignalHandler::getFirstNode()
{
	return first;
}


void DoubleListSignalHandler::createCopy(Node *otherNode)
{
	if(otherNode == NULL)
	{
		return;
	}

	first = new Node(otherNode->mySignalHandler);
	first->prev = NULL;
	first->next = NULL;

	Node *p = first;

	otherNode = otherNode->next;
	while(otherNode != NULL)
	{
		p->next = new Node(otherNode->mySignalHandler);
		p->next->prev = p;
		p->next->next = NULL;
		p = p->next;
		otherNode = otherNode->next;
	}
	last = p;
}



void DoubleListSignalHandler::insertAtEndHandler(SignalHandler signalHandler)
{

	Node *new_node = new Node(signalHandler);

	new_node->prev = last;
	new_node->next = NULL;
	if(last == NULL)
	{

		last = new_node;
		first = last;
	}else
		{
		last->next = new_node;
		last = new_node;
	}
}


void DoubleList::decSemaphores()
{
	// Prodjemo kroz celu listu i dekrementuje pcb blocked counter

	Node *p = first;
	Node *prev = NULL;

	while(p != NULL)
	{
		prev = p;
		p->myPCB->decBlockedTimeLeft();
		// Ako je sada blockedTime = 0 onda aktiviramo ovu nit
		if(p->myPCB->getBlockedTimeLeft() == 0)
		{
			Node *nextNode = p->next;

			//cout << "Globalna pre:\n";
			//globalSemaphoreList.print();
			//cout << "\n Semaforska pre:\n";
			//p->myPCB->getSemaphore()->blockedList.print();


			// Ali pre toga izbrisemo nit i u njenom semaforu
			DoubleList::Node *nodeInSem = p->otherNode;
			//p->myPCB->blockedList.removeNode(nodeInSem);
			// Sad stavimo u scheduler

			p->myPCB->getSemaphore()->incValue();
			p->myPCB->setUnblockedInGlobal();
			p->myPCB->blocked = 0;

			Scheduler::put((PCB*)p->myPCB);

			// I sad izbrisemo i odavde

			p->myPCB->getSemaphore()->blockedList.removeNode(nodeInSem);
			//p->myPCB->getSemaphore()->blockedList.print();

			//cout << "\n Semaforska posle:\n";
			//p->myPCB->getSemaphore()->blockedList.print();

			removeNode(p);

			//cout << "Globalna posle:\n";
			//globalSemaphoreList.print();

			p = nextNode;
			continue;
		}
		p = p->next;
	}
}



void DoubleList::insertAtBeginning(PCB *pcb)
{
	numberOfElements++;
	Node *new_node = new Node(pcb);
	new_node->next = first;
	new_node->prev = NULL;

	// Ako first nije null onda je njegov prev sada ovaj novi node
	if(first != NULL)
	{
		first->prev = new_node;
		first = new_node;
	}else
	{
		first = new_node;
		last = first;
	}
}

DoubleList::Node* DoubleList::insertAtEnd(PCB *pcb, Node *otherNode)
{
	numberOfElements++;
	Node *new_node = new Node(pcb);
	new_node->otherNode = otherNode;
	new_node->prev = last;
	new_node->next = NULL;
	if(last == NULL)
	{
		last = new_node;
		first = last;
	}else
	{
		last->next = new_node;
		last = new_node;
	}

	return new_node;
}


void DoubleList::setOtherNodeAtEnd(Node *otherNode)
{
	last->otherNode = otherNode;
}

int DoubleList::getNumberOfElements()
{
	return numberOfElements;
}

int DoubleListSignalId::getNumberOfElements()
{
	return numberOfElements;
}

PCB* DoubleList::getFirst()
{
	numberOfElements--;
	PCB *pcb = first->myPCB;
	Node *pom = first;
	first = first->next;
	// Ovo je jedini element znaci i sad je prazna
	if(first == NULL)
	{
		last = NULL;
		//delete pom;
		return pcb;
	}

	first->prev = NULL;

	pom->next = NULL;
	pom->prev = NULL;
	pom->myPCB = NULL;
	pom->otherNode = NULL;

	delete pom;

	return pcb;
}


unsigned DoubleListSignalId::getFirst()
{
	numberOfElements--;
	unsigned id = first->id;
	Node *pom = first;
	first = first->next;
	// Ovo je jedini element znaci i sad je prazna
	if(first == NULL)
	{
		last = NULL;
		return id;
	}

	first->prev = NULL;

	pom->next = NULL;
	pom->prev = NULL;

	delete pom;

	return id;
}
int DoubleList::isEmpty()
{
	if(first == NULL)
		return 1;
	else
		return 0;
}


void DoubleList::print()
{
	Node *p = first;
	while(p != NULL)
	{
		cout << p->myPCB->id << "\n";
		p = p->next;
	}
}

void DoubleListSignalId::print()
{
	Node *p = first;
	while(p != NULL)
	{
		cout << p->id << "\n";
		p = p->next;
	}
}

void DoubleListSignalHandler::print()
{
	Node *p = first;
	while(p != NULL)
	{
		cout << "Funckija u hanlderu" <<  endl;
		p = p->next;
	}
}


void DoubleList::removeNode(Node *node)
{
	numberOfElements--;
	if(node == first)
	{
		// Prvi je sad sledeci
		first = node->next;
		if(node->next == NULL)
		{
			last = NULL;
			return;
		}
		first->prev = NULL;
		node->next = NULL;
		node->prev = NULL;
		node->myPCB = NULL;
		node->otherNode = NULL;
		delete node;
	}else
	{
		// Da li je poslednji
		if(node->next == NULL)
		{
			last = node->prev;
			last->next = NULL;
			//node->prev = NULL;
			node->next = NULL;
			node->prev = NULL;
			node->myPCB = NULL;
			node->otherNode = NULL;
			delete node;
		}else
		{
			Node *prev = node->prev;
			node->prev->next = node->next;
			node->next->prev = node->prev;
			node->next = NULL;
			node->prev = NULL;
			delete node;
		}
	}
}

void DoubleListSignalId::removeNode(Node *node)
{
	numberOfElements--;
	if(node == first)
	{
		// Prvi je sad sledeci
		first = node->next;
		if(node->next == NULL)
		{
			last = NULL;
			return;
		}
		first->prev = NULL;
		node->next = NULL;
		node->prev = NULL;
		delete node;
	}else
	{
		// Da li je poslednji
		if(node->next == NULL)
		{
			last = node->prev;
			last->next = NULL;
			//node->prev = NULL;
			node->next = NULL;
			node->prev = NULL;
			delete node;
		}else
		{
			Node *prev = node->prev;
			node->prev->next = node->next;
			node->next->prev = node->prev;
			node->next = NULL;
			node->prev = NULL;
			delete node;
		}
	}
}
