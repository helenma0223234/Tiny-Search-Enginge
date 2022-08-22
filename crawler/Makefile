# Makefile for CS50 TSE /crawler                                                      
#                                                                                     
# Lab 4                                                                               
# Yuchuan Ma April 29, 2021                                                           

L = ../libcs50
L2 = ../common
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$L -I$(L2)
CC = gcc
OBJS = crawler.o
LLIBS = $L/libcs50-given.a $(L2)/common.a

MAKE = make

# for memory-leak tests                                                               
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

all: crawler

crawler: $(OBJS) $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LLIBS) -o $@

#../common/pagedir.o: ../common/pagedir.h $L/webpage.h
crawler.o: $L/mem.h $L/bag.h $L/hashtable.h $L/webpage.h $(L2)/pagedir.h


.PHONY: test valgrind clean

test: crawler
	bash -v testing.sh                                                            

valgrind: crawler
	$(VALGRIND) ./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters-1 2                                                    

clean:
	rm -rf *.dSYM  # MacOS debugger info                                          
	rm -f *~ *.o
	rm -f crawler
	rm -f core
