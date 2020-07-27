/*
 * PCBLists.cpp
 *
 *  Created on: Apr 28, 2020
 *      Author: OS1
 */

#include "PCB.h"

#ifndef BCC_BLOCK_IGNORE


	PCBList::PCBList()
	{
		first = NULL;
		last = NULL;
		number_of_elements = 0;
	}

	void PCBList::decBlockedTime()
	{
		// Prolazimo kroz listu smanjujemo blockedTime, ako dodje do 0 izbacimo taj element iz liste
		for(int i = 0;i<100;i++)
			cout << "WAT\n";

		Node *p = first;
		Node *prev = NULL;


		if(p == NULL)
			return;
		print();
		cout << "\n";

		while(p != NULL)
		{
			for(int i = 0;i<100;i++)
			cout << "WAT\n";
			p->myPCB->decBlockedTimeLeft();
			if(p->myPCB->getBlockedTimeLeft() == 0)
			{

				for(int i = 0;i<100;i++)
				cout << "Uslo\n";
				p->myPCB->blocked = 0;
				Scheduler::put((PCB*)p->myPCB);

				// Znaci brisemo prvi element
				if(prev == NULL)
				{
					cout << "HELPPPP\n";
					first = p->next;
					//delete p;
					if(last == p)
						last = first;
					p = first;
				}else
				{
					prev->next = p->next;
					//delete p;
					p = prev->next;
				}

				if(p == NULL)
				{
					cout << "p je null\n";
				}
				if(prev == NULL)
				{
					cout << "prev je null\n";
				}
				if(first == NULL)
				{
					cout << "first je null\n";
				}
				if(last == NULL)
				{
					cout << "Last je null\n";
				}



			}else
			{
				if(p == NULL || p->next == NULL)
					break;
				p = p->next;
			}
				prev = p;
		}
	}

	void PCBList::insert(PCB *pcb)
	{
		number_of_elements++;
		if(first == NULL)
		{
			first = new Node(pcb);
			last = first;
		}else
		{
			last->next = new Node(pcb);
			last = last->next;
		}
	}

	void PCBList::print()
	{
		Node *p = first;
		while(p != NULL)
		{
			cout << p->myPCB->getId() << "\n";
			p = p->next;
		}
	}

	int PCBList::isEmpty()
	{
		if(first == NULL)
			return 1;
		else
			return 0;
	}

	PCB *PCBList::getFirst()
	{
		number_of_elements--;
		PCB *pcb = first->myPCB;
		first = first->next;
		if(first == NULL)
		{
			last = NULL;
		}
		return pcb;
	}

#endif
