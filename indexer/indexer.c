/* 
 * indexer.c - CS50 TSE sub-system Indexer
 *
 * Usage: only interface with the user is on the command-line
 *        it takes two arguments: $ indexer pageDirectory indexFilename.
 *
 * Input: no file input.
 * Output: We save the index to a file using the format described in the Requirements.
 *
 * Data Structures: struct index, struct counters and struct webpage
 *
 * See Lab5 DESIGN.md, IMPLEMENTATION.md in the same folder specs for more details.
 *
 * Yuchuan Ma, May 6, 2021
 * Lab5 CS50 Spring 2021 Dartmouth
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "mem.h"
#include "index.h"
#include "webpage.h"
#include "pagedir.h"
#include "hashtable.h"
#include "word.h"


/* *********************** private functions prototype ************************** */
int main(const int argc, char* argv[]);
static void parseArgs(const int argc, char* argv[],
                      char** pageDirectory, char** indexFilename);
static void indexBuild(char* pageDirectory, char* indexFilename);
static void indexPage(index_t* indexer, webpage_t* page, const int docID);


/* ************************* main **************************** */
int
main(const int argc, char* argv[])
{
    // declare and initialize variables for each of the expected arguments
    char* pageDirectory = NULL;    // input crawler-produced directory
    char* indexFilename = NULL;  // input index file destination
    // process arguments
    parseArgs(argc, argv, &pageDirectory, &indexFilename);
    // build the index
    indexBuild(pageDirectory, indexFilename);

    return 0;
}

/* ******************* parseArgs ******************* */
/* Handle the messy business of parsing command-line arguments, so main() is cleaner and clearer.
 * Extract  destination directory and index file destination path from input arguments
 *
 * We return 'void' when successful;
 * Otherwise, print error or usage message to stderr and exit non-zero.
 * Note: following codes are sourced and adapted from CS50 examples/parseArgs.c
 */
static void
parseArgs(const int argc, char* argv[], char** pageDirectory, char** indexFilename)
{
    const char* progName = argv[0]; // name of this program

    if (argc != 3) {
        fprintf(stderr, "usage: %s pageDirectory indexFilename\n", progName);
        exit(1);
    }

    // save arguments to pointers,and  make sure they are valid input
    *pageDirectory = argv[1];
    if (pagedir_validate(*pageDirectory) == false){
        fprintf(stderr, "%s: '%s' invalid directory\n", progName, *pageDirectory);
        exit(2);
    }

    *indexFilename = argv[2];
    FILE* fp;
    if ((fp=fopen(*indexFilename, "w")) == NULL) {    // if cannot write in the file
        fprintf(stderr, "%s: '%s' invalid indexer destination\n", progName, *indexFilename);
        exit(3);
    } else {
        fclose(fp);
    }
}

/* ******************* indexBuild ******************* */
/* Do the real work of indexing and saving the mapping of word-to-document 
 * to the `indexFilename`.
 *
 * The function is responsible to make memory for the data structures and also
 * later freeing them after indexing.
 * The indexer builds from webpage files it finds in the pageDirectory by first looping 
 * through docIDs, fetches corresponding webpage, then parsing it to the indexPage().
 *
 * We return void when sucessful; otherwise, Print out error and return;
 */
static void 
indexBuild(char* pageDirectory, char* indexFilename)
{
    if (pageDirectory == NULL || indexFilename == NULL) {
        fprintf(stderr, "invalid input for indexBuild\n");
        return;
    }
    // initialize a new index
    index_t* idxer = index_new();
    if (idxer == NULL) {
        fprintf(stderr, "failed to create index_t\n");
        return;
    }
    // loop through all page file in the directory
    int docID = 1;
    webpage_t* page = pagedir_fetch(pageDirectory, docID);


    while (page != NULL) {
        indexPage(idxer, page, docID);
        docID++;
        webpage_delete(page);
        page = pagedir_fetch(pageDirectory, docID);
    }
    webpage_delete(page);

    // print out the index into indexFilename
    index_write(idxer, indexFilename);
    // delete the index_t
    index_delete(idxer, NULL);
}


/* ******************* indexPage ******************* */
/* scans a webpage document to add its words to the index
 *
 * The function steps through each word of the webpage HTML,
 * adds/increments  words that are normalized and 3-character longer into the index.
 *
 * We return void when sucessful; otherwise, Print out error and return;
 */
static void 
indexPage(index_t* indexer, webpage_t* page, const int docID) {

    //if (webpage_fetch(page)) {  // retrieve a HTML for the that URL
    int pos = 0;
    char* result;
    
    // loop through each word in the html
    while ((result = webpage_getNextWord(page, &pos)) != NULL) {

        if (strlen(result) < 3) {
            free(result);
        } else {

        // normalize the word
        char* result1 = normalizeWord(result);
        free(result);

        if (result1 == NULL) {
            fprintf(stderr, "error in normalizeWord/n");
            return;
        }

        // insert the word into indexer
        if (index_insert(indexer, result1, docID)){
            free(result1);
        } else {
            free(result1);
            return;
        }
        }
    }


}


