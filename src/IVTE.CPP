#include "IVTE.H"
#include <dos.h>
#include "KEVENT.H"
#include "EVENT.H"
#include <stdio.h>
#include <stdlib.h>
#include "LOCK.H"

IVTEntry** IVTEntry::IVTniz = new IVTEntry*[255];


IVTEntry::IVTEntry(unsigned char ivtNo, void interrupt (*nova)(...)){

	lock
	IVTEntry::IVTniz[ivtNo] = this;

	this->brojUlaza = ivtNo;
	this->myKernelEv = 0;
	this->staraRutina = getvect(brojUlaza);
	setvect(brojUlaza, nova);
	unlock
}

IVTEntry::~IVTEntry(){

	lock
	setvect(this->brojUlaza,this->staraRutina );
	IVTEntry::IVTniz[brojUlaza] = 0;
	unlock
}

void IVTEntry::signal(){

	this->myKernelEv->signal();

}
