#include "tests.h"
#include "slist.h"
#include "status.h"
#include <stdio.h>
#include <assert.h>

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

dsa_test_status_t test_slist_create_destroy() {
  printf("Running test_slist_create_destroy \n");
  slist_t *list = NULL;

  DSA_ASSERT_STATUS(slist_create(&list), DSA_STATUS_SUCCESS);
  DSA_ASSERT_TRUE(list != NULL);
  DSA_ASSERT_TRUE(slist_is_empty(list));
  DSA_ASSERT_TRUE(slist_len(list) == 0);

  slist_destroy(list);
  return DSA_TEST_SUCCESS;
}

dsa_test_status_t test_slist_insert_front_back() {
  printf("Running test_slist_insert_front_back\n");
  slist_t *list = NULL;
  slist_create(&list);

  DSA_ASSERT_STATUS(slist_insert_front(list, 10), DSA_STATUS_SUCCESS);
  DSA_ASSERT_TRUE(slist_len(list) == 1);
  DSA_ASSERT_TRUE(slist_find(list, 10) == 0);

  DSA_ASSERT_STATUS(slist_insert_front(list, 20), DSA_STATUS_SUCCESS);
  DSA_ASSERT_TRUE(slist_len(list) == 2);
  DSA_ASSERT_TRUE(slist_find(list, 20) == 0);
  DSA_ASSERT_TRUE(slist_find(list, 10) == 1);

  DSA_ASSERT_STATUS(slist_insert_back(list, 30), DSA_STATUS_SUCCESS);
  DSA_ASSERT_TRUE(slist_len(list) == 3);
  DSA_ASSERT_TRUE(slist_find(list, 30) == 2);
  
  slist_destroy(list);
  return DSA_TEST_SUCCESS;
}

dsa_test_status_t test_slist_pop_front_back() {
  printf("Running test_slist_pop_front_back\n");
  slist_t *list = NULL;
  slist_create(&list);

  slist_insert_back(list, 10);
  slist_insert_back(list, 20);
  slist_insert_back(list, 30);

  DSA_ASSERT_STATUS(slist_pop_back(list), DSA_STATUS_SUCCESS);
  DSA_ASSERT_TRUE(slist_len(list) == 2);
  DSA_ASSERT_TRUE(slist_find(list, 30) == -1);

  DSA_ASSERT_STATUS(slist_pop_front(list), DSA_STATUS_SUCCESS);
  DSA_ASSERT_TRUE(slist_len(list) == 1);
  DSA_ASSERT_TRUE(slist_find(list, 10) == -1);
  DSA_ASSERT_TRUE(slist_find(list, 20) == 0);

  DSA_ASSERT_STATUS(slist_pop_back(list), DSA_STATUS_SUCCESS);
  DSA_ASSERT_TRUE(slist_is_empty(list));
  DSA_ASSERT_TRUE(slist_len(list) == 0);

  DSA_ASSERT_STATUS(slist_pop_front(list), DSA_STATUS_FAILURE);
  DSA_ASSERT_STATUS(slist_pop_back(list), DSA_STATUS_FAILURE);

  slist_destroy(list);
  return DSA_TEST_SUCCESS;
}

dsa_test_status_t test_slist_insert_at_index() {
  printf("Running test_slist_insert_at_index\n");
  slist_t *list = NULL;
  slist_create(&list);

  DSA_ASSERT_STATUS(slist_insert_at_index(list, 0, 10), DSA_STATUS_SUCCESS);
  DSA_ASSERT_TRUE(slist_len(list) == 1);

  DSA_ASSERT_STATUS(slist_insert_at_index(list, 1, 30), DSA_STATUS_SUCCESS);
  DSA_ASSERT_TRUE(slist_len(list) == 2);

  DSA_ASSERT_STATUS(slist_insert_at_index(list, 1, 20), DSA_STATUS_SUCCESS);
  DSA_ASSERT_TRUE(slist_len(list) == 3);
  DSA_ASSERT_TRUE(slist_find(list, 10) == 0);
  DSA_ASSERT_TRUE(slist_find(list, 20) == 1);
  DSA_ASSERT_TRUE(slist_find(list, 30) == 2);

  DSA_ASSERT_STATUS(slist_insert_at_index(list, 99, 0), DSA_STATUS_OUT_OF_RANGE);
  DSA_ASSERT_STATUS(slist_insert_at_index(list, -1, 0), DSA_STATUS_OUT_OF_RANGE);

  slist_destroy(list);
  return DSA_TEST_SUCCESS;
}

dsa_test_status_t test_slist_remove_at_index() {
  printf("Running test_slist_remove_at_index\n");
  slist_t *list = NULL;
  slist_create(&list);

  slist_insert_back(list, 10);
  slist_insert_back(list, 20);
  slist_insert_back(list, 30);
  slist_insert_back(list, 40);

  DSA_ASSERT_STATUS(slist_remove(list, 1), DSA_STATUS_SUCCESS);
  DSA_ASSERT_TRUE(slist_len(list) == 3);
  DSA_ASSERT_TRUE(slist_find(list, 20) == -1);
  DSA_ASSERT_TRUE(slist_find(list, 30) == 1);

  DSA_ASSERT_STATUS(slist_remove(list, 0), DSA_STATUS_SUCCESS);
  DSA_ASSERT_TRUE(slist_len(list) == 2);
  DSA_ASSERT_TRUE(slist_find(list, 10) == -1);
  DSA_ASSERT_TRUE(slist_find(list, 30) == 0);

  DSA_ASSERT_STATUS(slist_remove(list, 1), DSA_STATUS_SUCCESS);
  DSA_ASSERT_TRUE(slist_len(list) == 1);
  DSA_ASSERT_TRUE(slist_find(list, 40) == -1);

  DSA_ASSERT_STATUS(slist_remove(list, 99), DSA_STATUS_OUT_OF_RANGE);

  slist_destroy(list);
  return DSA_TEST_SUCCESS;
}

dsa_test_status_t test_slist_find_and_clear() {
  printf("Running test_slist_find_and_clear\n");
  slist_t *list = NULL;
  slist_create(&list);
  
  slist_insert_back(list, 10);
  slist_insert_back(list, 20);
  slist_insert_back(list, 30);
  
  DSA_ASSERT_TRUE(slist_find(list, 20) == 1);
  DSA_ASSERT_TRUE(slist_find(list, 99) == -1);
  
  DSA_ASSERT_STATUS(slist_clear(list), DSA_STATUS_SUCCESS);
  DSA_ASSERT_TRUE(slist_is_empty(list));
  DSA_ASSERT_TRUE(slist_len(list) == 0);
  DSA_ASSERT_TRUE(slist_find(list, 10) == -1);

  slist_destroy(list);
  return DSA_TEST_SUCCESS;
}