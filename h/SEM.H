#ifndef _semaphor_h_
#define _semaphor_h_


typedef unsigned int Time;

class KernelSem;
class PCB;
class ListPCB;


class Semaphore {

public:

	Semaphore (int init=1);
	ListPCB* listNoTime;
	ListPCB* listTime;
 	 virtual ~Semaphore ();
 	 virtual int wait (Time maxTimeToWait);
 	 virtual void signal();
 	 int val () const; // Returns the current value of the semaphore
private:
 	 KernelSem* myImpl;
};
#endif
