#!/bin/bash

# Testing script for indexer
# Yuchuan Ma, May 10, 2021
# CS50 Lab 5

# test with various invalid input
# testing indexer with no argument
./indexer
# testing indexer with one argument
./indexer ~/cs50-dev/shared/tse/output/letters-10
# testing indexer with four argument
./indexer ~/cs50-dev/shared/tse/output/letters-10 errorArg otherErrorArg
# testing indexer with non-existent path
./indexer ~/cs50-dev/shared/tse/output/hello ../data/letters-10.index
# testing indexer with invalid crawler-produced directory
./indexer ~/cs50-dev/shared/tse/output/letters-10 ~/cs50-dev.index
# testing indexer with invalid indexFile
./indexer ~/cs50-dev/shared/tse/output/letters-0 ~/cs50-dev/data/cantopen.out
# with read-only directory
./indexer ~/cs50-dev/shared/tse/output/letters-0 ../data/letters-0
# with read-only file
./indexer ~/cs50-dev/shared/tse/output/letters-0 ../data/testdir/readOnlyFile.index

# correct input tests
# indexer on letters-1
./indexer ~/cs50-dev/shared/tse/output/letters-1 ../data/letters-1.index
# run indextest on letters-1
./indextest ../data/letters-1.index ../data/letters-1.index.out
~/cs50-dev/shared/tse/indexcmp ../data/letters-1.index ../data/letters-1.index.out
# valgrind with indexer on letters-1
valgrind ./indexer ~/cs50-dev/shared/tse/output/letters-1 ../data/letters-1.index
# valgrind with indextest on letters-1
valgrind ./indextest ../data/letters-1.index ../data/letters-1.index.out

# run indexer on letters-2
./indexer ~/cs50-dev/shared/tse/output/letters-2 ../data/letters-2.index
# run indextest on letters-2
./indextest ../data/letters-2.index ../data/letters-2.index.out
~/cs50-dev/shared/tse/indexcmp ../data/letters-2.index ../data/letters-2.index.out
# run valgrind with indexer on letters-2
valgrind ./indexer ~/cs50-dev/shared/tse/output/letters-2 ../data/letters-2.index
# run valgrind with indextest on letters-2
valgrind ./indextest ../data/letters-2.index ../data/letters-2.index.out

# run indexer on toscrape-0
./indexer ~/cs50-dev/shared/tse/output/toscrape-0 ../data/toscrape-0.index
# run indextest on toscrape-0
./indextest ../data/toscrape-0.index ../data/toscrape-0.index.out
~/cs50-dev/shared/tse/indexcmp ../data/toscrape-0.index ../data/toscrape-0.index.out


