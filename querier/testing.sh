#!/bin/bash

# Testing script for querier
# Yuchuan Ma, May 9, 2021
# CS50 Lab 5

# test with various invalid input
# testing indexer with no argument
./querier
# testing indexer with one argument
./querier ~/cs50-dev/shared/tse/output/letters-10
# testing indexer with four argument
./quereir ~/cs50-dev/shared/tse/output/letters-10 errorArg otherErrorArg
# testing indexer with non-existent path
./querier ~/cs50-dev/shared/tse/output/hello ../data/letters-10.index
# testing indexer with invalid crawler-produced directory
./querier ../crawler  ~/cs50-dev.index
# testing indexer with invalid indexFile
./querier ~/cs50-dev/shared/tse/output/letters-0 ~/cs50-dev/data/cantopen.out

# correct command line input tests
# testing incorrect query and simple querying with valgrind
cd ..
seed1="http://cs50tse.cs.dartmouth.edu/tse/letters/index.html"
pdir1="data/letters-2"
indx1="data/letters-2.index"
#mkdir $pdir1
#crawler/crawler $seed1 $pdir1 2
#indx1="data/letters-2.index"
# run query on letters-2
valgrind --leak-check=full --show-leak-kinds=all querier/querier $pdir1 $indx1 < querier/testingquery.in


# testing querying
pdir="data/toscrape-0"
indx="data/toscrape-0.index"
querier/querier $pdir $indx < querier/testingquery1.in
