#include "tests.h"
#include <stdio.h>

#define DSA_ASSERT_EQUAL(received_value, expected_value)                                  \
  do {                                                                                          \
      if (received_value != expected_value) {                                                   \
        printf("Assertion failed: expected %d, received %d\n", expected_value, received_value); \
        return DSA_TEST_FAILURE;                                                                \
      }                                                                                         \
  } while (0)

dsa_test_status_t test_add_two_numbers(int a, int b, int expected_result) {
  printf("Running %s\n", "test_add_two_numbers");
  DSA_ASSERT_EQUAL(a + b, expected_result);

  return DSA_TEST_SUCCESS;
}