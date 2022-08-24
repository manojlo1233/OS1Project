#include "THREAD.H"
#include "PCB.H"
#include "ELEMTHR.H"
#include <iostream.h>
#include <dos.h>
#include <stdio.h>

#include "LISTTHR.H"
#include "LOCK.H"

extern int brojAktivnihNiti;

int Thread::globalni_ID = 0;


int Thread::getRunningId(){
	return PCB::running->myThr->privatni_ID;
}

Thread* Thread::getThreadById(int id) {


	lock
	if ( id >= Thread::globalni_ID){
		cout << "Ne postoji nit sa takvom identifikacijom!" << endl;
		unlock
		return 0;
	}
	ElemThr* tmp = lista->head;
	if ( tmp == 0){
		cout << "Ne postoji nit sa takvom identifikacijom!" << endl;
		unlock
		return 0;
	}
	while(tmp->info->getId() != id){
		if ( tmp == lista->tail && tmp->info->getId() != id){
			cout << "Ne postoji nit sa takvom identifikacijom!" << endl;
			unlock
			return 0;
		}
		else tmp = tmp->sledeci;
	}
	unlock
	return tmp->info;


}

void Thread::start(){

	lock
	brojAktivnihNiti++;
	this->myPCB->startovana = 1;
	Scheduler::put(this->myPCB);
	unlock
	dispatch();
}

int Thread::getId(){
	if (this == 0) return -1;
	else return this->privatni_ID;
}

void Thread::waitToComplete(){


	if (!this->myPCB->zavrsio && this->myPCB->startovana && this->myPCB != PCB::running && !this->myPCB->zaludna){
		lock
		mojeBlokirane->dodaj(PCB::running->myThr);
		PCB::running->zablokirana = 1;
		unlock
		dispatch();
	}

}


Thread::Thread(StackSize stackSize, Time timeSlice){
	lock
	myPCB = new PCB(stackSize,timeSlice,this, PCB::wrapper);
	this->privatni_ID = globalni_ID++;
	lista->dodaj(this);
	this->mojeBlokirane = new ListThr();
	unlock

}
Thread::~Thread(){


	this->waitToComplete();
	lock
	delete myPCB;
	delete this->mojeBlokirane;
	if(lista->head != 0){
		ElemThr* prethodni = 0;
		ElemThr* tmp = lista->head;
		while ( tmp->info != this && tmp != lista->tail){
			prethodni = tmp;
			tmp = tmp->sledeci;
		}
		if ( tmp != lista->tail || (tmp == lista->tail && tmp->info == this)){

			if ( prethodni == 0) {
				lista->head = lista->head->sledeci;
				tmp->sledeci = 0;
			}
			else{
				prethodni->sledeci = tmp->sledeci;
				tmp->sledeci = 0;
			}

			delete tmp;

		}
	}
	unlock
}

