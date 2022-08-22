# CS50 TSE
## Customized common.a library

This folder contains common modules that are shared/used by sub programs of the TSE we build.

## Modules

### pagedir

We create a re-usable module `pagedir.c` to handles the *pagesaver*  mentioned in the design (writing a page to the pageDirectory), and marking it as a Crawler-produced pageDirectory (as required in the spec).
We chose to write this as a separate module, in `../common`, to encapsulate all the knowledge about how to initialize and validate a pageDirectory, and how to write and read page files, in one place... anticipating future use by the Indexer and Querier.

Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's declaration in `pagedir.h` and is not repeated here.

## index

This is a module that implements an abstract `index_t` type that represents an index in memory, and supports functions like `index_new()`, `index_delete()`, `index_save()`, and so forth.

## word

This is a module that implements normalization of a word, turning it to all lower case.
