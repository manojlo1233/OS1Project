#ifndef	_IDLE_H_
#define _IDLE_H_

#include "EVENT.H"
#include "THREAD.H"
#include "SEM.H"
#include "KEVENT.H"
#include "IVTE.H"
#include <stdio.h>
#include "LISTTHR.H"


extern volatile int brojAktivnihNiti;
extern PCB* mainPCB;

class Idle : public Thread{
public:
	Idle(unsigned long stackSize = defaultStackSize,unsigned int timeSlice = defaultTimeSlice):Thread(stackSize,timeSlice){}
	void run(){

		while(1){

			if (brojAktivnihNiti == 0) {
				Scheduler::put((PCB*)(mainPCB));
			}

			dispatch();

		}


	}



};

#endif
