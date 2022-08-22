/*
 * indextest.c - testing the output of indexer
 *
 * usage:
 * ./indextest oldIndexFilename newIndexFilename
 *
 * what it does:
 * the indextest load the index from the oldIndexFilename into an inverted-index data
 * structure, then creates a newIndexFilename and write the index to that file.
 *
 * Yuchuan Ma, May 9, 2021
 * Lab5 CS50 Spring 2021, Dartmouth
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "index.h"
#include "file.h"


/* *********************** private functions prototype ************************** */
int main(const int argc, char* argv[]);
static void parseArgs(const int argc, char* argv[], char** oldIndexFilename, char** newIndexFilename);
static void indexRead(char* oldIndexFilename, char* newIndexFilename);

/* ************************* main **************************** */
int
main(const int argc, char* argv[])
{
    // declare and initialize variables for each of the expected arguments
    char* oldIndexFilename = NULL;    // input crawler-produced directory
    char* newIndexFilename = NULL;  // input index file destination
    // process arguments
    parseArgs(argc, argv, &oldIndexFilename, &newIndexFilename);
    // build the index
    indexRead(oldIndexFilename, newIndexFilename);

    return 0;
}

/* ******************* parseArgs ******************* */
/* Handle the messy business of parsing command-line arguments, so main() is cleaner and clearer.
 * Extract  old and new index file  path from input arguments
 *
 * We return 'void' when successful;
 * Otherwise, print error or usage message to stderr and exit non-zero.
 * Note: following codes are sourced and adapted from CS50 examples/parseArgs.c
 */
static void
parseArgs(const int argc, char* argv[], char** oldIndexFilename, char** newIndexFilename)
{
    const char* progName = argv[0]; // name of this program

    if (argc != 3) {
        fprintf(stderr, "usage: %s oldIndexFilename newIndexFilenamen", progName);
        exit(1);
    }

    // save arguments to pointers,and  make sure they are valid input
    FILE* fp;
    *oldIndexFilename = argv[1];
    if ((fp=fopen(*oldIndexFilename, "r")) == NULL){
        fprintf(stderr, "%s: '%s' invalid oldIndexFile\n", progName, *oldIndexFilename);
        exit(2);
    } else {
        fclose(fp);
    }

    *newIndexFilename = argv[2];
    if ((fp=fopen(*newIndexFilename, "w")) == NULL) {    // if cannot write in the file
        fprintf(stderr, "%s: '%s' invalid newIndexFile\n", progName, *newIndexFilename);
        exit(3);
    } else {
        fclose(fp);
    }
}

/* ******************* indexBuild ******************* */
/* Read the oldIndexFile into a index_t then print it out in the same format.
 *
 * Note: the order of the new index file will be different because the internal
 * data structure of index_t are not ordered (hashtable and counters are both
 * not ordered)
 *
 * We return void when sucessful; otherwise, Print out error and return;
 */
static void indexRead(char* oldIndexFilename, char* newIndexFilename) 
{
    if (oldIndexFilename == NULL || newIndexFilename == NULL) {
        fprintf(stderr, "invalid input for indexRead\n");
        return;
    }
    
    FILE* fp;
    char* result;
    char* key;
    int count;
    int docID;
    index_t* idxer = index_new();
    // open old index file
    fp = fopen(oldIndexFilename, "r");
    
    // read through lines
    while((result = file_readLine(fp)) != NULL) {

        key = strtok(result, " ");
        // tokenize the resulted line
        char* key1 = calloc(strlen(key)+1, sizeof(char));
        strcpy(key1, key);
        // order of loop
        int j = 1; 
        // tokenize again to get docID and count
        while ((key = strtok(NULL, " "))!= NULL) {
            if ((j % 2) != 0) {
                docID = atoi(key);
            } else {
                count = atoi(key);
                index_set(idxer, key1, docID, count);
            }
            //free(key1);
            j++;
        }
        free(key1);
        free(result);
    }
    
    fclose(fp);
    // print out the index into indexFilename
    index_write(idxer, newIndexFilename);
    // delete the index_t
    index_delete(idxer, NULL);
}


