/* word.c
 *
 * A common module that normalizes a word into all lowercase. Used by the indexer and querier.
 *
 * Usage: call the function normalizeWord
 *
 * Yuchuan Ma, May 6, 2021
 * Lab5 CS50 Spring 2021
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

/* ************** normalizeWord ************* */
/* Change a word to all lower case.
 * 
 * Input: address of a string pointer
 * Output: void; the actual word string is changed at its address.
 *
 * We assume: valide pointer to a string as input
 * Return if NULL input and print error.
 */

char* normalizeWord(char* word) {
    if (word == NULL) {
        return NULL;
        fprintf(stderr, "null input for normalizeWord/n");
    }

    char* new = calloc(strlen(word)+1, sizeof(char));
    int i = 0;
    char let;
    strcpy(new, word);
    while (new[i] != '\0'){
        let = new[i];
        new[i] = tolower(let);
        i++;
    }

    return new;
}
