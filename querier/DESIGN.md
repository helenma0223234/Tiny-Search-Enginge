# CS50 TSE Querier
## Design Spec
In this document we reference the [Requirements Specification](REQUIREMENTS.md) and focus on the implementation-independent design decisions.
Here we focus on the core subset:

- User interface
- Inputs and outputs
- Functional decomposition into modules/functions
- Pseudo code (plain English-like language) for logic/algorithmic flow
- Major data structures
- Testing plan

## User interface
As described in the [Requirements Spec](REQUIREMENTS.md), the querier is initiated with a command-line, then it takes user-input querier strings (which are words to query) until EOF; it must always execute usage syntax with two inputs.
```bash
$ ./querier pageDirectory indexFilename
```
## Inputs and outputs

*Input:* a page directory input and an index file inputs; then continued keyboard input from user until EOF

*Output:* ranked documents with scores that satisfy the query search input by the user in stdout

## Functional decomposition into modules

We anticipate the following functions:

 1. *main*, which parses arguments and initializes other modules
 2. *parseArgs*, which parse in the input command line, validate the input, then parse out two variables: pageDirectory and indexFilename
 3. *fileno*, which prints a prompt for an interactive user
 4. *querier*,  which parses the query string word by word and initiates functions to find merge/intersect page indexes
 5. *normalQuery*, which normalizes a string query, delete space and lowercase the words .etc
 6. *buildDocs*, which ranks a query counters bag according to match score (see requirement spec for how) and insert them accordingly to an array of document structure
 7. *build_docs_helper*, which is a helper funct for buildDocs
 8. *printDocs*, which prints the array of documents in formats specified in requirement spec, as the
 result of the query
 9. *parseQuery*, which queries through the index and return query result

Other helpful module:
*query*: contains function that merges/intersects two counters object

## Pseudo code for logic/algorithmic flow

The querier will run as follows:

parse arguments and validate input;
Initiate the index structure and a query counters bag;
Read query lines from stdin line by line, input by the user;
Run the querier on the string;
While (there’s still words to query):
	Read word by word
Parse the sequence words (and/or)
merge/intersect querier bag of counters accordingly
	Update the query counters bag after reading each line
Rank the docIDs in the final query counters bag
Print the rank with doc info to stdin

## Major data structures and modules

- *counters*
- *index*
- *query* 
- *readlinep*

### counters
See libcs50/counters.h for details
### index
See common/index.h for details
### query
See common/query.h for details
The query module is similar to a supplementary package to the counters structure; it contains specific functions that merge two counters and find intersection of two counters 
### readlinep
See common/readlinep.h for details
To read input line by line, adopted from CS50 Course example folder

## Testing plan

*Unit testing*. A program *queryTest* will serve as a unit test for the *query* module, it merges or intersects the two counters and prints them out.
*Integration testing*.  The *querier*, as a complete program, will be tested by building an index from an indexFile, and then querying through the index with user-input query requests.

1. Test `querier` with various invalid arguments.
	2. no arguments
	3. one argument
	4. three or more arguments
	5. invalid `pageDirectory` (non-existent path)
	5. invalid `pageDirectory` (not a crawler directory)
	6. invalid `indexFile` (non-existent path)
	7. invalid `indexFile` (read-only directory)
	7. invalid `indexFile` (existing, read-only file)
0. Run *querier* with the fuzzy test method: *fuzzquery.c* is provided in the course lecture, we will use the file to generate 10 random query strings and test with them. 
0. Run *valgrind* on *querier* with some given querier strings.
