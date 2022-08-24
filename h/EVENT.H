#ifndef _event_h_
#define _event_h_

typedef unsigned char IVTNo;

class KernelEv;
class PCB;

class Event {

public:

	 Event (IVTNo ivtNo);
	 ~Event ();
	 void wait ();
	 PCB* myPCB;


protected:

	 friend class KernelEv;
	 void signal(); // can call KernelEv

private:

	 KernelEv* myImpl;
};
#endif
