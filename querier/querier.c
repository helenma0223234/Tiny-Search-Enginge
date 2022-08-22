/* querier.c - CS50 TSE Lan querier
 * 
 * The TSE Querier is a standalone program that reads the index file 
 * produced by the TSE Indexer, and page files produced by the TSE 
 * Querier, and answers search queries submitted via stdin.
 *
 * See the Design and Requirement specs for details.
 *
 * Yuchuan Ma, May 17, 2021
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include "mem.h"
#include "index.h"
#include "query.h"
#include "counters.h"
#include "pagedir.h"
#include "readlinep.h"
#include "file.h"
#include "webpage.h"

/**************** local types ****************/
typedef struct query_str {
    char* qstring;
    int wordcount;
} query_str_t;

typedef struct document {
    int id;
    int score;
} document_t;


/* *********************** private functions prototype ************************** */
int main(const int argc, char* argv[]);
static void parseArgs(const int argc, char* argv[],
                      char** pageDirectory, char** indexFilename);
static void querier(char* pageDirectory, char* indexFilename);
static char* normalQuery(query_str_t* queryString);
static counters_t* parseQuery(index_t* fileIdx, query_str_t* queryString);
int fileno(FILE *stream);
static void prompt(void);
static index_t* indexRead(const char* indexFilename);
static void buildDocs(counters_t* ctr, document_t** docArray, const int docCount);
static void build_docs_helper(document_t** docArray, const int slotnum); 
static void printDocs(document_t** docArray, const int docCount, const char* pageDirectory);
static document_t* document_new(void);
static void insertDoc(void* arg, const int key, const int count); 

/* ************************* main **************************** */
int
main(const int argc, char* argv[])
{
    // declare and initialize variables for each of the expected arguments
    char* pageDirectory = NULL;    // input crawler-produced directory
    char* indexFilename = NULL;  // input index file destination
    // process arguments
    parseArgs(argc, argv, &pageDirectory, &indexFilename);
    // build the index
    querier(pageDirectory, indexFilename);

    return 0;
}

/* ******************* parseArgs ******************* */
/* Handle the messy business of parsing command-line arguments, so main() is cleaner and clearer.
 * Extract  destination directory and index file destination path from input arguments
 *
 * We return 'void' when successful;
 * Otherwise, print error or usage message to stderr and exit non-zero.
 * Note: following codes are sourced and adapted from CS50 examples/parseArgs.c
 */
static void
parseArgs(const int argc, char* argv[], char** pageDirectory, char** indexFilename)
{
    const char* progName = argv[0]; // name of this program

    if (argc != 3) {
        fprintf(stderr, "usage: %s pageDirectory indexFilename\n", progName);
        exit(1);
    }

    // save arguments to pointers,and  make sure they are valid input
    *pageDirectory = argv[1];
    if (pagedir_validate(*pageDirectory) == false){
        fprintf(stderr, "%s: '%s' invalid directory\n", progName, *pageDirectory);
        exit(2);
    }

    *indexFilename = argv[2];
    FILE* fp;
    if ((fp=fopen(*indexFilename, "r")) == NULL) {    // if cannot write in the file
        fprintf(stderr, "%s: '%s' invalid indexer destination\n", progName, *indexFilename);
        exit(3);
    } else {
        fclose(fp);
    }
}

/* ******************* querier ******************* */
/* Do the real work of querying, by reading stdin user input
 * line by line as query strings;
 * Parse the string and query it according to precedence of and/or;
 * Print the final result to stdout.
 * 
 * return void when successful; otherwise, Print out error and return;
 */
static void
querier(char* pageDirectory, char* indexFilename)
{
    if (pageDirectory == NULL || indexFilename == NULL) {
        fprintf(stderr, "invalid input for indexBuild\n");
        return;
    }
    // read index file to data struct
    index_t* fileIdx = indexRead(indexFilename);  // need to delete
    if (fileIdx == NULL) {
        fprintf(stderr, "empty file index structure\n");
        return;
    }
       
    prompt();
    // read stdin line by line
    char* result;
    while((result = freadLinep(stdin)) != NULL) {

        // initialize query string structure
        query_str_t* queryString;
        queryString = mem_assert(malloc(sizeof(query_str_t)), "queryString");
        queryString->qstring = result;
        queryString->wordcount = 1;
        // normalize the qstring and update the structure's pointer to the normalized str
        char* normalized = normalQuery(queryString); //need to free this
        free(result);
        // if the qeury request is invalid
        if (normalized == NULL) {
            prompt();
            //free(normalized);
            free(queryString);
            continue;
        }
        queryString->qstring = normalized;
        // print out the normalized queery
        printf("Query: %s\n", normalized);
        // parse and query the string
        counters_t* resultCtr = parseQuery(fileIdx, queryString);
        free(queryString->qstring);
        free(queryString);
        if (resultCtr == NULL) {
            printf("-----------------------------------------------\n");
            prompt();
            continue;
        }

        // count result
        int docCount = query_count(resultCtr);
        // if no doc satisfies the querying
        document_t** docArray;
        if (docCount == 0) {
            counters_delete(resultCtr);
            printf("No documents match.\n");
            printf("-----------------------------------------------\n");
            prompt();
            continue;
        } else {
            printf("Matches %d documents (ranked):\n", docCount);
            docArray = calloc(docCount, sizeof(document_t*));
            for (int in = 0; in < docCount; in++) {
                docArray[in] = document_new();
            }
            buildDocs(resultCtr, docArray, docCount);
            counters_delete(resultCtr);
            // print specifics
            printDocs(docArray, docCount, pageDirectory);
        }

        //counters_delete(resultCtr);
        // free the docarray data struct
        for (int i = 0; i < docCount; i++) {
            free(docArray[i]);
        }
        free(docArray);
        printf("-----------------------------------------------\n");
        prompt();
    }
    //free(normalized);
    free(result);
    printf("Done! result freed\n");
    printf("deleting idx...\n");
    index_delete(fileIdx, NULL);
}

/* ******************* normalQuery  ******************* */
/* parse the string to normalized form
 * normalize words and get rid of spaces, add "add" between
 * query words.
 */
static char* 
normalQuery(query_str_t* queryString)
{
    char* queryStr = queryString->qstring;
    int length = strlen(queryStr);
    bool newWord = false;
    int newIdx = 0;
    int newWordIdx = 0;

    if (queryString == NULL) {
        fprintf(stderr, "empty query to normalize\n");
        return NULL;
    }
    
    char* newQuery = mem_assert(calloc(length+1, sizeof(char)), "normalizeQue");
    for(int i = 0; i < length; i++) {
        // if not a letter and not a space, print error
        if ((isalpha(queryStr[i]) == 0) && (isspace(queryStr[i]) == 0)){
            printf("Error: bad character '%c' in query.\n", queryStr[i]);
            free(newQuery);
            return NULL;
        } else {
            if (isalpha(queryStr[i]) != 0) {  // if is a letter
                if (newWord == false) {  // turn the new word flag on if not yet
                    newWord = true;
                    newWordIdx = i;
                }
                newQuery[newIdx] = tolower(queryStr[i]);  // copy the char to copy string
                newIdx++;
            } else { // if is space
                if (newWord == true) { // if by the end of word
                    newQuery[newIdx] = queryStr[i];
                    newIdx++;
                    newWord = false;  // turn newWord to false so won't read extra spaces
                    // in case when the word is followed by extra spaces
                    queryString->wordcount++;
                }
            }
        }
    }
    //free(queryString->qstring);
    return newQuery;
}

/* ******************* getNextWord ******************* */
/* Get the next word of the string after the given position
 * return a new pointer to the word string; update the position.
 *
 * Note:
 * following code partly from adopted course-provided starterkit of this lab
 * specifically from ../common/webpage.c, webpage_getNextWord function
 */
static char* 
getNextWord(const char* str, int* pos)
{
    if (str == NULL || pos == NULL) {
    	return NULL;
    }

    const char* beg;
    const char* end;
    // skip non-alpha char
	while (str[*pos] != '\0' && !isalpha(str[*pos])) {
		(*pos)++;
	}
	// if ran out of text
	if (str[*pos] == '\0') {
		return NULL;
	}

	// str[*pos] is the first character of a word
	beg = &(str[*pos]);
	  // consume word
	  while (str[*pos] != '\0' && isalpha(str[*pos])) {
        (*pos)++;
	}
	// at this point, str[*pos] is the first character *after* the word.
	// so str[*pos-1] is the last character of the word.
	 end = &(str[*pos-1]);
	 // 'beg' points to first character of the word
	 // 'end' points to last character of the word
    int wordlen = end - beg + 1;

    // allocate space for length of new word + '\0'
    char* word = mem_assert(calloc(wordlen + 1, sizeof(char)), "readword");
    if (word == NULL) {        // out of memory!
    	return NULL;
    } else {
        // copy the new word
        strncpy(word, beg, wordlen);
        return word;
    }
}

/* ******************* parseQuery  ******************* */
/* Parse the query string according to and/or precedence.
 * Build query counterset struct while getting each new query sequence.
 *
 * print error if the logic is incorrect
 * return the query counterset.
 */
static counters_t*
parseQuery(index_t* fileIdx, query_str_t* queryString) 
{
    if (fileIdx == NULL || queryString == NULL) {
        fprintf(stderr, "Nothing to query\n");
        return NULL;
    }
	const char* queryStr = queryString->qstring;
	int count = 0;
	char* andstr = "and";
	char* orstr = "or";
    bool first = false;
    bool second = false;
	bool and1 = false;
	bool or1 = false;
	bool and2 = false;
	bool or2= false;
    bool thisAnd = false;
    bool thisOr = false;
    bool to_merge = true;    // flag for merging temp counterset to result counterset
    //bool to_intersect = false;

	int pos = 0;
	char* result;
    counters_t* temp;
    counters_t* resultCtrs;
    resultCtrs = counters_new();
    temp = counters_new();
 
    while ((result = getNextWord(queryStr, &pos)) != NULL) {
        // count the word and compare to and/or
        count++;
        // flag and compare to and/or if is first or second word 
        // in a pair to determine literal adjacency
        if (first == false) {
            first = true;
            and1 = (strcmp(andstr, result)==0);
		    or1 = (strcmp(orstr, result)==0);
        } else {
            second = true;
            and2 = (strcmp(andstr, result)==0);
		    or2 = (strcmp(orstr, result)==0);
            first = false;
            second = false;
        }
        // if literals at the beginging or end
		if (count == 1 && and1) {
			printf("Error: 'and' cannot be first\n");
			free(result);
            counters_delete(temp);
            counters_delete(resultCtrs);
			return NULL;
		} else if (count == 1 && or1) {
			printf("Error: 'or' cannot be first\n");
			free(result);
            counters_delete(temp);
            counters_delete(resultCtrs);
			return NULL;
		} else if ((count == queryString->wordcount && and1) || 
                (count == queryString->wordcount && and2)) {
			printf("Error: 'and' cannot be last\n");
			free(result);
            counters_delete(temp);
            counters_delete(resultCtrs);
			return NULL;
		} else if ((count == queryString->wordcount && first && or1) || 
                (count == queryString->wordcount && or2 && !first && !second)) {
			printf("Error: 'or' cannot be last\n");
			free(result);
            counters_delete(temp);
            counters_delete(resultCtrs);
			return NULL;
		}
		// if literals adjacent
		if (and1 && and2) {
			printf("Error: 'and' and 'and' cannot be adjacent\n");
			free(result);
            counters_delete(temp);
            counters_delete(resultCtrs);
			return NULL;
		} else if (or1 && or2) {
			printf("Error: 'or' and 'or' cannot be adjacent\n");
			free(result);
            counters_delete(temp);
            counters_delete(resultCtrs);
			return NULL;
		} else if (and1 && or2) {
			printf("Error: 'and' and 'or' cannot be adjacent\n");
			free(result);
            counters_delete(temp);
            counters_delete(resultCtrs);
			return NULL;
		} else if (or1 && and2) {
			printf("Error: 'or' and 'and' cannot be adjacent\n");
			free(result);
            counters_delete(temp);
            counters_delete(resultCtrs);
			return NULL;
		}
        if (first == false && second ==false){
            and1 = false;
            or1 = false;
            and2 = false;
            or2 = false;}
        // check if the word is literal
        thisAnd = (strcmp(andstr, result)==0);
		thisOr = (strcmp(orstr, result)==0);

        counters_t* wordCtr;
        // make sure the word exists in the index, if not skip
        if(!thisAnd && !thisOr) {
            wordCtr = index_find(fileIdx, result);
            if(wordCtr == NULL) {
                free(result);
                continue;
            }
        }
   
        // if the word is and
        if (thisAnd) {
            to_merge = false;
        } 
        // if the word is or
        if (thisOr) {
            to_merge = true;
            query_merge(resultCtrs, temp);
            counters_delete(temp);
            temp = counters_new();
        } 
        // if is other word
        if (!thisAnd && !thisOr){
            if (to_merge) {
                query_merge(temp, wordCtr);
                to_merge = false;
            } else {
                counters_t* intersect = query_intersect(temp, wordCtr);
                counters_delete(temp);
                temp = intersect;
            }
        }
        
        //reset the flags 
        thisOr = false;
        thisAnd = false;

		free(result);
	}
    query_merge(resultCtrs, temp);
    counters_delete(temp);
   // free(temp); 

    return resultCtrs;
}


/* ******************* prompt  ******************* */
/* print a prompt for an interactive user 
 * (when stdin is a "tty", aka teletype, aka keyboard)
 */
static void
prompt(void)
{
  // print a prompt iff stdin is a tty (terminal)
  if (isatty(fileno(stdin))) {
    printf("Query? ");
  }
}

/* ******************* buildDocs ******************* */
/* Build the array of document structure from a counter's object
 * takes advantage of counters_iterate.
 *
 * the array is updated with every document id and score.
 * 
 * We return nothing; error will be printed.
 */
static void 
buildDocs(counters_t* ctr, document_t** docArray, const int docCount) 
{
    if (ctr == NULL || docArray == NULL) {
        fprintf(stderr, "buildDocs null input\n");
        return;
    } 

    counters_iterate(ctr, docArray, insertDoc);
    build_docs_helper(docArray, docCount);

}

/* ******************* insertDoc ******************* */
/* helper function to buildDocs, simply insert the given counters node into
 * the array of document structure as a document type.
 */
static void
insertDoc(void* arg, const int key, const int count) 
{
    // cast type
    document_t** arr = (document_t**) arg;
    // loop to empty slot
    int i;
    for (i = 0; arr[i]->id != 0; i++){
    }
    // insert the key
    arr[i]->id = key;
    arr[i]->score = count;
}


/* ******************* build_docs_helper ******************* */
/* helper for buildDocs function.
 *
 * insert sort algorithm applied here to sort the order of the array
 * in descending order.
 */
static void
build_docs_helper(document_t** docArray, const int slotnum) {

    document_t* current;
    // start from index one
    for (int i = 1; i < slotnum; i++){

        current = docArray[i];
        int j = i-1;

        // if score bigger than current node, move all nodes back by one
        // then insert at the front of them
        while (j >= 0 && (current->score > docArray[j]->score)) {
            docArray[j+1] = docArray[j];
            j--;
        }
        docArray[j+1] = current;
    }

}

/* ******************* printDocs  ******************* */
static void
printDocs(document_t** docArray, const int docCount, const char* pageDirectory) 
{
    // check input
    if (docArray == NULL) {
        fprintf(stderr, "null docArray for printing\n");
        return;
    }
    // loop through the array, pull out URL by fetching the docID into a webpage structure
    for (int i = 0; i < docCount; i++) {
        webpage_t* page = pagedir_fetch(pageDirectory, docArray[i]->id);
        printf("score  %d doc  %d: %s\n", docArray[i]->score, docArray[i]->id, webpage_getURL(page));
        webpage_delete(page);
    }
}


/* ******************* indexRead ******************* */
/* read the destination file into an index structure
 * we assume the file is produced by the indexer earlier
 * 
 * codes adopted from lab5 indexer indexteset 
 */
static 
index_t* indexRead(const char* indexFilename)
{
    FILE* fp;
    char* result;
    char* key;
    int count;
    int docID;

    if (indexFilename == NULL) {
        fprintf(stderr, "empty input to read index\n");
        return NULL;
    }

    index_t* idxer = index_new();
    // open index file
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
            j++;
        }
        free(key1);
        free(result);
    }
    fclose(fp);
    return idxer;
}

/*************** document_new ****************/
/* create empty document structure to populate the initialized 
 * array of documents.
 *
 * needs to free later.
 */
static document_t*
document_new(void) {
	document_t* doc = malloc(sizeof(document_t));
	if (doc!=NULL) {
		doc->id = 0;
		doc->score=0;
	}
	return doc;
}

