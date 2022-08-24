#ifndef _LIST_H_
#define _LIST_H_

#include "ELEMTHR.H"
#include "THREAD.H"

class ListThr{

public:
	ListThr(){
		head = 0;
		tail = 0;
	}


	ElemThr* head;
	ElemThr* tail;

	void dodaj(Thread* t);
	Thread* izbaci();



};




#endif
