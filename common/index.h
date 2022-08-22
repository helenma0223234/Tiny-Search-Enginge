/*
 * index.h - .h file for index data structure
 *
 * The index is a hashtable keyed by word and storing counters as items.
 * The counters is keyed by docID and stores a count of the number of occurrences
 * of that word in the document with that ID.
 *
 * Yuchuan Ma, May 6, 2021
 * CS50 Lab5, Spring 2021
 */

#ifndef __INDEX_H
#define __INDEX_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"
#include "counters.h"
#include "mem.h"

/**************** global types ****************/
typedef struct index index_t;  // opaque to users of the module



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
index_t* index_new();

/**************** index_insert ****************/
/* Insert item, identified by key (string), into the given index.
 *
 * Caller provides:
 *   valid pointer to index, valid string for key, valid pointer for item.
 * We return:
 *   false if key exists in ht, any parameter is NULL, or error;
 *   true iff new item was inserted.
 */
bool index_insert(index_t* idx, const char* key, const int docID);

/**************** index_find ****************/
/* Return the item associated with the given key.
 *
 * Caller provides:
 *   valid pointer to index, valid string for key.
 * We return:
 *   pointer to the item corresponding to the given key, if found;
 *   NULL if index is NULL, key is NULL, or key is not found.
 */
counters_t* index_find(index_t* idx, const char* key);

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
void index_iterate(index_t* idx, void* arg, 
        void (*itemfunc)(void* arg, const char* key, void* item) );
/***************** index_set *****************/
/* set a pair of docID, count in the given indexer.                             
 *                                                                       
 * Caller provides:                                                    
 *   valid pointer to index, key, docID, count                          
 *                                                                         
 * We do:                                                              
 *   return true nothing,return upon error                                 
 */
void index_set(index_t* idx, const char* key, const int docID, const int count);

/**************** index_write ****************/
/* Iterate over all items in the index; print them to a destination file.           
 *                                                                        
 * Caller provides:                                                          
 *   valid pointer to index,                                                     
 *   valid path to the file,                                      
 * We do:                                                                
 *   return true if sucess, false if error.                            
 */
bool index_write(index_t* idx, const char* indexFile);

/**************** index_read ****************/
/* read an indexfile to a index.                                                
 * return the index structure;                                                
 *                                                                              
 * User:                                                                        
 * is responsible to free this returned index later.     
 */
index_t* index_read(const char* indexFilename);

/**************** index_delete ****************/
/* Delete index, calling hash_delete function
 *
 * Caller provides:
 *   valid index pointer,
 *   valid pointer to function that handles one item (may be NULL).
 * We do:
 *   if index==NULL, do nothing.
 *   otherwise, unless itemfunc==NULL, pass the itemfunc on hashtable_delete.
 *   free all the hashtable inside the index, and the index itself.
 */
void index_delete(index_t* idx, void (*itemdelete)(void* item) );

#endif // __INDEX_H
