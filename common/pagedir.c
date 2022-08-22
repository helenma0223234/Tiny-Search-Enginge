/* 
 * pagedir.c - CS50 'pagedir' module
 *
 * see pagedir.h for more information.
 *
 * Yuchuan Ma, April 29 2021
 * CS50 Spring 2021, Lab4
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file.h"
#include "mem.h"
#include "webpage.h"

/****************** functions ******************/
/************** pagedir_init() **************/
bool 
pagedir_init(const char* pageDirectory)
{
    //int BUFSIZ = 100;
    char* path = mem_assert(malloc(100), "pageDir path");         // string to hold file path
    FILE* fp;

    if (pageDirectory == NULL) {
        return false;
    }
    // output the directory + file path to string 'path'
    sprintf(path, "%s/.crawler", pageDirectory);
    if (path == NULL) {
        return false;
    } 
    
    if ((fp=fopen(path, "w")) == NULL) {    // if cannot create/write the file
        fprintf(stderr, "could not open .crawler in %s!\n", pageDirectory);
        return false;
    } else {
        fclose(fp);
        free(path);
    }
    return true;
}

/************** pagedir_save() **************/
void 
pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID)
{   
    //char path[100];         // string to hold file path
    char* path = mem_assert(malloc(100), "pageDir path");
    FILE* fp;    // file holder
    // check input
    if (page == NULL || pageDirectory == NULL || docID < 0) {
        return;
    }
    // combine directory and file to a pathname
    sprintf(path, "%s/%d", pageDirectory, docID);
    if (path == NULL) {
        return;
    } 
    
    if ((fp=fopen(path, "w")) == NULL) {    // if cannot create/write the file
        fprintf(stderr, "could not open webpage file%d!\n", docID);
        free(path);
        return;
    } else {
        // print the page's information and content
        fprintf(fp, "%s\n", webpage_getURL(page));
        fprintf(fp, "%d\n", webpage_getDepth(page));
        fprintf(fp, "%s\n", webpage_getHTML(page));
        free(path);
        fclose(fp);
    }

}

/************** pagedir_validate() **************/
bool
pagedir_validate(const char* dir)
{
    char* path = mem_assert(malloc(100), "pageDir path");  // string to hold file path
    FILE* fp;
    // check input
    if (dir == NULL) {
        return false;
    }

    // combine directory and file name
    sprintf(path, "%s/.crawler", dir);
    if (path == NULL) {    // check if combined successfully
        return false;
    } 
    
    if ((fp=fopen(path, "r")) == NULL) {    // if cannot read the .crawler file
        free(path);
        return false;
    } else {
        // do nothing and close the file, return true
        fclose(fp);
        free(path);
    }

    return true;
}

/************** pagedir_fetch() **************/
webpage_t*
pagedir_fetch(const char* pageDirectory, const int docID)
{
    char* path = mem_assert(malloc(100), "pageDir path");
    FILE* fp;    // file holder
    // check input
    if (pageDirectory == NULL || docID < 0) {
        return NULL;
    }
    // combine directory and file to a pathname
    sprintf(path, "%s/%d", pageDirectory, docID);
    if (path == NULL) {
        return NULL;
    } 
    
    if ((fp=fopen(path, "r")) == NULL) {    // if cannot create/write the file
        free(path);
        return NULL;
    } else {
        // read the lines into new webpage_t
        char* url;
        url = file_readLine(fp);
        //char* depth;
        //depth = file_readLine(fp);
        int depth_int;
        fscanf(fp, "%d", &depth_int);
        
        char* html;
        html = file_readFile(fp);
        webpage_t* page = webpage_new(url, depth_int, html);
        // free memory used
        free(path);
        fclose(fp);

        return page;
    }

}
