/*
 * Timer.h
 *
 *  Created on: Apr 26, 2020
 *      Author: OS1
 */

#ifndef TIMER_H_
#define TIMER_H_

static volatile unsigned int lockFlag = 1;

//pomocne promenljive za prekid tajmera


static volatile int brojac = 20;
static volatile int zahtevana_promena_konteksta = 0;


void interrupt timer();


#endif /* TIMER_H_ */
