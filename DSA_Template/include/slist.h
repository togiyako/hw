#ifndef LIST_H
#define LIST_H

#include <stdbool.h>
#include "status.h"

typedef struct slist slist_t;

/**
 * @brief Create a new singly linked list
 *
 * The function is responsible for allocating the memory
 * To free the memory, please use slist_destroy()
 *
 * @param list Pointer to a list object that will be created
 *
 * @returns DSA_STATUS_SUCCESS on success, a failure status on error
 */
dsa_status_t slist_create(slist_t **list);

/**
 * @brief Destroy the singly linked list, freeing the memory
 *
 * Use this API only on lists that were allocated with slist_create()
 *
 * @param list Pointer to a list object
 *
 */
void slist_destroy(slist_t *list);

/**
 * @brief Insert data in front of the list, changing the head
 *
 * @param list Pointer to the list object
 * @param data Data value
 *
 * @returns DSA_STATUS_SUCCESS on success, a failure status on error
 */
dsa_status_t slist_insert_front(slist_t *list, int data);

/**
 * @brief Append data to the back of the list
 *
 * @param list Pointer to the list object
 * @param data Data value
 *
 * @returns DSA_STATUS_SUCCESS on success, a failure status on error
 */
dsa_status_t slist_insert_back(slist_t *list, int data);

/**
 * @brief Insert data at the given index
 *
 * If index == 0, the inserted node becomes the head
 * If index == list length, the data is appended to the back of the list
 * If index is out of range, an error is returned
 * Otherwise the data is inserted before the node that is currently at the given index
 *
 * @param list  Pointer to a list object
 * @param index List index to insert at
 * @param data  Data value
 *
 * @returns DSA_STATUS_SUCCESS on success, a failure status on error
 */
dsa_status_t slist_insert_at_index(slist_t *list, int index, int data);

/**
 * @brief Find the first node containing the given data
 *
 * @param list Pointer to the list object
 * @param data Data value
 *
 * @returns Index of the first found node or -1 if not found
 */
int slist_find(slist_t *list, int data);

/**
 * @brief Remove the node at the given index
 *
 * @param list  Pointer to the list object
 * @param index List index to remove from
 *
 * @returns DSA_STATUS_SUCCESS on success, a failure status on error
 */
dsa_status_t slist_remove(slist_t *list, int index);

/**
 * @brief Remove the node from the back of the list
 *
 * @param list  Pointer to the list object
 *
 * @returns DSA_STATUS_SUCCESS on success, a failure status on error
 */
dsa_status_t slist_pop_back(slist_t *list);

/**
 * @brief Remove the node from the front of the list
 *
 * @param list  Pointer to the list object
 *
 * @returns DSA_STATUS_SUCCESS on success, a failure status on error
 */
dsa_status_t slist_pop_front(slist_t *list);

/**
 * @brief Remove all added data from the list
 *
 * @param list  Pointer to the list object
 *
 * @returns DSA_STATUS_SUCCESS on success, a failure status on error
 */
dsa_status_t slist_clear(slist_t *list);

/**
 * @brief Check if list is empty
 *
 * @param list  Pointer to the list object
 *
 * @returns true if the list is empty, false otherwise
 */
bool slist_is_empty(slist_t *list);

/**
 * @brief Returh the length of the list
 *
 * @param list  Pointer to the list object
 *
 * @returns A length of the given list
 */
int slist_len(slist_t *list);

#endif
