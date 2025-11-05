#ifndef DARRAY_H
#define DARRAY_H

#include <stdbool.h>
#include "status.h"

typedef struct darray darray_t;

/**
 * @brief Create a new dynamic array (vector).
 *
 * @param array A pointer to darray_t* where the new array will be stored.
 * @param initial_capacity The initial number of elements the array will
 * allocate memory for. If 0, a default value will be used.
 *
 * @returns DSA_STATUS_SUCCESS on success, or an error code.
 * @complexity O(capacity) - due to malloc (or O(1) if malloc is optimized)
 */
dsa_status_t darray_create(darray_t **array, int initial_capacity);

/**
 * @brief Destroy the dynamic array, freeing all allocated memory.
 *
 * @param array Pointer to the array.
 * @complexity O(1)
 */
void darray_destroy(darray_t *array);

/**
 * @brief Add an element to the end of the array.
 *
 * If the capacity is insufficient, the array will be automatically
 * resized (realloc).
 *
 * @param array Pointer to the array.
 * @param data  The value to add.
 *
 * @returns DSA_STATUS_SUCCESS on success, or an error code.
 * @complexity O(1) - on average (amortized).
 * O(n) - in the worst case (when realloc is needed).
 */
dsa_status_t darray_push_back(darray_t *array, int data);

/**
 * @brief Remove the last element from the array.
 *
 * @param array Pointer to the array.
 *
 * @returns DSA_STATUS_SUCCESS on success, or an error code.
 * @complexity O(1)
 */
dsa_status_t darray_pop_back(darray_t *array);

/**
 * @brief Insert an element at a specific position.
 *
 * All elements from 'index' onwards will be shifted to the right.
 *
 * @param array Pointer to the array.
 * @param index The index to insert at (0 <= index <= size).
 * @param data  The value to insert.
 *
 * @returns DSA_STATUS_SUCCESS on success, or an error code.
 * @complexity O(n) - due to shifting elements (memmove).
 */
dsa_status_t darray_insert_at(darray_t *array, int index, int data);

/**
 * @brief Remove an element from a specific position.
 *
 * All elements to the right of 'index' will be shifted to the left.
 *
 * @param array Pointer to the array.
 * @param index The index to remove from (0 <= index < size).
 *
 * @returns DSA_STATUS_SUCCESS on success, or an error code.
 * @complexity O(n) - due to shifting elements (memmove).
 */
dsa_status_t darray_remove_at(darray_t *array, int index);

/**
 * @brief Get the element at a specific index.
 *
 * @param array Pointer to the array.
 * @param index The element's index.
 * @param data  A pointer (out parameter) where the value will be stored.
 *
 * @returns DSA_STATUS_SUCCESS on success, or DSA_STATUS_OUT_OF_RANGE.
 * @complexity O(1)
 */
dsa_status_t darray_get_at(darray_t *array, int index, int *data);

/**
 * @brief Reserve space (increase capacity).
 *
 * Guarantees that the array can hold at least 'new_capacity' elements
 * without further reallocations.
 *
 * @param array Pointer to the array.
 * @param new_capacity The new minimum capacity.
 *
 * @returns DSA_STATUS_SUCCESS on success, or an error code.
 * @complexity O(n) - due to realloc
 */
dsa_status_t darray_reserve(darray_t *array, int new_capacity);

/**
 * @brief Clear the array (remove all elements).
 * The capacity of the array remains unchanged.
 *
 * @param array Pointer to the array.
 * @complexity O(1)
 */
dsa_status_t darray_clear(darray_t *array);

/**
 * @brief Check if the array is empty.
 *
 * @returns true if size == 0, false otherwise.
 * @complexity O(1)
 */
bool darray_is_empty(darray_t *array);

/**
 * @brief Get the current number of elements in the array.
 *
 * @returns The number of elements (size).
 * @complexity O(1)
 */
int darray_get_size(darray_t *array);

/**
 * @brief Get the current capacity of the array.
 *
 * @returns The capacity.
 * @complexity O(1)
 */
int darray_get_capacity(darray_t *array);

#endif // DARRAY_H