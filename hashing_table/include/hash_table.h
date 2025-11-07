#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdbool.h>
#include <stdlib.h>
#include "status.h"

typedef struct hash_table hash_table_t;

typedef int  (*hash_table_hash_func)(void *entry);
typedef bool (*hash_table_compare_func)(void *entry1, void *entry2);
typedef void (*hash_table_entry_action)(void *entry);

dsa_status_t hash_table_create(unsigned initial_capacity, hash_table_hash_func hash_func, hash_table_compare_func compare_func, hash_table_t **table);
dsa_status_t hash_table_destroy(hash_table_t *table);

// `entry` is a pointer to an already created entry
// do not allow duplicate entries (i.e. where hash_table_compare_func() == true)
dsa_status_t hash_table_insert(hash_table_t *table, void *entry);
dsa_status_t hash_table_remove(hash_table_t *table, void *entry);

// looks up if `entry` is present in the hash table according to the user-defined hash_table_compare_func
// returns a pointer to the entry if found
// NULL if not found
void *hash_table_find(hash_table_t *table, void *entry);

// executes entry_action for each entry in the table
dsa_status_t hash_table_foreach(hash_table_t *table, hash_table_entry_action entry_action);

typedef struct hash_table_node
{
    void *entry;
    struct hash_table_node *next;
}hash_table_node_t;

struct hash_table
{
    unsigned int capacity;
    hash_table_hash_func hash_func;
    hash_table_compare_func compare_func;
    hash_table_node_t **buckets;
};

#endif