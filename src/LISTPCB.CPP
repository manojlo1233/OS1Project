#include "LISTPCB.H"
#include "ELEMPCB.H"
#include <iostream.h>
#include "LOCK.H"

extern void dispatch();

void ListPCB::dodaj(PCB* t){

	lock
	ElemPCB* node = new ElemPCB();
	node->info = t;
	if ( head == 0) head = node;
	else tail->sledeci = node;
	tail = node;
	tail->sledeci = 0;
	unlock
}

PCB* ListPCB::izbaci(){

	lock

	if ( head == 0) {
		unlock
		return 0;
	}
	else{
		if ( head == tail ) {
			ElemPCB* tmp = head;
			head = 0;
			tail = 0;
			unlock
			return tmp->info;
		}
		else{
			ElemPCB* tmp = head;
			head = head->sledeci;
			tmp->sledeci = 0;
			unlock
			return tmp->info;
		}
	}


}
