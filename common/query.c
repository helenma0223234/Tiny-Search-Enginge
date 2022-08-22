/* 
 * query.c - implementation of the query module fuctions
 *
 * See query.h for documentation.
 * 
 * Yuchuan Ma, May 16, 2021
 * CS 50 Spring 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <counters.h>
#include <mem.h>

/**************** local types ****************/
typedef struct ctr_bag {
    counters_t* ctr1;
    counters_t* ctr2;
} ctr_bag_t;

/*typedef struct doc_array {
    //int docNum;
    int current;
    int (*arr)[];
} doc_array_t;*/

/**************** functions ****************/
/**************** local functions ****************/
static void query_merge_helper(void* arg, const int key, const int count);
static void query_intersect_helper(void* arg, const int key, const int count);
static void countfunct(void* arg, const int key, const int count);
//static void pullKey(void* arg, const int key, const int count);

/**************** query_merge ****************/
/* merge countersB to countersA.
 * see query.h for details.
 */
void 
query_merge(counters_t* ctrA, counters_t* ctrB)
{
  counters_iterate(ctrB, ctrA, query_merge_helper);
}

/************* query_merge_helper *************/
/* helper func to query_merge
 * 
 * if the key exists in ctrA, update its score;
 * otherwise, add the key into ctrA
 */
static void 
query_merge_helper(void* arg, const int key, const int count)
{
  if (arg != NULL && count > 0 && key >0) { 
    counters_t* ctrA = arg;
    int countB = count;

    // find the same key in ctrA
    int countA = counters_get(ctrA, key);
    if (countA == 0) {
      // not found: insert it
      counters_set(ctrA, key, countB);
      //printf("\t%d added\n", key);
    } else {
      // add to the existing value
      countA += countB; 
      counters_set(ctrA, key, countA);
      
      //printf("\t%d exists\n", key);
    }  
  }
}

/**************** query_intersect ****************/
/* intersect countersA and countersB.
 * see query.h for details.
 */
counters_t* 
query_intersect(counters_t* ctrA, counters_t* ctrB)
{
    if (ctrA == NULL || ctrB == NULL) {
        return NULL;
    }
    // create a struct to hold two bags
    ctr_bag_t* ctrBag;
    ctrBag = mem_assert(malloc(sizeof(ctr_bag_t)), "ctrBag");
    ctrBag->ctr1 = ctrA;
    counters_t* intersect = counters_new();    // need to free this later
    ctrBag->ctr2 = intersect;
    
    // iterate the bagB, 
    counters_iterate(ctrB, ctrBag, query_intersect_helper);
    free(ctrBag);

  return intersect;
}

/************* query_intersect_helper *************/
/* Helper func to query_intersect
 *
 * Takes in the ctr_bag_t, looks for common keys in ctrA
 * and ctrB, insert into the intersect counter if needed
 * Update the count according to the query's scoring method
 */
static void 
query_intersect_helper(void* arg, const int key, const int count)
{
   if (arg != NULL && count > 0 && key >0){

       ctr_bag_t* ctrBag = (ctr_bag_t*)arg;
       int countB = count;

       // find the same key in ctrA
       int countA = counters_get(ctrBag->ctr1, key);
       if (countA != 0) {
        // if found, add existing count and add to the intersect counterset
        if(countA <= countB) {
            counters_set(ctrBag->ctr2, key, countA);
        } else {
            counters_set(ctrBag->ctr2, key, countB);
        }
        //printf("\t%d intersects\n", key);
      }  
    }
}

/* ***************** query_count  ******************* */
/* count the keys in the counterset */
int
query_count(counters_t* ctr) {
    int keynum = 0;
    counters_iterate(ctr, &keynum, countfunct);

    return keynum;
}


/* ***************** itemcount  ******************* */
/* count the non-null items in the counterset */
static void countfunct(void* arg, const int key, const int count)
{
    int* nitem = arg;

    if (nitem != NULL && key > 0 && count > 0) {
        (*nitem)++;
    }
}


