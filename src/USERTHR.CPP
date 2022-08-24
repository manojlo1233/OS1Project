#include <iostream.h>
#include <stdlib.h>
#include <assert.h>
#include <dos.h>


#include "keyevent.h"
#include "bounded.h"
#include "user.h"
#include "intLock.h"
#include "SEM.H"
#include "SCHEDULE.H"
#include "THREAD.H"

#include "PCB.H"
#include "ELEMTHR.H"
#include "USER.H"


#include "LISTPCB.H"
#include "ELEMPCB.H"
#include "IVTE.H"
#include "EVENT.H"
#include "KEVENT.H"
#include "LISTSEM.H"
#include "ELEMSEM.H"
#include "IDLE.H"
#include "LISTTHR.H"


extern int userMain (int argc, char* argv[]);
extern void tick();

#define lock blockFlag=1;

#define unlock blockFlag=0;\
 if (zahtevana_promena_konteksta) {\
 dispatch();\
}



unsigned tsp;
unsigned tss;
unsigned tbp;
volatile unsigned blockFlag = 0;
volatile int brojac = -1;
volatile int brojAktivnihNiti = 0;
volatile int zahtevana_promena_konteksta = 0;

PCB* mainPCB;
ListThr* lista;
Idle* zaludna = new Idle(4096,5);
ListSem* listaSemafora;



void incProtekao(){
	lock

	if ( listaSemafora->head != 0){

		ElemSem* sem = listaSemafora->head;
		while ( sem != 0){
			if ( sem->info->listTime->head != 0){
						ElemPCB* prethodni = 0;
						ElemPCB* trenutni = sem->info->listTime->head;
						while (trenutni != 0 ){
							trenutni->info->vremeNaSemaforu -= 1;
							if ( trenutni->info->vremeNaSemaforu == 0){

								if ( prethodni == 0){ 						//Izbacujemo element za pocetka liste
									ElemPCB* tmp = trenutni;
									tmp->info = sem->info->listTime->izbaci();
									tmp->sledeci = 0;
									tmp->info->zablokirana = 0;
									Scheduler::put((PCB*)(tmp->info));
									trenutni = sem->info->listTime->head;
								}
								else{										//Izbacujemo negde unutar liste
									prethodni->sledeci = trenutni->sledeci;
									ElemPCB* tmp = trenutni;
									trenutni = trenutni->sledeci;
									tmp->sledeci = 0;
									tmp->info->zablokirana = 0;
									Scheduler::put((PCB*)(tmp->info));

								}
							}
							else{

								prethodni = trenutni;
								trenutni = trenutni->sledeci;
							}
						}


					}
			sem = sem->sledeci;
		}


	}
	unlock
}


unsigned oldTimerOFF, oldTimerSEG; // stara prekidna rutina

void interrupt timer(){

	if (!zahtevana_promena_konteksta){
		brojac--;
		incProtekao();
		asm cli
		tick();
		asm int 60h;
	}


	if (brojac == 0 || zahtevana_promena_konteksta) {
		if (!blockFlag){

			zahtevana_promena_konteksta = 0;

				asm {
					mov tsp, sp
					mov tss, ss
					mov tbp, bp
				}

				PCB::running->sp = tsp;
				PCB::running->ss = tss;
				PCB::running->bp = tbp;

				if (!PCB::running->zavrsio && !PCB::running->zablokirana && !PCB::running->zaludna && PCB::running != mainPCB){
					Scheduler::put((PCB*)PCB::running);
				}


				if ((PCB::running = Scheduler::get()) == 0){

					PCB::postaviRunning(zaludna);
					PCB::running->zaludna = 1;

				}

				tsp = PCB::running->sp;
				tss = PCB::running->ss;
				tbp = PCB::running->bp;

				brojac = PCB::running->kvant;

				asm {
					mov sp, tsp
					mov ss, tss
					mov bp, tbp
				}

		}
		else {
			zahtevana_promena_konteksta = 1;
		}

	}





}

void dispatch(){ // sinhrona promena konteksta
	asm cli;
	zahtevana_promena_konteksta = 1;
	timer();
	asm sti;
}

// vraca staru prekidnu rutinu
void restore();





void napraviMainThread(){
	lock
	PCB::running = mainPCB = new PCB(4096,0,0,0);
	PCB::running->startovana = 1;
	unlock
}

//--------------------------------------------------------------------------


class UserThread: public Thread{
public:
	UserThread(int Uargc, char** Uargv):Thread(4096,1){
		this->Uargc = Uargc;
		this->Uargv = Uargv;
		this->povratnaVrednost = -2;
	}

	int Uargc;
	char** Uargv;
	int povratnaVrednost;
	void run(){

		povratnaVrednost = userMain(Uargc,Uargv);

	}

	~UserThread(){waitToComplete();}


};

void inic();

int main(int argc, char* argv[]){

	napraviMainThread();

	inic();

	UserThread* userT = new UserThread(argc,argv);
	userT->start();

	lock
	int povratnaMain = userT->povratnaVrednost;
	unlock

	delete userT;

	restore();


	return povratnaMain;
}

void restore(){


		asm {
			cli
			push es
			push ax

			mov ax,0
			mov es,ax


			mov ax, word ptr oldTimerSEG
			mov word ptr es:0022h, ax
			mov ax, word ptr oldTimerOFF
			mov word ptr es:0020h, ax

			pop ax
			pop es
			sti
		}
		delete lista;
		delete listaSemafora;


}
void inic(){

	asm{
		cli
		push es
		push ax

		mov ax,0   //  ; inicijalizuje rutinu za tajmer
		mov es,ax

		mov ax, word ptr es:0022h //; pamti staru rutinu, racuna se u osnovi 16
		mov word ptr oldTimerSEG, ax
		mov ax, word ptr es:0020h
		mov word ptr oldTimerOFF, ax

		mov word ptr es:0022h, seg timer	 //postavlja
		mov word ptr es:0020h, offset timer //novu rutinu

		mov ax, oldTimerSEG	 //	postavlja staru rutinu
		mov word ptr es:0182h, ax //; na int 60h
		mov ax, oldTimerOFF
		mov word ptr es:0180h, ax

		pop ax
		pop es
		sti
	}
	lock
	lista = new ListThr();
	listaSemafora = new ListSem();
	unlock


}



