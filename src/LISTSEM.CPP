#include "LISTSEM.H"
#include "ELEMSEM.H"
#include <iostream.h>
#include "LOCK.H"

extern void dispatch();

void ListSem::dodaj(Semaphore* t){

	lock
	ElemSem* node = new ElemSem();
	node->info = t;
	if ( head == 0) head = node;
	else tail->sledeci = node;
	tail = node;
	tail->sledeci = 0;
	unlock
}
Semaphore* ListSem::izbaci(){

	lock
	if ( head == 0){
		unlock
		return 0;
	}
	else{
		if ( head == tail ) {
			ElemSem* tmp = head;
			Semaphore* ret = tmp->info;
			head = 0;
			tail = 0;
			unlock
			return ret;
		}
		else{
			ElemSem* tmp = head;
			while ( tmp->sledeci != tail) tmp = tmp->sledeci;
			tail = tmp;
			tmp = tmp->sledeci;
			tail->sledeci = 0;
			Semaphore* ret = tmp->info;
			unlock
			return ret;
		}
	}


}

