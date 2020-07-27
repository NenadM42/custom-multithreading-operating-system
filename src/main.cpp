//pretpostavljeni memorijski model: huge

#include "PCB.h"
#include <stdio.h>
#include "DList.h"
#include "Event.h"

#ifndef BCC_BLOCK_IGNORE

// Zabranjuje prekide
#define lock asm cli

// Dozvoljava prekide
#define unlock asm sti

#define PREPAREENTRY(numEntry, callOld)\
void interrupt inter##numEntry(...); \
IVTEntry newEntry##numEntry(numEntry, inter##numEntry); \
void interrupt inter##numEntry(...) {\
	newEntry##numEntry.signal();\
	if(callOld == 1)\
		newEntry##numEntry.callReal();\
}

PREPAREENTRY(9,1)

class IdleThread : public Thread
{
public:

	IdleThread(): Thread(){}
	~IdleThread()
	{
		waitToComplete();
	}
protected:

	void run();

};

void IdleThread::run()
{
	//cout << "Uslo u idle nit\n";
	while(1)
	{
		//cout << "idle\n";
	}
}

//PREPAREENTRY(9,1)

volatile unsigned int lockFlag = 1;
volatile unsigned int fromDispatch = 0;


unsigned tsp;
unsigned tss;
unsigned tbp;

volatile int brojac = 1;
volatile int zahtevana_promena_konteksta = 0;

PCB *running = NULL;


PCB *idlePCB;
PCB* prethodni;

DoubleList globalSemaphoreList;



void interrupt timer(){	// prekidna rutina

	if (!zahtevana_promena_konteksta && running->kvant != 0)
	{
		brojac--;

		globalSemaphoreList.decSemaphores();
	}
	if (zahtevana_promena_konteksta == 1 || (  (brojac == 0) && (running->kvant != 0 )) || running->done == 1) {

		if(lockFlag == 1)
		{
			asm {
				// cuva sp
				mov tsp, sp
				mov tss, ss
				mov tbp, bp
				}

				running->sp = tsp;
				running->ss = tss;
				running->bp = tbp;

				//cout << "\n";
				//cout << "Running ID: " << running->id << "\n";
				//asm cli
				//cout << "I ova nit je blokirana: " << running->isBlocked() << "\n";

				PCB  *prev = running;

				if(!running->done && !running->isBlocked())
					Scheduler::put((PCB*)running);
				running = Scheduler::get();

				// Idle nit ima id = 1
				if(running == 0)
				{
					// Idle nit ima id = 1
					running = Thread::getThreadById(1)->myPCB;
				}
				if(prev->removed == 1)
				{
				    //cout << "BRISEMO PRETHODNI!\n";
					//asm cli
					//delete prev;
				}

		 	    //cout << "Running id sledeci:: " << running->id << "\n";
				//asm cli

				tsp = running->sp;
				tss = running->ss;
				tbp = running->bp;

				brojac = running->kvant;

				asm {
					mov sp, tsp   // restore sp
					mov ss, tss
					mov bp, tbp
				}

			// Pozovemo sve signale sto treba
		}
	}


	// poziv stare prekidne rutine koja se
     // nalazila na 08h, a sad je na 60h
     // poziva se samo kada nije zahtevana promena
     // konteksta – tako se da se stara
     // rutina poziva samo kada je stvarno doslo do prekida
if(!zahtevana_promena_konteksta) asm int 60h;


zahtevana_promena_konteksta = 0;

asm sti
//lockFlag = 0;
running->doSignals();
//lockFlag = 1;

}

void dispatch(){ // sinhrona promena konteksta
	asm cli;
	zahtevana_promena_konteksta = 1;
	timer();
	asm sti;
}

unsigned oldTimerOFF, oldTimerSEG; // stara prekidna rutina

// postavlja novu prekidnu rutinu
void inic(){
	asm{
		cli
		push es
		push ax

		mov ax,0   //  ; inicijalizuje rutinu za tajmer
		mov es,ax

		mov ax, word ptr es:0022h //; pamti staru rutinu
		mov word ptr oldTimerSEG, ax
		mov ax, word ptr es:0020h
		mov word ptr oldTimerOFF, ax

		mov word ptr es:0022h, seg timer	 //postavlja
		mov word ptr es:0020h, offset timer //novu rutinu

		mov ax, oldTimerSEG	 //	postavlja staru rutinu
		mov word ptr es:0182h, ax //; na int 60h
		mov ax, oldTimerOFF
		mov word ptr es:0180h, ax

		pop ax
		pop es
		sti
	}
}

// vraca staru prekidnu rutinu
void restore(){
	asm {
		cli
		push es
		push ax

		mov ax,0
		mov es,ax


		mov ax, word ptr oldTimerSEG
		mov word ptr es:0022h, ax
		mov ax, word ptr oldTimerOFF
		mov word ptr es:0020h, ax

		pop ax
		pop es
		sti
	}
}







class userThread : public Thread
{
public:

	userThread(int a, int b): Thread(a,b){}
	~userThread()
	{
		waitToComplete();
	}
protected:

	void run();

};

#include "user.h"

void userThread::run()
{
	//cout << "Uslo u idle nit\n";
	  char *args[4] = {"Javni","500","5","1"};
	  userMain(4,args);
}

/*
void signal1()
{
	cout << "Uslo u signal 1! ";
	cout << "Running id: " << running->id << "\n";
}
*/
void signal2()
{
	cout << "Uslo u signal 2! ";
	cout << "Running id: " << running->id << "\n";
}




int main(int arg, char *argv[]){

      for(int i = 0;i<16;i++)
      {
    	  globalBlockedSignals[i] = 0;
      }

	  Thread *main = new Thread();
	  running = main->myPCB;

	  inic();
	  IdleThread idle;

	  main->registerHandler(2,signal2);
	  userThread *trd = new userThread(1024,1);
	  trd->start();
	  delete trd;

	  //miniTest();
	  restore();

	  exit(0);
	  //return 0;
}

#endif
