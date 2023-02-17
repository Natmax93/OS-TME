# Mettre dans SCHEDPATH le repertoire ou se trouve libsched.a
# et sched.h
SCHEDPATH = /Infos/lmd/2022/licence/ue/LU3IN010-2023fev/libsched/src

CFLAGS = -Wall -I$(SCHEDPATH) -g
CC = /usr/bin/gcc

all: main scen 

main: main.o 
	$(CC) $(CFLAGS) -o main main.o -L$(SCHEDPATH) -lsched -L$(SCHEDPATH) -lelf 


scen: scen.o 
	$(CC) $(CFLAGS) -o scen scen.o -L$(SCHEDPATH) -lsched -L$(SCHEDPATH) -lelf 

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

scen.o: scen.c
	$(CC) $(CFLAGS) -c scen.c

clean: 
	rm -f main main.o scen scen.o
