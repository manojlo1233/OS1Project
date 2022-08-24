#include "PCB.H"
#include <iostream.h>

#include "LISTTHR.H"
#include "LOCK.H"



PCB* PCB::running = 0;
extern ListThr* lista;
extern volatile int brojAktivnihNiti;


void PCB::wrapper(){

	PCB::running->myThr->run();
	lock
	PCB::running->zavrsio = 1;
	int stavi = 1;

	ElemThr* tmp = PCB::running->myThr->mojeBlokirane->head;

	if ( tmp != 0){

		while ( tmp != 0) {

			stavi = 1;
			ElemThr* tmp1 = lista->head;
			if ( tmp1 != 0){

				while ( tmp1 != 0){
					if ( tmp1->info->myPCB->zavrsio == 1) tmp1 = tmp1->sledeci;
					else{
						ElemThr* tmp2 = tmp1->info->mojeBlokirane->head;
						if ( tmp2 != 0){
							while( tmp2 != 0){
								if ( tmp->info == tmp2->info) stavi = 0;
								tmp2 = tmp2->sledeci;
							}
						}

						tmp1 = tmp1->sledeci;
					}

				}
			}

			if (stavi){
				tmp->info->myPCB->zablokirana = 0;
				Scheduler::put((PCB*)(tmp->info->myPCB));
			}
			tmp = tmp->sledeci;

		}
	}


	brojAktivnihNiti--;
	unlock


	dispatch();

}

PCB::PCB(unsigned long stackSize, unsigned int timeSlice, Thread* thr,void (*body)()){
	lock
	if ( stackSize < 1024) stackSize = 1024;
	stackSize /= sizeof(unsigned);
	this->velicinaSteka = stackSize;
	this->st = new unsigned[stackSize];

	st[stackSize - 1] =0x200;

	st[stackSize - 2] = FP_SEG(body);
	st[stackSize - 3] = FP_OFF(body);
	this->sp = FP_OFF(st + stackSize - 12);
	this->ss = FP_SEG(st + stackSize - 12);
	this->myThr = thr;
	this->zavrsio = 0;
	this->zablokirana = 0;
	this->startovana = 0;
	this->zaludna = 0;
	this->vremeNaSemaforu = -1;
	this->bp = this->sp;
	this->prviBP = this->bp;
	if(timeSlice != 0){
		this->kvant = timeSlice;
	}
	else this->kvant = -1;

	unlock

}

PCB::~PCB(){
	lock
	this->myThr = 0;;
	delete[] this->st;
	unlock
}

void PCB::postaviRunning(Thread* t){
	lock
	t->myPCB->startovana = 1;
	PCB::running = t->myPCB;
	unlock
}


