#ifndef DLIST_H
#define DLIST_H

#include <stdbool.h>
#include "status.h"

typedef struct dlist dlist_t;

/**
 * @brief Create a new doubly linked list
 *
 * The function is responsible for allocating the memory
 * To free the memory, please use dlist_destroy()
 *
 * @param list Pointer to a list object that will be created
 *
 * @returns DSA_STATUS_SUCCESS on success, a failure status on error
 * @complexity O(1)
 */
dsa_status_t dlist_create(dlist_t **list);

/**
 * @brief Destroy the doubly linked list, freeing the memory
 *
 * Use this API only on lists that were allocated with dlist_create()
 *
 * @param list Pointer to a list object
 * @complexity O(n)
 */
void dlist_destroy(dlist_t *list);

/**
 * @brief Insert data in front of the list, changing the head
 *
 * @param list Pointer to the list object
 * @param data Data value
 *
 * @returns DSA_STATUS_SUCCESS on success, a failure status on error
 * @complexity O(1)
 */
dsa_status_t dlist_insert_front(dlist_t *list, int data);

/**
 * @brief Append data to the back of the list
 *
 * @param list Pointer to the list object
 * @param data Data value
 *
 * @returns DSA_STATUS_SUCCESS on success, a failure status on error
 * @complexity O(1)
 */
dsa_status_t dlist_insert_back(dlist_t *list, int data);

/**
 * @brief Insert data at the given index
 *
 * If index == 0, the inserted node becomes the head
 * If index == list length, the data is appended to the back of the list
 * If index is out of range, an error is returned
 *
 * @param list  Pointer to a list object
 * @param index List index to insert at
 * @param data  Data value
 *
 * @returns DSA_STATUS_SUCCESS on success, a failure status on error
 * @complexity O(n) - in worst case (insertion in the middle)
 * O(1) - for index 0 or index == length
 */
dsa_status_t dlist_insert_at_index(dlist_t *list, int index, int data);

/**
 * @brief Find the first node containing the given data
 *
 * @param list Pointer to the list object
 * @param data Data value
 *
 * @returns Index of the first found node or -1 if not found
 * @complexity O(n)
 */
int dlist_find(dlist_t *list, int data);

/**
 * @brief Remove the node at the given index
 *
 * @param list  Pointer to the list object
 * @param index List index to remove from
 *
 * @returns DSA_STATUS_SUCCESS on success, a failure status on error
 * @complexity O(n) - in worst case
 * O(1) - for index 0 or index == length - 1
 */
dsa_status_t dlist_remove(dlist_t *list, int index);

/**
 * @brief Remove the node from the back of the list
 *
 * @param list  Pointer to the list object
 *
 * @returns DSA_STATUS_SUCCESS on success, a failure status on error
 * @complexity O(1)
 */
dsa_status_t dlist_pop_back(dlist_t *list);

/**
 * @brief Remove the node from the front of the list
 *
 * @param list  Pointer to the list object
 *
 * @returns DSA_STATUS_SUCCESS on success, a failure status on error
 * @complexity O(1)
 */
dsa_status_t dlist_pop_front(dlist_t *list);

/**
 * @brief Remove all added data from the list
 *
 * @param list  Pointer to the list object
 *
 * @returns DSA_STATUS_SUCCESS on success, a failure status on error
 * @complexity O(n)
 */
dsa_status_t dlist_clear(dlist_t *list);

/**
 * @brief Check if list is empty
 *
 * @param list  Pointer to the list object
 *
 * @returns true if the list is empty, false otherwise
 * @complexity O(1)
 */
bool dlist_is_empty(dlist_t *list);

/**
 * @brief Return the length of the list
 *
 * @param list  Pointer to the list object
 *
 * @returns A length of the given list
 * @complexity O(1)
 */
int dlist_len(dlist_t *list);

#endif // DLIST_H