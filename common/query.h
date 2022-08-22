/* 
 * query.h - header file for CS50 query module
 *
 * The query module is similar to a supplementary package to the counters structure;
 * it contains specific functions that merge two counters and 
 * find intersection of two counters.
 * The functions are specific to the querier and used to update/generate
 * th final query counters bag.
 *
 * Yuchuan Ma, May 16, 2021
 * CS50 Spring 2021
 */

#ifndef __QUERY_H
#define __QUERY_H

#include <stdio.h>
#include <stdbool.h>

/**************** FUNCTION ****************/
/**************** query_merge ****************/
/* Merge the two given counters by calling merge helper
 *
 * Caller provides:
 *     valid pointer to countersets.
 * We return:
 *     nothing, the second counter is merged into the first.
 *     B is merged to A, and the count is added if the key is 
 *     present in both sets, the count of each item key
 *     is the score of the key for the query.
 */
void query_merge(counters_t* ctrA, counters_t* ctrB);

/**************** query_intersect ****************/
/* Intersect the two given counters by calling intersect helper
 *
 * Caller provides:
 *     valid pointer to countersets.
 * We return:
 *     a pointer to a counterset that contains the intersect keys. 
 * Note: 
 *     key's count is the smaller score from the one of the original counters
 *     it is also their query score;
 */
counters_t* query_intersect(counters_t* ctrA, counters_t* ctrB);

/**************** query_count ****************/
/* count the given counterset.
 *
 * Caller provides:
 *     valid pointer to countersets.
 * We return:
 *     an integer that represents the count.
 */
int query_count(counters_t* ctr);



#endif // __QUERY_H
