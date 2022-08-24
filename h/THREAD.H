#ifndef _THREAD_H_
#define _THREAD_H_



typedef unsigned long StackSize;
const StackSize defaultStackSize = 4096;
typedef unsigned int Time; // time, x 55ms
const Time defaultTimeSlice = 2; // default = 2*55ms
typedef int ID;

class PCB; // Kernel's implementation of a user's thread
class ListThr;
class ElemThr;
class Idle;

extern ListThr* lista;

class Thread {
public:

	ListThr* mojeBlokirane;
	int static globalni_ID;
	int privatni_ID;

	void start();
	void waitToComplete();
	virtual ~Thread();
	ID getId();
	static ID getRunningId();
	static Thread * getThreadById(ID id);

protected:
	friend class PCB;
	friend class ListThr;
	Thread (StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice);
	virtual void run() {}
	PCB* myPCB;
private:

};
void dispatch ();


#endif /* THREAD_H_ */
