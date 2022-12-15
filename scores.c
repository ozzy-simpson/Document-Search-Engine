#include <stdlib.h>
#include <string.h>

#include "scores.h"

//initialize score list
scores_t * scores_init(void) {
    scores_t * s = malloc(sizeof(scores_t));
    s->size = 0; //initial size is 0
    s->head = NULL; //the head is null
    return s; 
}

//delete score list
void scores_delete(scores_t * s){
    score_t * cur = s->head;
    while(s->head){
        cur = s->head;
        s->head = cur->next;

        //free the node
        free(cur->file); //free's from strdup()    
        free(cur);

        //reduce the size
        s->size--;
    }    
    free(s); //free the llist itself
}


// add a score, in descending order
void scores_push(scores_t * s, char * f, double score){
    //allocate
    score_t * n = malloc(sizeof(score_t));

    n->file = strdup(f); //duplicate the string
    n->score = score;

    //add to list in descending order
    if(s->head == NULL){
        s->head = n;
        n->next = NULL;
    } else {
        score_t * cur = s->head;
        score_t * prev = NULL;
        while(cur && cur->score > score){
            prev = cur;
            cur = cur->next;
        }
        if(prev == NULL){
            n->next = s->head;
            s->head = n;
        } else {
            n->next = cur;
            prev->next = n;
        }
    }

    s->size++; //increase size
}

//print the score list
void scores_print(scores_t * s) {
    score_t * cur = s->head;
    while(cur){
        if (cur->score > 0) {
            // Only print non-zero scores
            printf("%s\n", cur->file);
        }
        cur = cur->next;
    }
}

//save the list to a file
void scores_save(scores_t * s) {
    FILE * fp = fopen("search_scores.txt", "a");
    score_t * cur = s->head;
    while(cur){
        fprintf(fp, "%s:\t%f\n", cur->file, cur->score);
        cur = cur->next;
    }
    fclose(fp);
}
