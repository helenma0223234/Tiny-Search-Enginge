/* 
 * pagedir.h - header file for CS50 TSE 'pagedir' module
 * 
 * The re-usable module handles the pagesaver (writing a page to the pageDirectory)
 * and marking it as a Crawler-produced pageDirectory
 *
 * Yuchuan Ma, April 29, 2021
 * CS50 Spring Lab4
 */

#ifndef __PAGEDIR_H
#define __PAGEDIR_H

#include <stdio.h>
#include <stdbool.h>
#include "file.h"
#include "mem.h"
#include "webpage.h"

/**************** functions ****************/
/**************** pagedir_init ****************/
/* Construct the pathname for .crawler file in the given directory
 * Open the file for writing, close it.
 *
 * We assume:
 * The input directory is a valide pathname.
 * We return:
 * True if the crawler file is successfully opened and closed.
 * False otherwise (if encounters error).
 */
bool pagedir_init(const char* pageDirectory);

/**************** pagedir_saver ****************/
/* Construct the pathname for the page file in pageDirectory
 *
 * We assume:
 * The input webpage pointer, directory of the page and document ID are valid.
 * We return:
 * Void if the page file is successfully opened and printed with necessary information.
 * NULL if any input value is NULL, page file not opened with writting,
 * or documentID < 0
 */

void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);

/**************** pagedir_validate ****************/
/* Verify whether dir is indeed a Crawler-produced directory.
 *
 * We assume:
 * valid string pointer as input, it should be a  directory path
 * We return:
 * True if the directory is created by running the crawler.
 * False otherwise and upon errors.
 */
bool pagedir_validate(const char* dir);

/**************** pagedir_fetch ****************/
/* load a webpage_t from a file in page directory
 * first line into the webpage_t's URL
 * second line of the file's page
 * rest of the file as content
 *
 * We assume:
 * Input a valid page directory that is validated by pagedir_validate;
 * A docID that is within  the range of page file the directory contains
 * We return:
 * A webpage_t pointer that is generated according to the page file's content,
 * and NULL upon errors.
 */
webpage_t* pagedir_fetch(const char* pageDirectory, const int docID);

#endif // __PAGEDIR_H
