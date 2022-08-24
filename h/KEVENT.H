#ifndef	_KEVENT_H_
#define _KEVENT_H_

#include "EVENT.H"
#include "PCB.H"

class Semaphore;

class KernelEv{

public:

	KernelEv(unsigned char brojUlaza, Event* pointer);
	void wait();
	void signal();
	unsigned int brojUlaza;
	Event* myEvent;
	Semaphore* mySem;


	~KernelEv();



};


#endif
