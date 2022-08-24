#ifndef _IVTE_H_
#define _IVTE_H_

#define PREPAREENTRY(numEntry, callOld)\
void interrupt inter##numEntry(...); \
IVTEntry newEntry##numEntry(numEntry, inter##numEntry);\
void interrupt inter##numEntry(...) {\
newEntry##numEntry.signal();\
if (callOld == 1) newEntry##numEntry.staraRutina();\
dispatch();\
}

class KernelEv;
class IVTEntry{

public:
	IVTEntry();
	IVTEntry(unsigned char ivtNo, void interrupt (*nova)(...));
	void interrupt (*staraRutina)(...);
	unsigned int brojUlaza;
	static IVTEntry** IVTniz;
	~IVTEntry();
	void signal();
	KernelEv* myKernelEv;
	friend class KernelEv;

};

#endif
