#ifndef SEARCH_H
#define SEARCH_H

#include "hashmap.h"
#include "llist.h"

hashmap_t * training(glob_t files, int numBuckets);
void stop_word(hashmap_t * hm, unsigned int numFiles);
llist_t * read_query(char * query);
void rank(hashmap_t * hm, llist_t * query, glob_t files);

#endif
