#ifndef _KSEM_H_
#define _KSEM_H_
#include "SEM.H"
#include "LISTPCB.H"
#include "SCHEDULE.H"


class KernelSem{
public:

	KernelSem(int value,Semaphore* pointer);
	int val;
	Semaphore* mySem;
	int wait (Time maxTimeToWait);
	void signal();

	friend class Semaphore;
	~KernelSem();
};


#endif
