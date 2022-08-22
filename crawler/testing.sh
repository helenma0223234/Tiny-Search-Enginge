#!/bin/sh

# testing script for crawler.c
# This test invokes the crawler several times, with a variety of command-line arguments. 
#
# First, a sequence of invocations with erroneous arguments.
# Second, runs over all three CS50 websites (letters at depths 0,1,2,10, toscrape at 
# depths 0,1,2,3, wikipedia at depths 0,1,2).
# Third, a run with valgrind over a moderate-sized website.
#
# Yuchuan Ma, May 1, 2021
# CS50 TSE Lab4, Spring 2021 Dartmouth

# make a test directory first
mkdir ../data/letters-0

# test with more than 3 arguments
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters-0 1 extra argument
# test with invalid seedURL
./crawler http://cs50tse.cs.dartmouth.edu/tse/index.html ../data/letters-0 1
# test with external URL
./crawler https://en.wikipedia.org/wiki/Algorithm ../data/letters-0 1
# test with invalid directory
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters-100 1
# test with wrong maxDepth
#   excessive letters
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters-0 1a
#   too big or less than 0
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters-0 20
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters-0 -20

# valgrind test on toscrape/index.html depth 1
mkdir ../data/toscrape-1
valgrind --leak-check=full --show-leak-kinds=all ./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/index.html ../data/toscrape-1 1

# test on letters/index.html depth 0,1,210
mkdir ../data/letters-0
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters-0 0
mkdir ../data/letters-1
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters-1 1
mkdir ../data/letters-2
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters-2 2
mkdir ../data/letters-10
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters-10 10

# test on toscrape/index.html depth 0,2
mkdir ../data/toscrape-0
./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/index.html ../data/toscrape-0 0
mkdir ../data/toscrape-1
./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/index.html ../data/toscrape-1 1

# test on wikipedia/index.html depth 0,1
mkdir ../data/wikipedia-0
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/index.html ../data/wikipedia-0 0
mkdir ../data/wikipedia-1
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/index.html ../data/wikipedia-1 1

