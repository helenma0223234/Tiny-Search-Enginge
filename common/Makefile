# Makefile for CS50 TSE /common
#
# Lab 4
# Yuchuan Ma April 29, 2021

L = ../libcs50

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$L $(FLAGS)
CC = gcc
OBJS = pagedir.o word.o index.o query.o
LLIBS = $L/libcs50-given.a
LIB = common.a

MAKE = make

# for memory-leak tests
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all


$(LIB): $(OBJS) $(LLIBS)
	ar cr $(LIB) $(OBJS)

pagedir.o: pagedir.h $L/webpage.h $L/file.h $L/mem.h
index.o: index.h $L/hashtable.h $L/mem.h $L/counters.h $L/file.h
word.o: word.h
query.o: query.h $L/mem.h $L/counters.h

.PHONY: test sourcelist


clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f $(LIB) *~ *.o
	rm -f pagedir
	rm -f core
