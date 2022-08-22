/* 
 * crawler.c - CS50 TSE sub-system Crawler
 *
 * Usage: only interface with the user is on the command-line
 *        it takes three arguments: $ crawler seedURL pageDirectory maxDepth.
 *
 * Input: no file input.
 * Output: Per the requirements spec, Crawler will save each explored webpage to a file, 
 *         one webpage per file, using a unique documentID as the file name.
 *
 * Data Structures: struct bag, struct hashtable and struct webpage
 *
 * See Lab4 Design, and Requirements specs for more details.
 *
 * Yuchuan Ma, April 29, 2021
 * CS50 Spring 2021 Dartmouth
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "mem.h"
#include "bag.h"
#include "hashtable.h"
#include "webpage.h"
#include "pagedir.h"

/* *********************** private functions prototype ************************** */
static void parseArgs(const int argc, char* argv[], char** seedURL, char** pageDirectory, int* maxDepth);
static void crawl(char* seedURL, char* pageDirectory, const int maxDepth);
static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen);

/* ************************* main **************************** */
int 
main(const int argc, char* argv[]) 
{
    // declare and initialize variables for each of the expected arguments
    char* seedURL = NULL;    // input url address
    char* pageDirectory = NULL;  // input crawler output destination
    int maxDepth = 0;  // input depth to crawl
    // process arguments
    parseArgs(argc, argv, &seedURL, &pageDirectory, &maxDepth);
    // run the crawler
    crawl(seedURL, pageDirectory, maxDepth);

    return 0;
}

/* ******************* parseArgs ******************* */
/* Handle the messy business of parsing command-line arguments, so main() is cleaner and clearer. 
 * Extract seedURL, destination directory and crawler's depth from input arguments
 * and then 1) normalize URL, 2) initialize crawler destination directory
 *
 * We return 'void' when successful; 
 * Otherwise, print error or usage message to stderr and exit non-zero.
 * Note: following codes are sourced and adapted from CS50 examples/parseArgs.c
 */
static void 
parseArgs(const int argc, char* argv[], 
            char** seedURL, char** pageDirectory, int* maxDepth)
{
    const char* progName = argv[0]; // name of this program

    if (argc != 4) {
        fprintf(stderr, "usage: %s seedURL pageDirectory maxDepth\n", progName);
        exit(1);
    }
    // save arguments to pointers,and  make sure they are valid input
    *seedURL = normalizeURL(argv[1]);
    if (*seedURL == NULL) {
        fprintf(stderr, "%s: '%s' invalid URL\n", progName, argv[1]);
        exit(4);
    }
    if (isInternalURL(*seedURL) == false) {
        fprintf(stderr, "%s: '%s' external URL \n", progName, argv[1]);
        exit(2);
    }

    *pageDirectory = argv[2];
    if (pagedir_init(*pageDirectory) == false){
        fprintf(stderr, "%s: '%s' invalid directory\n", progName, *pageDirectory);
        exit(4);
    }

    // convert maxDepth to an int
    const char* depthString = argv[3];  // local holder of the string input of depth num
    *maxDepth = 0;  // initialize the depth num
    char excess;    // holder of excess char after the num

    // if the argument is invalid, sscanf fails and print error
    if (sscanf(depthString, "%d%c", maxDepth, &excess) != 1) {
        fprintf(stderr, "%s: '%s' invalid integer\n", progName, depthString);
        exit(4);
    }

    if (*maxDepth < 0 || *maxDepth > 10) {
        fprintf(stderr, "%s: '%d' must be >= 0 and <= 10\n", progName, *maxDepth);
        exit(3);
    }
}

/* ******************* crawl  ******************* */
/* Crawling from seedURL to maxDepth and saving pages in pageDirectory
 *
 * The function is responsible to make memory for the data structures bag and hashtable
 * and also later freeing them after crawling.
 * The crawler fetches HTML(s) in a given webpage then recycles this process untill the seedURL 
 * is exhausted or maxDepth is reached. Each time it finishes feachting all urls from one webpage,
 * depth +1.
 * Write the HTML to the pageDirectory with a unique document ID when a HTML is successfully fetched.
 *
 * We return nothing if success; otherwise, Print out error and return NULL.
 */
static void 
crawl(char* seedURL, char* pageDirectory, const int maxDepth) 
{   
    int docID = 0;  // unique document ID for each webpage
    if (seedURL == NULL || pageDirectory == NULL || maxDepth < 0) {
        fprintf(stderr, "wrong input for function crawl\n");
        return;
    }
    // initialize the bag, hashtable
    hashtable_t* pagesSeen  = hashtable_new(200);
    if (pagesSeen == NULL) {
        fprintf(stderr, "failed to to create hashtable\n");
    }
    bag_t* toCrawl = bag_new();
    if (toCrawl == NULL) {
        fprintf(stderr, "failed to to create bag\n");
    }

    // initialize seedURL and insert it into the bag and hs
    hashtable_insert(pagesSeen, seedURL, "a");
    webpage_t* seed = webpage_new(seedURL, 0, NULL);
    if (seed == NULL) {
        fprintf(stderr, "failed to create seed webpage\n");
    }
    bag_insert(toCrawl, seed);

    // extract a webpage item while bag not empty

    webpage_t* page;
    while ((page = bag_extract(toCrawl)) != NULL) {  
        // pasue for one sec
        //sleep(1);

        if (webpage_fetch(page)) {  // retrieve a HTML for the that URL 

            
            // if not at max depth yet:
            if (webpage_getDepth(page) <= maxDepth){
                printf("%-3d Fetched: %s\n", webpage_getDepth(page), webpage_getURL(page));
                // write the webpage to the pageDirectory with a unique document ID
                docID++;
                pagedir_save(page, pageDirectory, docID); 

                printf("%-3d Scanning: %s\n", webpage_getDepth(page), webpage_getURL(page));
                // extract all embedded URLs
                pageScan(page, toCrawl, pagesSeen);
            }

        } else {
            fprintf(stderr, "failed to pull html from %s\n", webpage_getURL(page));
            continue;
        }

       webpage_delete(page);        
    }

    // delete the hashtable and bag
    hashtable_delete(pagesSeen, NULL);
    bag_delete(toCrawl, NULL);
}

/* ******************* pageScan ******************* */
/* Given a webpage, scan the given page to extract any links (URLs), ignoring non-internal URLs 
 * and already-seen URLs.
 *
 * Wessume the webpage, bag and hashtable are valid input (data structures).
 * We return nothing if success; otherwise, Print out error and return NULL.
 */
static void 
pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen) 
{
    // position and char holders for scanning URL
    int pos = 0;
    char* result;

    while ((result = webpage_getNextURL(page, &pos)) != NULL) {
        char* result1 = normalizeURL(result);
        free(result);
        if (result1 == NULL) {
            free(result1);
            continue;
        }
        printf("%-3d Found: %s\n", webpage_getDepth(page), result1);

        if (isInternalURL(result1)) {
            // if the URL is internal
            //char* result2 = malloc(strlen(result1)+1);
            //strcpy(result2, result1);
            //free(result1);
            if(hashtable_insert(pagesSeen, result1, "a")) { // add the webpage to hashtable
                // if successfully added, means not have seen it yet
                // create a webpage_t for it, insert into bag
                webpage_t* new = webpage_new(result1, webpage_getDepth(page)+1, NULL);
                bag_insert(pagesToCrawl, new);

                printf("%-3d Added: %s\n", webpage_getDepth(page), result1);
            } else {
                // if seen the URL
                printf("%-3d IgnDupl: %s\n", webpage_getDepth(page), result1);
                free(result1);
            }
        } else {
            // if URL external
            printf("%-3d IgnExtrn: %s\n", webpage_getDepth(page), result1);
            free(result1);
        }
        
    }
}


