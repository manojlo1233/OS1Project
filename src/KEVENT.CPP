#include "KEVENT.H"
#include "IVTE.H"
#include "SEM.H"
#include <iostream.h>
#include "LOCK.H"



KernelEv::KernelEv(unsigned char ulaz,Event* pointer){

	lock
	IVTEntry::IVTniz[ulaz]->myKernelEv = this;
	this->brojUlaza = ulaz;
	this->myEvent = pointer;
	this->mySem = new Semaphore(0);
	unlock
}

void KernelEv::wait(){

	if ( PCB::running == this->myEvent->myPCB) mySem->wait(0);

}

void KernelEv::signal(){
	lock
	if (mySem->val() < 0) mySem->signal();
	unlock
}

KernelEv::~KernelEv(){
	lock
	IVTEntry::IVTniz[this->brojUlaza]->myKernelEv = 0;
	this->myEvent = 0;
	delete this->mySem;
	unlock
}
