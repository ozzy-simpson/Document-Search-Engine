#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashmap.h"

// Allocate a new HashMap with the specified number of buckets.
hashmap_t * hm_create(int num_buckets){
    //create the hashmap
    hashmap_t * hm = calloc(1, sizeof(hashmap_t ));
    hm->num_buckets = num_buckets;

    //create the buckets
    hm->map = calloc(hm->num_buckets, sizeof(struct wordnode*));
    int i;
    for(i = 0; i < hm->num_buckets; i++){
        hm->map[i] = NULL;
    }

    return hm;
}

// Return the number of documents a given word is in. If the element is not found, return -1.
int hm_get_documents(hashmap_t * hm, char* word){
    //get the hash value
    int h = hash_code(hm, word);

    //get the bucket
    struct wordnode * bucket = hm->map[h];

    //search the bucket for the word
    while(bucket != NULL){
        if(strcmp(bucket->word, word) == 0) {
            return bucket->num_documents;
        }
        bucket = bucket->next;
    }

    return -1;
}

// Return the number of times a given word is in a given document. If the element is not found, return -1.
int hm_get(hashmap_t * hm, char* word, char* document_id){
    //get the hash value
    int h = hash_code(hm, word);

    //get the bucket
    struct wordnode * bucket = hm->map[h];

    //search the bucket for the word
    while(bucket != NULL){
        if(strcmp(bucket->word, word) == 0) {
            struct llnode * node = bucket->documents;
            // search the linked list for the document
            while(node != NULL){
                if(strcmp(node->document_id, document_id) == 0){
                    return node->num_occurrences;
                }
                node = node->next;
            }
        }
        bucket = bucket->next;
    }

    return -1;
}

// Put the key value pair into the HashMap that is passed in. If the word and document id combination already exists within the HashMap, its value will be overwritten with the new one.
void hash_table_insert(hashmap_t * hm, char* word, char* document_id){
    //get the hash value
    int h = hash_code(hm, word);

    //get the bucket
    struct wordnode * bucket = hm->map[h];

    //search the bucket for the word; update num_occurrences if found
    while(bucket != NULL){
        if(strcmp(bucket->word, word) == 0){
            // Word in hashmap. Now iterate through the document_id list to see if the document_id is already in the list
            struct llnode * doc = bucket->documents;
            while(doc != NULL){
                if(strcmp(doc->document_id, document_id) == 0){
                    // Document id already in list. Update num_occurrences
                    doc->num_occurrences++;
                    return;
                }
                doc = doc->next;
            }
            // Word in hashmap, but document id not in list. Add document id to list
            struct llnode * new_doc = calloc(1, sizeof(struct llnode));
            new_doc->document_id = strdup(document_id);
            new_doc->num_occurrences = 1;
            new_doc->next = bucket->documents;
            bucket->documents = new_doc;
            bucket->num_documents++;
            return;
        }
        bucket = bucket->next;
    }

    //if the word is not in the bucket, add it
    struct wordnode * new_node = calloc(1, sizeof(struct wordnode));
    new_node->word = strdup(word);
    new_node->num_documents = 1;
    new_node->next = hm->map[h];

    // Add the document id to the document id list
    struct llnode * new_doc = calloc(1, sizeof(struct llnode));
    new_doc->document_id = strdup(document_id);
    new_doc->num_occurrences = 1;
    new_doc->next = NULL;

    new_node->documents = new_doc;
    hm->map[h] = new_node;

    return;
}

// Remove a word in the HashMap that is associated with the given key.
void hm_remove(hashmap_t * hm, char* word){
    //get the hash value
    int h = hash_code(hm, word);

    //get the bucket
    struct wordnode * bucket = hm->map[h];

    //search the bucket for the word; remove it if found
    struct wordnode * prev = NULL;
    while(bucket != NULL){
        if(strcmp(bucket->word, word) == 0){
            if(prev == NULL){
                hm->map[h] = bucket->next;
            } else {
                prev->next = bucket->next;
            }
            free(bucket->word);

            // Free the document id list
            struct llnode * cur_doc = NULL;
            while(bucket->documents){
                cur_doc = bucket->documents;
                bucket->documents = bucket->documents->next;
                free(cur_doc->document_id);
                free(cur_doc);
            }

            free(bucket);
            return;
        }
        prev = bucket;
        bucket = bucket->next;
    }

    return;
}

// Deallocate (free) the HashMap that is passed in and all of its elements.
void hm_destroy(hashmap_t * hm){
    int i;
    for(i=0; i<hm->num_buckets; i++){
        struct wordnode * cur = hm->map[i];
        while(hm->map[i]){
            cur = hm->map[i];
            hm->map[i] = cur->next;

            // Free the document id list
            struct llnode * cur_doc = cur->documents;
            while(cur->documents){
                cur_doc = cur->documents;
                cur->documents = cur->documents->next;
                free(cur_doc->document_id);
                free(cur_doc);
            }

            //free the node
            free(cur->word); //free's from strdup() 
            free(cur);
        }
    }
    free(hm->map);

    //delete the hashmap
    free(hm);

    return;
}

// sum the ASCII codes of all of the characters then modulo by the number of buckets.
int hash_code(hashmap_t * hm, char* w){
    int sum = 0;
    size_t i;
    for (i = 0; i < strlen(w); i++) {
        sum += w[i];
    }
    return sum % hm->num_buckets;
}

// Print the HashMap that is passed in.
void hm_print(hashmap_t * hm){
    int i;
    for(i=0; i<hm->num_buckets; i++){
        struct wordnode * cur = hm->map[i];
        while(cur){
            printf("Bucket %d: %s (%d documents)\n", i, cur->word, cur->num_documents);
            struct llnode * doc = cur->documents;
            while(doc){
                printf("\t%s: %d\n", doc->document_id, doc->num_occurrences);
                doc = doc->next;
            }
            cur = cur->next;
        }
    }
    return;
}
