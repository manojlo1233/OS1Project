#include "LISTTHR.H"

#include <iostream.h>
#include "LOCK.H"

void ListThr::dodaj(Thread* t){

	lock
	ElemThr* node = new ElemThr();
	node->info = t;
	if ( head == 0) head = node;
	else tail->sledeci = node;
	tail = node;
	tail->sledeci = 0;
	unlock

}

Thread* ListThr::izbaci(){

	lock
	if ( head == 0) {
		unlock
		return 0;
	}
	else{
		if ( head == tail ) {
			ElemThr* tmp = head;
			Thread* ret = tmp->info;
			head = 0;
			tail = 0;

			unlock
			return ret;
		}
		else{
			ElemThr* tmp = head;
			while ( tmp->sledeci != tail) tmp = tmp->sledeci;
			tail = tmp;
			tmp = tmp->sledeci;
			tail->sledeci = 0;
			Thread* ret = tmp->info;

			unlock
			return ret;
		}
	}


}


