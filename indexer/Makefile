# Makefile for CS50 TSE /indexer                                                    
#                                                                                     
# Lab 4                                                                               
# Yuchuan Ma May 9, 2021                                                           

L = ../libcs50
L2 = ../common
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$L -I$(L2)
CC = gcc
OBJS = indexer.o
OBJS1 = indextest.o
LLIBS = $(L2)/common.a $L/libcs50-given.a 

MAKE = make

# for memory-leak tests                                                               
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

all: indexer indextest

indexer: $(OBJS) $(LLIBS)
	$(CC) $(CFLAGS) $(OBJS) $(LLIBS) -o indexer
indextest:$(OBJS1) $(LLIBS)
	$(CC) $(CFLAGS) $(OBJS1) $(LLIBS) -o indextest

indexer.o: $L/mem.h $L/webpage.h $(L2)/pagedir.h $(L2)/index.h $(L2)/word.h $(L)/hashtable.h 
indextest.o:  $L/file.h $(L2)/index.h

.PHONY: test valgrind clean

test: indexer
	bash -v testing.sh > testing.out                                                           

valgrind: indexer
	$(VALGRIND) ./indexer ../data/letters-10 ./indexl10.out

valgrind: indextest
	$(VALGRIND) ./indextest ./indexl10.out ./indextestl10.out

clean:
	rm -rf *.dSYM  # MacOS debugger info                                          
	rm -f *~ *.o
	rm -f indexer
	rm -f indextest
	rm -f core
