#ifndef _lock_h_
#define _lock_h_

extern volatile int zahtevana_promena_konteksta;
extern void dispatch();
extern volatile int blockFlag;
#define lock blockFlag=1;

#define unlock blockFlag=0;\
 if (zahtevana_promena_konteksta) {\
 dispatch();\
}



#endif
