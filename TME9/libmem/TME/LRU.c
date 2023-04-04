#include "LRU.h"

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int	initLRU(Swapper*);
void	referenceLRU(Swapper*,unsigned int frame);
unsigned int chooseLRU(Swapper*);
void	finalizeLRU(Swapper*);

typedef struct {
	unsigned int clock;
	unsigned int * age;
} InfoLRU;

int initLRUSwapper(Swapper*swap,unsigned int frames){
 return	initSwapper(swap,frames,initLRU,referenceLRU,chooseLRU,finalizeLRU);
}

int	initLRU(Swapper*swap){
	/* A ecrire en TME */
	return 0;
}

void	referenceLRU(Swapper*swap,unsigned int frame){
	/* A ecrire en TME */
}

unsigned int chooseLRU(Swapper*swap){
	/* A ecrire en TME */
	
}

void	finalizeLRU(Swapper*swap){
	/* A ecrire en TME */

}
