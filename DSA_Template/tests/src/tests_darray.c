#include "tests.h"
#include "darray.h"
#include "status.h"
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

// #define LARGE_ARRAY_SIZE 1000000 // !NE REKOMENDUIU!!
#define LARGE_ARRAY_SIZE 100000


#define DSA_ASSERT_STATUS(received_status, expected_status)                     \
  do {                                                                          \
    if (received_status != expected_status) {                                   \
      printf("Assertion failed: expected status %d, received %d at %s:%d\n",    \
             expected_status, received_status, __FILE__, __LINE__);             \
      return DSA_TEST_FAILURE;                                                  \
    }                                                                           \
  } while (0)

#define DSA_ASSERT_TRUE(condition)                                              \
  do {                                                                          \
    if (!(condition)) {                                                         \
      printf("Assertion failed: condition is false at %s:%d\n", __FILE__, __LINE__); \
      return DSA_TEST_FAILURE;                                                  \
    }                                                                           \
  } while (0)

#define DSA_ASSERT_ARRAY_VALUE(array, index, expected_value)                    \
  do {                                                                          \
    int value;                                                                  \
    DSA_ASSERT_STATUS(darray_get_at(array, index, &value), DSA_STATUS_SUCCESS); \
    if (value != expected_value) {                                              \
      printf("Assertion failed: array[%d] expected %d, received %d at %s:%d\n", \
             index, expected_value, value, __FILE__, __LINE__);                 \
      return DSA_TEST_FAILURE;                                                  \
    }                                                                           \
  } while(0)



static bool is_sorted(darray_t *arr) {
    for (int i = 0; i < darray_get_size(arr) - 1; i++) {
        int val1, val2;
        darray_get_at(arr, i, &val1);
        darray_get_at(arr, i + 1, &val2);
        if (val1 > val2) {
            printf("Sort failed: arr[%d]=%d > arr[%d]=%d\n", i, val1, i+1, val2);
            return false;
        }
    }
    return true;
}


static void populate_test_array_large(darray_t *arr, int size) {
    darray_clear(arr);
    dsa_status_t status = darray_reserve(arr, size);
    if (status != DSA_STATUS_SUCCESS) {
        printf("Failed to reserve memory for large array test!\n");
        return;
    }

    for (int i = 0; i < size; i++) {
        darray_push_back(arr, rand() % (size * 10) + 1);
    }
}


dsa_test_status_t test_darray_create_destroy() {
  printf("Running test_darray_create_destroy\n");
  darray_t *arr = NULL;

  DSA_ASSERT_STATUS(darray_create(&arr, 10), DSA_STATUS_SUCCESS);
  DSA_ASSERT_TRUE(arr != NULL);
  DSA_ASSERT_TRUE(darray_is_empty(arr));
  DSA_ASSERT_TRUE(darray_get_size(arr) == 0);
  DSA_ASSERT_TRUE(darray_get_capacity(arr) == 10);

  darray_destroy(arr);
  return DSA_TEST_SUCCESS;
}

dsa_test_status_t test_darray_push_pop() {
  printf("Running test_darray_push_pop\n");
  darray_t *arr = NULL;
  darray_create(&arr, 2);

  DSA_ASSERT_STATUS(darray_push_back(arr, 10), DSA_STATUS_SUCCESS);
  DSA_ASSERT_TRUE(darray_get_size(arr) == 1);
  DSA_ASSERT_ARRAY_VALUE(arr, 0, 10);

  DSA_ASSERT_STATUS(darray_push_back(arr, 20), DSA_STATUS_SUCCESS);
  DSA_ASSERT_TRUE(darray_get_size(arr) == 2);
  DSA_ASSERT_ARRAY_VALUE(arr, 1, 20);

  DSA_ASSERT_STATUS(darray_pop_back(arr), DSA_STATUS_SUCCESS);
  DSA_ASSERT_TRUE(darray_get_size(arr) == 1);
  DSA_ASSERT_STATUS(darray_get_at(arr, 1, NULL), DSA_STATUS_OUT_OF_RANGE);

  DSA_ASSERT_STATUS(darray_pop_back(arr), DSA_STATUS_SUCCESS);
  DSA_ASSERT_TRUE(darray_is_empty(arr));

  DSA_ASSERT_STATUS(darray_pop_back(arr), DSA_STATUS_FAILURE);

  darray_destroy(arr);
  return DSA_TEST_SUCCESS;
}

dsa_test_status_t test_darray_capacity_growth() {
  printf("Running test_darray_capacity_growth\n");
  darray_t *arr = NULL;
  darray_create(&arr, 1);
  
  DSA_ASSERT_TRUE(darray_get_capacity(arr) == 1);

  darray_push_back(arr, 10);
  DSA_ASSERT_TRUE(darray_get_size(arr) == 1);
  DSA_ASSERT_TRUE(darray_get_capacity(arr) == 1);

  darray_push_back(arr, 20);
  DSA_ASSERT_TRUE(darray_get_size(arr) == 2);
  DSA_ASSERT_TRUE(darray_get_capacity(arr) >= 2);

  darray_push_back(arr, 30);
  DSA_ASSERT_TRUE(darray_get_size(arr) == 3);
  DSA_ASSERT_TRUE(darray_get_capacity(arr) >= 3);
  
  DSA_ASSERT_ARRAY_VALUE(arr, 0, 10);
  DSA_ASSERT_ARRAY_VALUE(arr, 1, 20);
  DSA_ASSERT_ARRAY_VALUE(arr, 2, 30);

  darray_destroy(arr);
  return DSA_TEST_SUCCESS;
}

dsa_test_status_t test_darray_insert_at() {
  printf("Running test_darray_insert_at\n");
  darray_t *arr = NULL;
  darray_create(&arr, 5);

  darray_push_back(arr, 10);
  darray_push_back(arr, 20);
  darray_push_back(arr, 30);

  DSA_ASSERT_STATUS(darray_insert_at(arr, 1, 15), DSA_STATUS_SUCCESS);
  DSA_ASSERT_TRUE(darray_get_size(arr) == 4);
  DSA_ASSERT_ARRAY_VALUE(arr, 0, 10);
  DSA_ASSERT_ARRAY_VALUE(arr, 1, 15);
  DSA_ASSERT_ARRAY_VALUE(arr, 2, 20);
  DSA_ASSERT_ARRAY_VALUE(arr, 3, 30);

  DSA_ASSERT_STATUS(darray_insert_at(arr, 0, 5), DSA_STATUS_SUCCESS);
  DSA_ASSERT_TRUE(darray_get_size(arr) == 5);
  DSA_ASSERT_ARRAY_VALUE(arr, 0, 5);
  DSA_ASSERT_ARRAY_VALUE(arr, 1, 10);

  DSA_ASSERT_STATUS(darray_insert_at(arr, 5, 40), DSA_STATUS_SUCCESS);
  DSA_ASSERT_TRUE(darray_get_size(arr) == 6);
  DSA_ASSERT_ARRAY_VALUE(arr, 5, 40);
  DSA_ASSERT_TRUE(darray_get_capacity(arr) > 5);

  DSA_ASSERT_STATUS(darray_insert_at(arr, 99, 0), DSA_STATUS_OUT_OF_RANGE);
  
  darray_destroy(arr);
  return DSA_TEST_SUCCESS;
}

dsa_test_status_t test_darray_remove_at() {
  printf("Running test_darray_remove_at\n");
  darray_t *arr = NULL;
  darray_create(&arr, 5);

  darray_push_back(arr, 10);
  darray_push_back(arr, 20);
  darray_push_back(arr, 30);
  darray_push_back(arr, 40);
  darray_push_back(arr, 50);

  DSA_ASSERT_STATUS(darray_remove_at(arr, 2), DSA_STATUS_SUCCESS);
  DSA_ASSERT_TRUE(darray_get_size(arr) == 4);
  DSA_ASSERT_ARRAY_VALUE(arr, 2, 40);
  DSA_ASSERT_ARRAY_VALUE(arr, 3, 50);

  DSA_ASSERT_STATUS(darray_remove_at(arr, 0), DSA_STATUS_SUCCESS);
  DSA_ASSERT_TRUE(darray_get_size(arr) == 3);
  DSA_ASSERT_ARRAY_VALUE(arr, 0, 20);

  DSA_ASSERT_STATUS(darray_remove_at(arr, 2), DSA_STATUS_SUCCESS);
  DSA_ASSERT_TRUE(darray_get_size(arr) == 2);
  DSA_ASSERT_STATUS(darray_get_at(arr, 2, NULL), DSA_STATUS_OUT_OF_RANGE);

  DSA_ASSERT_STATUS(darray_remove_at(arr, 99), DSA_STATUS_OUT_OF_RANGE);

  darray_destroy(arr);
  return DSA_TEST_SUCCESS;
}

dsa_test_status_t test_darray_reserve_and_clear() {
  printf("Running test_darray_reserve_and_clear\n");
  darray_t *arr = NULL;
  darray_create(&arr, 2);

  darray_push_back(arr, 10);
  darray_push_back(arr, 20);
  DSA_ASSERT_TRUE(darray_get_size(arr) == 2);
  DSA_ASSERT_TRUE(darray_get_capacity(arr) >= 2);

  DSA_ASSERT_STATUS(darray_reserve(arr, 100), DSA_STATUS_SUCCESS);
  DSA_ASSERT_TRUE(darray_get_capacity(arr) == 100);
  DSA_ASSERT_TRUE(darray_get_size(arr) == 2);
  DSA_ASSERT_ARRAY_VALUE(arr, 1, 20);

  DSA_ASSERT_STATUS(darray_clear(arr), DSA_STATUS_SUCCESS);
  DSA_ASSERT_TRUE(darray_is_empty(arr));
  DSA_ASSERT_TRUE(darray_get_size(arr) == 0);
  DSA_ASSERT_TRUE(darray_get_capacity(arr) == 100);

  darray_destroy(arr);
  return DSA_TEST_SUCCESS;
}


dsa_test_status_t test_darray_sort_bubble() {
    printf("Running test_darray_sort_bubble (size: %d)\n", LARGE_ARRAY_SIZE);
    darray_t *arr = NULL;
    darray_create(&arr, 10);
    populate_test_array_large(arr, LARGE_ARRAY_SIZE);
    
    DSA_ASSERT_STATUS(darray_sort_bubble(arr), DSA_STATUS_SUCCESS);
    
    DSA_ASSERT_TRUE(darray_get_size(arr) == LARGE_ARRAY_SIZE);
    DSA_ASSERT_TRUE(is_sorted(arr));
    
    darray_destroy(arr);
    return DSA_TEST_SUCCESS;
}

dsa_test_status_t test_darray_sort_merge() {
    printf("Running test_darray_sort_merge (size: %d)\n", LARGE_ARRAY_SIZE);
    darray_t *arr = NULL;
    darray_create(&arr, 10);
    populate_test_array_large(arr, LARGE_ARRAY_SIZE);
    
    DSA_ASSERT_STATUS(darray_sort_merge(arr), DSA_STATUS_SUCCESS);
    
    DSA_ASSERT_TRUE(darray_get_size(arr) == LARGE_ARRAY_SIZE);
    DSA_ASSERT_TRUE(is_sorted(arr));
    
    darray_destroy(arr);
    return DSA_TEST_SUCCESS;
}

dsa_test_status_t test_darray_sort_quick() {
    printf("Running test_darray_sort_quick (size: %d)\n", LARGE_ARRAY_SIZE);
    darray_t *arr = NULL;
    darray_create(&arr, 10);
    populate_test_array_large(arr, LARGE_ARRAY_SIZE);
    
    DSA_ASSERT_STATUS(darray_sort_quick(arr), DSA_STATUS_SUCCESS);
    
    DSA_ASSERT_TRUE(darray_get_size(arr) == LARGE_ARRAY_SIZE);
    DSA_ASSERT_TRUE(is_sorted(arr));
    
    darray_destroy(arr);
    return DSA_TEST_SUCCESS;
}

dsa_test_status_t test_darray_search_linear() {
    printf("Running test_darray_search_linear (size: %d)\n", LARGE_ARRAY_SIZE);
    darray_t *arr = NULL;
    darray_create(&arr, 10);
    populate_test_array_large(arr, LARGE_ARRAY_SIZE);

    DSA_ASSERT_TRUE(darray_search_linear(arr, -1) == -1);

    int value_to_find;
    darray_get_at(arr, LARGE_ARRAY_SIZE / 2, &value_to_find);
    DSA_ASSERT_TRUE(darray_search_linear(arr, value_to_find) != -1);
    
    darray_destroy(arr);
    return DSA_TEST_SUCCESS;
}

dsa_test_status_t test_darray_search_binary() {
    printf("Running test_darray_search_binary (size: %d)\n", LARGE_ARRAY_SIZE);
    darray_t *arr = NULL;
    darray_create(&arr, 10);
    populate_test_array_large(arr, LARGE_ARRAY_SIZE);

    int value_to_find;
    darray_get_at(arr, LARGE_ARRAY_SIZE / 2, &value_to_find);

    DSA_ASSERT_STATUS(darray_sort_merge(arr), DSA_STATUS_SUCCESS); 

    DSA_ASSERT_TRUE(darray_search_binary(arr, -1) == -1);

    DSA_ASSERT_TRUE(darray_search_binary(arr, value_to_find) != -1);

    darray_destroy(arr);
    return DSA_TEST_SUCCESS;
}