#include "tests.h"
#include <stdio.h>

int main() {
  
  // DSA_RUN_TEST(test_add_two_numbers, 2, 4, 6);

  // printf("\n--- Running Singly Linked List Tests ---\n");
  // DSA_RUN_TEST(test_slist_create_destroy);
  // DSA_RUN_TEST(test_slist_insert_front_back);
  // DSA_RUN_TEST(test_slist_pop_front_back);
  // DSA_RUN_TEST(test_slist_insert_at_index);
  // DSA_RUN_TEST(test_slist_remove_at_index);
  // DSA_RUN_TEST(test_slist_find_and_clear);
  // printf("----------------------------------------");
  // printf("\n--- Running Dynamic Array Tests ---\n");
  // DSA_RUN_TEST(test_darray_create_destroy);
  // DSA_RUN_TEST(test_darray_push_pop);
  // DSA_RUN_TEST(test_darray_capacity_growth);
  // DSA_RUN_TEST(test_darray_insert_at);
  // DSA_RUN_TEST(test_darray_remove_at);
  // DSA_RUN_TEST(test_darray_reserve_and_clear);
  printf("----------------------------------------");
  printf("\n- Running Dynamic Array Sort/Search Tests ---\n");
  DSA_RUN_TEST(test_darray_search_linear);
  printf("\n");
  DSA_RUN_TEST(test_darray_sort_bubble);
  printf("\n");
  DSA_RUN_TEST(test_darray_sort_merge);
  printf("\n");
  DSA_RUN_TEST(test_darray_sort_quick);
  printf("\n");
  DSA_RUN_TEST(test_darray_search_binary);
  printf("----------------------------------------");
  // printf("\n--- Running Doubly Linked List Tests ---\n");
  // DSA_RUN_TEST(test_dlist_create_destroy);
  // DSA_RUN_TEST(test_dlist_insert_front_back);
  // DSA_RUN_TEST(test_dlist_pop_front_back);
  // DSA_RUN_TEST(test_dlist_insert_at_index);
  // DSA_RUN_TEST(test_dlist_remove_at_index);
  // DSA_RUN_TEST(test_dlist_find_and_clear);
  // printf("----------------------------------------\n");

  printf("\nAll tests passed!\n");

  return DSA_TEST_SUCCESS;
}
