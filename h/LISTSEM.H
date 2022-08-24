#ifndef _SEMLIST_H_
#define _SEMLIST_H_

#include "SEM.H"
#include "ELEMSEM.H"

class ListSem{

public:
	ListSem(){
		head = 0;
		tail = 0;
	}


	ElemSem* head;
	ElemSem* tail;

	void dodaj(Semaphore* t);
	Semaphore* izbaci();



};




#endif
