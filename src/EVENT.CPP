#include "EVENT.H"
#include "KEVENT.H"
#include "PCB.H"
#include "IVTE.H"
#include <iostream.h>
#include <dos.h>
#include "LOCK.H"

extern volatile int zahtevana_promena_konteksta;

extern volatile int blockFlag;



Event::Event(IVTNo ovtNo){
	lock
	this->myImpl = new KernelEv(ovtNo, this);
	this->myPCB = PCB::running;
	unlock

}


Event::~Event(){
	lock
	delete myImpl;
	unlock

}

void Event::wait(){
		this->myImpl->wait();
}

void Event::signal(){

	this->myImpl->signal();

}


