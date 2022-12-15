#ifndef HASHMAP_H
#define HASHMAP_H

struct llnode {
    char* document_id;
    int num_occurrences;
    struct llnode* next;
};

struct wordnode {
    char* word;
    int num_documents;
    struct llnode* documents;
    struct wordnode* next;
};

typedef struct hashmap {
    struct wordnode** map;
    int num_buckets;
} hashmap_t;


hashmap_t * hm_create(int num_buckets);
int hm_get_documents(hashmap_t * hm, char* word);
int hm_get(hashmap_t * hm, char* word, char* document_id);
void hash_table_insert(hashmap_t * hm, char* word, char* document_id);
void hm_remove(hashmap_t * hm, char* word);
void hm_destroy(hashmap_t * hm);
int hash_code(hashmap_t * hm, char* w);
void hm_print(hashmap_t * hm);

#endif
