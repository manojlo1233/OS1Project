#include "SEM.H"
#include "KSEM.H"
#include "LISTSEM.H"
#include "LOCK.H"



extern ListSem* listaSemafora;

Semaphore::Semaphore(int init ){
	lock
	listNoTime = new ListPCB();
	listTime = new ListPCB();
	this->myImpl = new KernelSem(init,this);
	listaSemafora->dodaj(this);
	unlock
}

Semaphore::~Semaphore (){
	lock
	delete listNoTime;
	delete listTime;
	delete myImpl;
	if(listaSemafora->head != 0){
			ElemSem* prethodni = 0;
			ElemSem* tmp = listaSemafora->head;
			while ( tmp->info != this && tmp != listaSemafora->tail){
				prethodni = tmp;
				tmp = tmp->sledeci;
			}
			if ( tmp != listaSemafora->tail || (tmp == listaSemafora->tail && tmp->info == this)){

				if ( prethodni == 0) {
					listaSemafora->head = listaSemafora->head->sledeci;
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

void Semaphore::signal(){

	lock
	this->myImpl->signal();
	unlock
}
int Semaphore::wait(Time maxTimetoWait){
	return this->myImpl->wait(maxTimetoWait);
}

int Semaphore::val() const{
	return this->myImpl->val;
}
