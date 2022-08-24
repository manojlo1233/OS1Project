#ifndef _LISTPCB_H_
#define _LISTPCB_H_

class PCB;
class ElemPCB;

class ListPCB{

public:
	ListPCB(){
		head = 0;
		tail = 0;
	}


	ElemPCB* head;
	ElemPCB* tail;

	void dodaj(PCB* t);
	PCB* izbaci();


};




#endif
