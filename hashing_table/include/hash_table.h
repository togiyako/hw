#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdbool.h>
#include <stdlib.h>
#include "status.h"

/**
 * @brief Opaque declaration of the hash table structure.
 * The user does not have direct access to the fields of this structure.
 * Interaction is only possible through the provided API functions.
 */
typedef struct hash_table hash_table_t;

/**
 * @brief Pointer to a function that computes the hash for an entry.
 * @param entry Pointer to user data.
 * @return An integer hash value.
 */
typedef int  (*hash_table_hash_func)(void *entry);

/**
 * @brief Pointer to a function that compares two entries.
 * @param entry1 Pointer to the first entry.
 * @param entry2 Pointer to the second entry.
 * @return true if the entries are considered equal, false otherwise.
 */
typedef bool (*hash_table_compare_func)(void *entry1, void *entry2);

/**
 * @brief Pointer to a function that performs an action on an entry (for hash_table_foreach).
 * @param entry Pointer to user data.
 */
typedef void (*hash_table_entry_action)(void *entry);

/**
 * @brief Creates a new hash table instance.
 *
 * @param initial_capacity The initial number of buckets.
 * @param hash_func The function to compute the hash.
 * @param compare_func The function to compare two entries.
 * @param table A pointer to a pointer where the newly created table will be stored.
 * @returns DSA_OK on success, or an error code.
 */
dsa_status_t hash_table_create(unsigned int initial_capacity, 
                               hash_table_hash_func hash_func, 
                               hash_table_compare_func compare_func, 
                               hash_table_t **table);

/**
 * @brief Destroys the hash table and frees all associated memory.
 * @note This function does NOT free the memory allocated for user data (entry).
 * The user is responsible for freeing this data independently (e.g.,
 * by using hash_table_foreach before destruction).
 *
 * @param table Pointer to the hash table to be destroyed.
 * @returns DSA_OK on success, or DSA_BAD_PARAM if table == NULL.
 */
dsa_status_t hash_table_destroy(hash_table_t *table);

/**
 * @brief Inserts a new entry into the hash table.
 * @note Does not allow inserting duplicates (where compare_func returns true).
 * @param table Pointer to the hash table.
 * @param entry Pointer to the user data to be inserted.
 * @returns DSA_OK on successful insertion.
 * @returns DSA_EXISTS if such an element already exists.
 * @returns DSA_BAD_PARAM if table or entry == NULL.
 * @returns DSA_ERROR on a memory allocation error.
 */
dsa_status_t hash_table_insert(hash_table_t *table, void *entry);

/**
 * @brief Removes an entry from the hash table.
 * @note This function only frees the table node, NOT the user data (entry).
 * @param table Pointer to the hash table.
 * @param entry Pointer to the entry (or key) to be removed.
 * Uses compare_func for searching.
 * @returns DSA_OK on successful removal.
 * @returns DSA_NOT_FOUND if the element is not found.
 * @returns DSA_BAD_PARAM if table or entry == NULL.
 */
dsa_status_t hash_table_remove(hash_table_t *table, void *entry);

/**
 * @brief Finds an entry in the hash table.
 *
 * @param table Pointer to the hash table.
 * @param entry Pointer to the entry (or key) to be found.
 * Uses compare_func for searching.
 * @returns A pointer to the found entry in the table if found.
 * @returns NULL if the entry is not found, or in case of an error (table/entry == NULL).
 */
void *hash_table_find(hash_table_t *table, void *entry);

/**
 * @brief Executes an action (entry_action) for each entry in the table.
 *
 * @param table Pointer to the hash table.
 * @param entry_action The function that will be called for each entry.
 * @returns DSA_OK on success.
 * @returns DSA_BAD_PARAM if table or entry_action == NULL.
 */
dsa_status_t hash_table_foreach(hash_table_t *table, hash_table_entry_action entry_action);


#endif // HASH_TABLE_H