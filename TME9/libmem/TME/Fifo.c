#include "Fifo.h"

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int initFifo(Swapper*);
unsigned int fifoChoose(Swapper*);
void fifoReference(Swapper*,unsigned int frame);
void finalizeFifo(Swapper*);

int initFifoSwapper(Swapper*swap,unsigned int frames){
	return	initSwapper(swap,frames,initFifo,NULL,fifoChoose,finalizeFifo);
}

int initFifo(Swapper*swap){
	/* A ecrire en TME */
	return 0;
}

unsigned int fifoChoose(Swapper*swap){
	/* A ecrire en TME */
}

void finalizeFifo(Swapper*swap){
	/* A ecrire en TME */
}
