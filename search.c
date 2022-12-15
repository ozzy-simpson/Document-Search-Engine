#include <glob.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "search.h"
#include "scores.h"

#define MAX_BUFFER 1024

int main(void){
    char buffer[MAX_BUFFER];

    // Ask the user for the number of buckets to be used in the hashmap
    int numBuckets = 0;
    printf("How many buckets?\n");
    fgets(buffer, MAX_BUFFER, stdin);
    sscanf(buffer, "%d", &numBuckets);

    if (numBuckets <= 0) {
        fprintf(stderr, "Invalid number of buckets. Please try again with a valid (positive) number of buckets.\n");
        exit(1);
    }

    // Ask the user for the directory to be searched
    char search_string[MAX_BUFFER];
    glob_t result;
    printf("Enter a filename search string:\n");
    fgets(search_string, MAX_BUFFER, stdin);
    search_string[strlen(search_string)-1] = '\0'; // add null terminator
    int retval = glob(search_string, 0, NULL, &result);
    if (retval != 0) {
        if (retval == GLOB_NOMATCH) {
            fprintf(stderr, "No filename matches\n");
        }
        else {
            fprintf(stderr, "Glob error\n");
        }
        exit(1);
    }

    // Run the training algorithm
    hashmap_t * hm = training(result, numBuckets);

    // Ask the user for the search query. If the user enters X, exit the program. Words can be separated by spaces and up to 20 characters each
    while (1) {
        char query[MAX_BUFFER];
        printf("Enter a search query (X to exit):\n");
        fgets(query, MAX_BUFFER, stdin);
        if (query[0] == 'X') {
            break;
        }
        query[strlen(query)-1] = '\0';
        llist_t * words = read_query(query);
        rank(hm, words, result);
        ll_delete(words);
    }

    // Free the hashmap
    hm_destroy(hm);

    // Free the glob_t structure
    globfree(&result);

    return 0;
}

hashmap_t * training(glob_t files, int numBuckets) {
    unsigned int i;

    // Create the hashmap
    hashmap_t * hm = hm_create(numBuckets);

    // Loop through all the files and add every word to the hashmap
    for (i = 0; i < files.gl_pathc; i++) {
        FILE * fp = fopen(files.gl_pathv[i], "r");
        if (fp == NULL) {
            fprintf(stderr, "Error opening file %s", files.gl_pathv[i]);
            exit(1);
        }

        // Read the file one word at a time
        char word[21]; // 20 characters + null terminator
        while(fscanf(fp, "%s", word) != EOF){
            // Insert word to hashmap
            hash_table_insert(hm, word, files.gl_pathv[i]);
        }
        fclose(fp);
    }

    // Remove words that appear in all documents
    stop_word(hm, files.gl_pathc);

    return hm;
}

// Removes words that appear in all of the documents
void stop_word(hashmap_t * hm, unsigned int numFiles) {
    int i;
    // Loop through all the buckets
    for (i = 0; i < hm->num_buckets; i++) {
        struct wordnode * bucket = hm->map[i];
        struct wordnode * prev = NULL;
        // Loop through all the words in the bucket
        while (bucket) {
            prev = bucket;
            bucket = bucket->next;
            // If the word appears in all the documents, remove it from the hashmap
            if (prev->num_documents == (int) numFiles) {
                hm_remove(hm, prev->word);
            }

        }
    }
}

// Reads a query and returns as a linked list of words
llist_t * read_query(char * query) {
    // Split the query into words. Store in a llist
    llist_t * words = ll_init();
    char * word = strtok(query, " ");
    while (word != NULL) {
        ll_push(words, word);
        word = strtok(NULL, " ");
    }

    return words;
}
    
// Ranks the documents based on the query
void rank(hashmap_t * hm, llist_t *query, glob_t files) {
    // Create a list of scores
    scores_t * scores = scores_init();
    unsigned int i;

    int count = 0; // counts non-zero scores

    // Loop through each document
    for (i = 0; i < files.gl_pathc; i++) {
        double score = 0;
        // Loop through each word in the query
        ll_node_t * node = query->head;
        while (node != NULL) {
            // Calculate term frequency
            int tf = hm_get(hm, node->val, files.gl_pathv[i]);
            if (tf == -1) {
                tf = 0;
            }
    
            // Calculate document frequency
            int df = hm_get_documents(hm, node->val);
            if (df <= 0) {
                df = 1;
            }
            // Calculate inverse document frequency
            double idf = log((double) files.gl_pathc / (double) df);
    
            // Add tf-idf
            score += tf * idf;
            node = node->next;
        }
        scores_push(scores, files.gl_pathv[i], score);
        if (score > 0) {
            count++;
        }
    }
    if (count == 0) {
        // No documents were found with the query
        printf("No documents found\n");
    }
    else {
        scores_print(scores); // Print the scores
    }
    scores_save(scores); // Save the scores
    scores_delete(scores);
}
