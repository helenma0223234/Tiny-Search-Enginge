# CS50 TSE Indexer
## Implementation Spec

In this document we reference the [Requirements Specification](REQUIREMENTS.md) and [Design Specification](DESIGN.md) and focus on the implementation-specific decisions.

Here we focus on the core subset:

-  Data structures
-  Control flow: pseudo code for overall flow, and for each of the functions
-  Detailed function prototypes and their parameters
-  Error handling and recovery
-  Testing plan

## Data structures 

We use one data structures: the 'index', specifically created to implement the indexer; 
The *index* is a *hashtable* keyed by *word* and storing *counters* as items.
The *counters* is keyed by *docID* and stores a count of the number of occurrences of that word in the document with that ID. 

## Control flow
The Indexer is implemented in one file 'indexer.c', with three functions.

### main

The `main` function simply calls `parseArgs` and `indexBuild`, then exits zero.

### parseArgs

Given arguments from the command line, extract them into the function parameters; return only if successful.

* for `pageDirectory`, call `pagedir_validate()` to verify whether pageDirectory is indeed a Crawler-produced directory
* for `indexFilename`, make sure it is valid path and the file can be written
* if any trouble is found, print an error to stderr and exit non-zero.

### indexBuild

Do the real work of indexing from `pageDirectory` and saving the mapping of word-to-document to the `indexFilename`.
Pseudocode:
      creates a new 'index' object
      loops over document ID numbers, counting from 1
        loads a webpage from the document file 'pageDirectory/id'
        if successful, 
          passes the webpage and docID to indexPage

### indexPage
This function implements the *indexPage* mentioned in the design.
Pseudocode:
     steps through each word of the webpage,
       skips trivial words (less than length 3),
       normalizes the word (converts to lower case),
       looks up the word in the index,
         adding the word to the index if needed
       increments the count of occurrences of this word in this docID

## Other modules

### index
The index is a hashtable keyed by word and storing counters as items. The counters is keyed by docID and stores a count of the number of occurrences of that word in the document with that ID. The model is like a wrapper class for hashtable. 
See index.h for more details, the funtion heads and how the module works.

### libcs50
We leverage the modules of libcs50, most notably `bag`, `hashtable`, and `webpage`.
See that directory for module interfaces.i
The `webpage` module allows us to represent pages as `webpage_t` objects, to fetch a page from the Internet, and to scan a (fetched) page for URLs; in that regard, it serves as the *pagefetcher* described in the design. This module is used by the module pagedir to read a crawler-produced webpage file.

### pagedir

We continue to expand and use the moduole `pagedir.c` in the `../common`, created for the Crawler earlier.

Pseudocode for `pagedir_validate`:
	open the file .crawler in that directory; on error, return false
	close the file and return true.

Pseudocode for `pagedir_load`:
    open the page file named with corresponding docID in the directory
    create webpage_t by loading the headers of the page file
    return the pointer to the webpage_t
    

### word

A module in the `../common` that provides a function to normalize a word.

Pseudocode for `normalizeWord`:
    loop through the string (input word)
      convert each character to lowercase

## Function prototypes

### index
Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's implementation in `index.h` 
```c
index_t* index_new();
bool index_insert(index_t* idx, const char* key, const int docID);
void* index_find(index_t* idx, const char* key);
void index_iterate(index_t* idx, void* arg, void (*itemfunc)(void* arg, const char* key, void* item) );
void index_set(index_t* idx, const char* key, const int docID, const int count);
bool index_write(index_t* idx, const char* indexFile);
void index_delete(index_t* idx, void (*itemdelete)(void* item) );
```

### indexer

Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's implementation in `indexer.c` and is not repeated here.

```c
int main(const int argc, char* argv[]);
static void parseArgs(const int argc, char* argv[],
                      char** pageDirectory, char** indexFilename);
static void indexBuild(char** pageDirectory, char** indexFilename);
static void indexPage(index_t* indexer, webpage_t* page, const int docID);
```

### pagedir

Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's declaration in `pagedir.h` and is not repeated here.

```c
bool pagedir_validate(const char* pageDirectory);
webpage_t* pagedir_load(const char* pageDirectory, const int docID);
```

### word
Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's declaration in `word.h` and is not repeated here.
```c
char* normalizeWord(const char* word);
```


## Error handling and recovery

All the command-line parameters are rigorously checked before any data structures are allocated or work begins; problems result in a message printed to stderr and a non-zero exit status.

Out-of-memory errors are handled by variants of the `mem_assert` functions, which result in a message printed to stderr and a non-zero exit status.
We anticipate out-of-memory errors to be rare and thus allow the program to crash (cleanly) in this way.

All code uses defensive-programming tactics to catch and exit (using variants of the `mem_assert` functions), e.g., if a function receives bad parameters.

That said, certain errors are caught and handled internally: for example, `pagedir_init` returns false if there is any trouble creating the `.crawler` file, allowing the Crawler to decide what to do; the `webpage` module returns false when URLs are not retrievable, and the Crawler does not treat that as a fatal error.

### Testing plan

*Unit testing*.  A program *indextest* will serve as a unit test for the *index* module; it reads an index file into the internal *index* data structure, then writes the index out to a new index file.

*Integration testing*.  The *indexer*, as a complete program, will be tested by building an index from a pageDirectory, and then the resulting index will be validated by running it through the *indextest* to ensure it can be loaded.

1. Test `indexer` with various invalid arguments.
	2. no arguments
	3. one argument
	4. three or more arguments
	5. invalid `pageDirectory` (non-existent path)
	5. invalid `pageDirectory` (not a crawler directory)
	6. invalid `indexFile` (non-existent path)
	7. invalid `indexFile` (read-only directory)
	7. invalid `indexFile` (existing, read-only file)
0. Run *indexer* on a variety of pageDirectories and use *indextest* as one means of validating the resulting index.
0. Run *valgrind* on both *indexer* and *indextest* to ensure no memory leaks or errors.
