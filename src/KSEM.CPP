#include "KSEM.H"
#include "PCB.H"
#include <iostream.h>
#include "LOCK.H"

extern void dispatch();

KernelSem::KernelSem(int value,Semaphore* pointer): val(value){
	lock
	this->mySem = pointer;
	unlock
}

int KernelSem::wait(unsigned int maxTimeToWait){

		lock
		this->val--;

		if ( this->val < 0){

			if ( maxTimeToWait == 0){
				this->mySem->listNoTime->dodaj(PCB::running);
				PCB::running->zablokirana = 1;
			}
			else{

				PCB::running->vremeNaSemaforu = maxTimeToWait;
				this->mySem->listTime->dodaj(PCB::running);
				PCB::running->zablokirana = 1;
			}

		unlock
			dispatch();
		lock

			if ( PCB::running->vremeNaSemaforu == -1 || PCB::running->vremeNaSemaforu > 0) {
				PCB::running->vremeNaSemaforu = -1;
				unlock
				return 1;
			}
			else {
				PCB::running->vremeNaSemaforu = -1;
				unlock
				return 0;
			}

		}
		unlock
		return 2;

}

void KernelSem::signal(){

	lock
	if ( this->val < 0){
		if ( this->mySem->listNoTime->head != 0) {
			PCB* tmp = this->mySem->listNoTime->izbaci();
			if ( tmp == 0 ){
				tmp = this->mySem->listTime->izbaci();
					if ( tmp == 0) {
						val++;
						unlock
						return;
					}
				tmp->zablokirana = 0;
				Scheduler::put((PCB*)tmp);
			}
			else{
				tmp->zablokirana = 0;
				Scheduler::put((PCB*)tmp);
			}

		}
	}
	val++;
	unlock


}

KernelSem::~KernelSem(){
	lock
	mySem = 0;
	unlock
}
