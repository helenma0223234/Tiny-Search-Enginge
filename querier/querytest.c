/* 
 * querytest.c - testing script for query module.
 *
 * Yuchuan Ma, May 16, 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <counters.h>
#include <query.h>

int main() {
    counters_t* ctr1 = NULL;           // one bag
    counters_t* ctr2 = NULL;           // another bag
    counters_t* ctr_emp = NULL;

    printf("Creating counters and inserting test keys...\n");
    ctr1 = counters_new();
    ctr2 = counters_new();
    ctr_emp = counters_new();

    counters_set(ctr1, 6, 9);
    counters_set(ctr1, 1, 1);
    counters_set(ctr1, 2, 2);
    counters_set(ctr1, 3, 3);

    counters_set(ctr2, 6, 9);
    counters_set(ctr2, 1, 2);
    counters_set(ctr2, 2, 2);
    counters_set(ctr2, 4, 4);
    printf("ctr1:");
    counters_print(ctr1, stdout);
    printf("\n");
    printf("ctr2:");
    counters_print(ctr2, stdout);
    printf("\n");

    printf("testing intersection...\n");
    counters_t* intersect1;
    counters_t* intersect2;

    intersect1 = query_intersect(ctr1, ctr_emp);
    printf("printing intersection of ctr1 and empty counters...\nshould be empty\n");
    counters_print(intersect1, stdout);
    printf("\n");

    intersect2 = query_intersect(ctr1, ctr2);
    printf("printing intersection of ctr1 and ctr2...\n");
    printf("should be have keys: 6,9 and 2,2, 1,1\n");
    counters_print(intersect2, stdout);
    printf("\n");

    printf("testing merging...\n");
    query_merge(ctr1, ctr_emp);
    printf("merged empty counter to ctr1, printing ctr1...\nshould be unchanged\n");
    counters_print(ctr1, stdout);
    printf("\n");

    query_merge(ctr1, ctr2);
    printf("merged ctr2 to ctr1, printing ctr1...\n");
    printf("should be have keys: 6,18, 2,4, 1,3, 4,4, 3,3\n");
    counters_print(ctr1, stdout);
}
