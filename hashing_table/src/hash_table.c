#include "hash_table.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct hash_table_node
{
    void *entry;
    struct hash_table_node *next;
} hash_table_node_t;

struct hash_table
{
    unsigned int capacity;
    hash_table_hash_func hash_func;
    hash_table_compare_func compare_func;
    hash_table_node_t **buckets;
};


static unsigned int _hash_table_get_index(hash_table_t *table, void *entry) {
    return (unsigned int)table->hash_func(entry) % table->capacity;
}

dsa_status_t hash_table_create(unsigned int initial_capacity,hash_table_hash_func hash_func,hash_table_compare_func compare_func,hash_table_t **table) {
    if (initial_capacity == 0 || !hash_func || !compare_func || !table) {
        return DSA_BAD_PARAM;
    }

    hash_table_t *new_table = malloc(sizeof(hash_table_t));
    if (!new_table) {
        return DSA_ERROR;
    }

    new_table->buckets = calloc(initial_capacity, sizeof(hash_table_node_t *));
    if (!new_table->buckets) {
        free(new_table);
        return DSA_ERROR;
    }

    new_table->capacity = initial_capacity;
    new_table->hash_func = hash_func;
    new_table->compare_func = compare_func;

    *table = new_table;
    return DSA_OK;
}

dsa_status_t hash_table_destroy(hash_table_t *table) {
    if (!table) {
        return DSA_BAD_PARAM;
    }

    for (unsigned int i = 0; i < table->capacity; i++) {
        hash_table_node_t *current = table->buckets[i];

        while (current != NULL) {
            hash_table_node_t *to_free = current;
            current = current->next;
            free(to_free);
        }
    }

    free(table->buckets);
    free(table);

    return DSA_OK;
}

dsa_status_t hash_table_insert(hash_table_t *table, void *entry) {
    if (!table || !entry) {
        return DSA_BAD_PARAM;
    }

    if (hash_table_find(table, entry) != NULL) {
        return DSA_EXISTS;
    }

    unsigned int index = _hash_table_get_index(table, entry);

    hash_table_node_t *new_node = malloc(sizeof(hash_table_node_t));
    if (!new_node) {
        return DSA_ERROR;
    }
    new_node->entry = entry;

    new_node->next = table->buckets[index];
    table->buckets[index] = new_node;

    return DSA_OK;
}

void *hash_table_find(hash_table_t *table, void *entry) {
    if (!table || !entry) {
        return NULL;
    }

    unsigned int index = _hash_table_get_index(table, entry);

    hash_table_node_t *current = table->buckets[index];
    while (current != NULL) {
        if (table->compare_func(current->entry, entry)) {
            return current->entry;
        }
        current = current->next;
    }

    return NULL;
}

dsa_status_t hash_table_remove(hash_table_t *table, void *entry) {
    if (!table || !entry) {
        return DSA_BAD_PARAM;
    }

    unsigned int index = _hash_table_get_index(table, entry);

    hash_table_node_t *current = table->buckets[index];
    hash_table_node_t *prev = NULL;

    while (current != NULL) {
        if (table->compare_func(current->entry, entry)) {
            break;
        }
        prev = current;
        current = current->next;
    }

    if (current == NULL) {
        return DSA_NOT_FOUND;
    }

    if (prev == NULL) {
        table->buckets[index] = current->next;
    } else {
        prev->next = current->next;
    }

    free(current);
    return DSA_OK;
}

dsa_status_t hash_table_foreach(hash_table_t *table, hash_table_entry_action entry_action) {
    if (!table || !entry_action) {
        return DSA_BAD_PARAM;
    }

    for (unsigned int i = 0; i < table->capacity; i++) {
        hash_table_node_t *current = table->buckets[i];
        while (current != NULL) {
            entry_action(current->entry);
            current = current->next;
        }
    }

    return DSA_OK;
}