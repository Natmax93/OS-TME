CFLAGS = -Wall  -fPIC -I. -g
all: libsched.a 
	
libsched.a: sched.o nlist2.o
	ar r libsched.a sched.o nlist2.o
	ranlib libsched.a
	
sched.o: sched.c sched.h
	gcc $(CFLAGS) -c sched.c

nlist2.o: nlist2.c
	gcc $(CFLAGS) -c nlist2.c 
	
clean: 
	rm -f libsched.a sched.o nlist2.o
