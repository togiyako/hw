#include "tests/tests.h"
#include <stdio.h>

int main() {
  DSA_RUN_TEST(test_add_two_numbers, 2, 4, 7);

  printf("\nSuccess!\n");

  return DSA_TEST_SUCCESS;
}