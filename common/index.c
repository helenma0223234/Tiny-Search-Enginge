/* 
 * index.c - CS50 TSE project
 *
 * A module that maps from word to *(docID, #occurrences)* pairs.
 *
 * The index is a hashtable keyed by word and storing counters as items.
 * The counters is keyed by docID and stores a count of the number of occurrences 
 * of that word in the document with that ID.
 *
 * Yuchuan Ma, May 6, 2021
 * CS50 Lab5, Spring 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"
#include "counters.h"
#include "mem.h"
#include "file.h"

/**************** global types ****************/
typedef struct index {
  hashtable_t* hash;
} index_t;

/***************** functions ****************/
/***************** private functions prototypes *****************/
static void hsprint(void* arg, const char* key, void* item);
static void simpleprint(void* arg, const int key, const int count);
static void counters_del_helper(void* item);
/***************** global functions *****************/
/**************** index_new() ****************/
/* Create a new (empty) index.
 *
 * We return:
 *   pointer to the new hashtable; return NULL if error.
 * We guarantee:
 *   hashtable is initialized empty.
 * Caller is responsible for:
 *   later calling hashtable_delete.
 */
index_t*
index_new() 
{
    // make memory for the index
    index_t* idx = mem_assert(malloc(sizeof(index_t)), "new idx");
    // initialize the hashtable inside the index struct
    //hashtable_t* hash = mem_assert(malloc(sizeof(hashtable_t*)), "newhash");
    idx->hash = hashtable_new(666);

    return idx;
}

/**************** index_insert ****************/
/* Insert item, identified by key (string), into the given index.
 *
 * Caller provides:
 *   valid pointer to index, valid string for key, valid pointer for item.
 * We return:
 *   false if key exists in ht, any parameter is NULL, or error;
 *   true iff new item was inserted.
 */
bool
index_insert(index_t* idx, const char* key, const int docID)
{
    if (idx == NULL || key == NULL ) {
        return false;
    }
    
    hashtable_t* ht = idx->hash;
    //find the key if it exists
    void* item = hashtable_find(ht, key);
    // if the key doesn't exist
    if (item == NULL) {
        counters_t* ctrs = counters_new();
        if (counters_add(ctrs, docID) != 0) {
            hashtable_insert(ht, key, ctrs);
            return true;
        } else {
            return false;
        }
    } else { // if it exists, add/increment the counters
        if (counters_add(item, docID) == 0) {
            return false;
        } else {
            return true;
        }
    }

}

/**************** index_find ****************/
/* Return the item associated with the given key.
 *
 * Caller provides:
 *   valid pointer to index, valid string for key.
 * We return:
 *   pointer to the item corresponding to the given key, if found;
 *   NULL if index is NULL, key is NULL, or key is not found.
 */
counters_t*
index_find(index_t* idx, const char* key) 
{
    // check input params
    if (idx == NULL || key == NULL ) {
        return NULL;
    }

    return hashtable_find(idx->hash, key);
}

/**************** index_iterate ****************/
/* Iterate over all items in the index; in undefined order.
 *
 * Caller provides:
 *   valid pointer to index, 
 *   arbitrary void*arg pointer,
 *   itemfunc that can handle a single (key, item) pair.
 * We do:
 *   nothing, if idx==NULL or itemfunc==NULL.
 *   otherwise, call hashtable_insert with (arg, key, item).
 */
void
index_iterate(index_t* idx, void* arg, void (*itemfunc)(void* arg, const char* key, void* item) ) 
{
    if (idx == NULL || itemfunc == NULL) {
        return;
    }
    // evoke hashtable_iterate with the itemfunc
    hashtable_iterate(idx->hash, arg, itemfunc);
    return;
}

/*************** index_set ******************/
/* see index.h for details about this function */
void
index_set(index_t* idx, const char* key, const int docID, const int count) 
{
    if (idx == NULL || key == NULL) {
        return;
    }
    // pull out hashtable
    hashtable_t* ht = idx->hash;
    // check if counters exists
    counters_t* ctrs = hashtable_find(ht, key);

    // if the key exists
    if (ctrs != NULL) {
        counters_set(ctrs, docID, count);
    } else {
        // create new counters
        counters_t* ctrs1 = counters_new();
        counters_set(ctrs1, docID, count);
        hashtable_insert(ht, key, ctrs1);
    }
}

/**************** index_write ****************/
/* Iterate over all items in the index; print them to a destination file.      
 *                                                            
 * Caller provides:                                    
 *   valid pointer to index,  
 *   valid path to the file,                                                   
 * We do:                                                          
 *   return true if sucess, false if error.        
 */
bool
index_write(index_t* idx, const char* indexFile)
{
    if (idx == NULL || indexFile == NULL) {
        return false;
    }
    FILE* fp;
    if ((fp=fopen(indexFile, "w"))== NULL) {
        return false;
    }
    index_iterate(idx, (void*)fp, hsprint);
    fclose(fp);

    return true;
}

/**************** index_read ****************/
/* read an indexfile to a index.
 * return the index structure;
 *
 * User:
 * is responsible to free this returned index later.
 */
index_t*
index_read(const char* indexFilename)
{   
    FILE* fp;
    char* result;
    char* key;
    int count;
    int docID;

    if(indexFilename == NULL)
        return NULL;

    index_t* idxer = index_new();

    fp = fopen(indexFilename, "r");

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

    return idxer;
}


/**************** index_delete ****************/
/* Delete index, calling hash_delete function
 *
 * Caller provides:
 *   valid index pointer,
 *   valid pointer to function that handles one item (may be NULL).
 * We do:
 *   if idx==NULL, do nothing.
 *   otherwise, unless itemfunc==NULL, pass the itemfunc on hashtable_delete.
 *   free all the hashtable inside the index, and the index itself.
 */
  
void
index_delete(index_t* idx, void (*itemdelete)(void* item) )
{
    if (idx == NULL) {
        return;
    }
    // delete the hash table
    hashtable_delete(idx->hash, counters_del_helper);
    // free up the memory
    free(idx);
}

/***************** private functions *****************/
/* ******************* hsprint ******************* */
/* A print function that prints out the key (the word) to a given file,            
 * and calls counters_iterate to print out the items within the counters            
 * Used to print out the index map into the destination file.             
 *                                                       
 * Returns nothing is success. Print error if invalid input.                       
 */
static void
hsprint(void* arg, const char* key, void* item)
{
    if (arg == NULL || key == NULL) {
        return;
    }

    FILE* fp = (FILE*)arg;
    fprintf(fp, "%s ", key);
    counters_iterate((counters_t*)item, arg, simpleprint);
    fprintf(fp, "\n");

}

/* ******************* simpleprint ******************* */
/* print the given item to the given file;                              
 * just print the key and item.                                                             
 */
static void
simpleprint(void* arg, const int key, const int item)
{
  FILE* fp = (FILE*)arg;
  if (fp != NULL) {
    fprintf(fp, "%d %d", key, item);
  }
}

/* ************* counters_del_helper ************ */
static void
counters_del_helper(void* item)
{
  counters_delete((counters_t*)item);
}
