#ifndef TESTS_H
#define TESTS_H

#define DSA_RUN_TEST(func, ...)                             \
  do {                                                      \
      if (func(__VA_ARGS__) != DSA_TEST_SUCCESS) {          \
        printf("Test %s failed\n", #func);                  \
        return DSA_TEST_FAILURE;                            \
      }                                                     \
  } while (0)

typedef enum {
  DSA_TEST_SUCCESS = 0,
  DSA_TEST_FAILURE,
} dsa_test_status_t;

dsa_test_status_t test_add_two_numbers(int a, int b, int expected_result);

dsa_test_status_t test_slist_create_destroy();
dsa_test_status_t test_slist_insert_front_back();
dsa_test_status_t test_slist_pop_front_back();
dsa_test_status_t test_slist_insert_at_index();
dsa_test_status_t test_slist_remove_at_index();
dsa_test_status_t test_slist_find_and_clear();

dsa_test_status_t test_dlist_create_destroy();
dsa_test_status_t test_dlist_insert_front_back();
dsa_test_status_t test_dlist_pop_front_back();
dsa_test_status_t test_dlist_insert_at_index();
dsa_test_status_t test_dlist_remove_at_index();
dsa_test_status_t test_dlist_find_and_clear();

dsa_test_status_t test_darray_create_destroy();
dsa_test_status_t test_darray_push_pop();
dsa_test_status_t test_darray_capacity_growth();
dsa_test_status_t test_darray_insert_at();
dsa_test_status_t test_darray_remove_at();
dsa_test_status_t test_darray_reserve_and_clear();

#endif