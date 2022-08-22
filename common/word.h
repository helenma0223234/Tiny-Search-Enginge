/* word.h - .h file for word.c                                                    
 *                                              
 * A common module that normalizes a word into all lowercase. 
 * Used by the indexer and querier.        
 *                                                       
 * Usage: call the function normalizeWord                                     
 *                                                  
 * Yuchuan Ma, May 6, 2021                          
 * Lab5 CS50 Spring 2021                                                
 */

#ifndef __WORD_H
#define __WORD_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>


/* ************** normalizeWord ************* */
/* Change a word to all lower case.            
 *                
 * Input: address of a string pointer                                  
 * Output: void; the actual word string is changed at its address.                
 *                                                                           
 * We assume: valide pointer to a string as input                                   
 * Return if NULL input and print error.                                
 */
char* normalizeWord(char* word);

#endif // __WORD_H
