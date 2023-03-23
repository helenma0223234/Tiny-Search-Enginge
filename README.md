# Tiny Search Engine
## CS50 Lab

The assignment and Specs are in a [public repo](https://github.com/cs50spring2021/tse-labs).
The program is composed by three subsystems: crawler, indexer, querier, which in combination functions like a miny search engine. The crawler crawls every link on a given webpage.

The indexer is a standalone program that reads the document files produced by the TSE crawler, builds an index, and writes that index to a file. 

The querier takes in search words, spill out a list of websites with relativity scores. More specificall, you read one query per line, parse the query according to its syntax, determine which documents satisfy the query, and determine score for each document satisfying the query.

## How to use
Go into each sub directory and do a make command to compile all the code, then you are able to use that particular subsystem.

### crawler
the crawler's only interface with the user is on the command-line; it must always have three arguments.

```bash
$ crawler seedURL pageDirectory maxDepth
```

For example, to crawl a website http://dali.dartmouth.edu, store the pages found in a subdirectory `data` in the current directory, and to search only depths 0, 1, and 2, use this command line:

``` bash
$ mkdir ../data/letters
$ ./crawler http://dali.dartmouth.edu ../data/letters 2
```

### indexer

The indexer's only interface with the user is on the command-line; it must always have two arguments.

```
indexer pageDirectory indexFilename
```

For example, if `letters` is a pageDirectory in `../data`,

``` bash
$ indexer ../data/letters ../data/letters.index
```
**Output**: the program saves the index to a file using the format described in the Requirements.

### querier

The querier **shall**:

1. execute from a command line with usage syntax
   * `./querier pageDirectory indexFilename`
   * where `pageDirectory` is the pathname of a directory produced by the Crawler, and
   * where `indexFilename` is the pathname of a file produced by the Indexer.

1. validate it received exactly two command-line arguments and that 
	* `pageDirectory` is the pathname for a directory produced by the Crawler, and
	* `indexFilename` is the pathname of a file that can be read;

1. load the index from `indexFilename` into an internal data structure.

1. read search queries from stdin, one per line, until EOF.

	2. clean and parse each query according to the *syntax* described below.
	2. if the query syntax is somehow invalid, print an error message, do not perform the query, and prompt for the next query.
	2. print the 'clean' query for user to see.
	2. use the index to identify the set of documents that *satisfy* the query, as described below.
	2. if the query is empty (no words), print nothing.
	2. if no documents satisfy the query, print `No documents match.`
	2. otherwise, rank the resulting set of documents according to its *score*, as described below, and print the set of documents in decreasing rank order; for each, list the score, document ID and URL.
(Obtain the URL by reading the first line of the relevant document file from the `pageDirectory`.)

1. output nothing to stdout other than what is indicated above

1. exit zero when EOF is reached on stdin; exit with an error message to stderr and non-zero exit status on encountering an unrecoverable error, including
	* out of memory,
	* invalid command-line arguments,
	* unable to read a file named `pageDirectory/.crawler`,
	* unable to read a file named `pageDirectory/1`
	* unable to read a file named `indexFilename`
  
Note: the above are sourced from design/implementation specs from lab material provided by the class.
