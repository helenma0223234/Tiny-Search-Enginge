# Makefile for CS50 TSE /querier                                                    
#                                                                                     
# Lab 6                                                                               
# Yuchuan Ma May 17, 2021                                                           

L = ../libcs50
L2 = ../common
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$L -I$(L2)
CC = gcc
OBJS = querier.o readlinep.o
OBJS1 = querytest.o
LLIBS = $(L2)/common.a $L/libcs50-given.a 

MAKE = make

# for memory-leak tests
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

all: querier querytest

querier: $(OBJS) $(LLIBS)
	$(CC) $(CFLAGS) $(OBJS) $(LLIBS) -o querier

querytest:$(OBJS1) $(LLIBS)
	$(CC) $(CFLAGS) $(OBJS1) $(LLIBS) -o querytest

querier.o: $L/mem.h $(L2)/index.h $L/counters.h $(L2)/query.h $(L2)/pagedir.h $L/file.h $L/webpage.h readlinep.h
readlinep.o: readlinep.h
querytest.o: $L/counters.h $(L2)/query.h

.PHONY: test valgrind clean

test: querytest
	./querytest &> querytest.out

test: querier
	bash -v testing.sh &> testing.out

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f querier
	rm -f querytest
	rm -f core
