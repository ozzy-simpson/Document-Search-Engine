#ifndef __SCORES_H__
#define __SCORES_H__

#include <stdio.h>

//node type stored in lists
typedef struct score_node{
    struct score_node * next; //next node in list
    char * file; //string value stored in list
    double score;
} score_t;

//list_t struct to store a list
typedef struct scores{
    score_t * head; //pointer to the node at the head of the list
    int size; //the number of nodes in the list
} scores_t;

// Return a newly initialized, empty linked list
scores_t * scores_init(void);

//delete/deallocate a linked list
void scores_delete(scores_t * s);

//insert the file's score into the list, in descendingorder
void scores_push(scores_t * s, char * f, double score);

//print the list
void scores_print(scores_t * s);

//saves the list to a file
void scores_save(scores_t * s);

#endif
