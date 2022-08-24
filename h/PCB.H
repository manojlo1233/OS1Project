#ifndef _PCB_H_
#define _PCB_H_

#include "dos.h"
#include "SCHEDULE.H"
#include "THREAD.H"



class PCB{

public:
	unsigned* st;
	unsigned sp;
	unsigned ss;
	unsigned bp;
	unsigned prviBP;
	int zavrsio;
	int zaludna;
	int zablokirana;
	int startovana;
	int vremeNaSemaforu;
	int kvant;
	unsigned long velicinaSteka;

	static void postaviRunning(Thread* t);

	static PCB* running;
	Thread* myThr;

	PCB(unsigned long stackSize, unsigned int timeSlice, Thread* thr, void (*body)());
	static void wrapper();

	~PCB();
};

#endif



