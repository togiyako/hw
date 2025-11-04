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

#endif