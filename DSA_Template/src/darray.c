#include "darray.h"
#include <stdlib.h>
#include <string.h>

#define DARRAY_DEFAULT_CAPACITY 8
#define DARRAY_GROWTH_FACTOR 2

struct darray {
  int *data;
  int size;
  int capacity;
};

static dsa_status_t darray_grow_capacity(darray_t *array) {

  if (array->size < array->capacity) {
    return DSA_STATUS_SUCCESS; 
  }

  int new_capacity = (array->capacity == 0) 
                     ? DARRAY_DEFAULT_CAPACITY 
                     : array->capacity * DARRAY_GROWTH_FACTOR;

  int *new_data = realloc(array->data, sizeof(int) * new_capacity);
  if (!new_data) {
    return DSA_STATUS_MEM_ALLOC_FAILED;
  }

  array->data = new_data;
  array->capacity = new_capacity;
  return DSA_STATUS_SUCCESS;
}


dsa_status_t darray_create(darray_t **array, int initial_capacity) {

  if (!array) {
    return DSA_STATUS_FAILURE;
  }

  *array = malloc(sizeof(darray_t));
  if (!(*array)) {
    return DSA_STATUS_MEM_ALLOC_FAILED;
  }

  int capacity = (initial_capacity > 0) ? initial_capacity : DARRAY_DEFAULT_CAPACITY;

  (*array)->data = malloc(sizeof(int) * capacity);
  if (!(*array)->data) {
    free(*array);
    return DSA_STATUS_MEM_ALLOC_FAILED;
  }

  (*array)->size = 0;
  (*array)->capacity = capacity;
  return DSA_STATUS_SUCCESS;
}

void darray_destroy(darray_t *array) {

  if (!array) {
    return;
  }

  if (array->data) {
    free(array->data);
  }

  free(array);
}

dsa_status_t darray_push_back(darray_t *array, int data) {
    
    if (!array) {
    return DSA_STATUS_FAILURE;
  }

  if (array->size == array->capacity) {
    dsa_status_t status = darray_grow_capacity(array);
    if (status != DSA_STATUS_SUCCESS) {
      return status;
    }
  }

  array->data[array->size] = data;
  array->size++;
  return DSA_STATUS_SUCCESS;
}

dsa_status_t darray_pop_back(darray_t *array) {
  if (!array || darray_is_empty(array)) {
    return DSA_STATUS_FAILURE;
  }
  array->size--;
  return DSA_STATUS_SUCCESS;
}

dsa_status_t darray_insert_at(darray_t *array, int index, int data) {

  if (!array) {
    return DSA_STATUS_FAILURE;
  }

  if (index < 0 || index > array->size) {
    return DSA_STATUS_OUT_OF_RANGE;
  }
  
  if (index == array->size) {
    return darray_push_back(array, data);
  }

  if (array->size == array->capacity) {
    dsa_status_t status = darray_grow_capacity(array);
    if (status != DSA_STATUS_SUCCESS) {
      return status;
    }
  }

  memmove(
    &array->data[index + 1],
    &array->data[index],
    (array->size - index) * sizeof(int)
  );

  array->data[index] = data;
  array->size++;
  return DSA_STATUS_SUCCESS;
}

dsa_status_t darray_remove_at(darray_t *array, int index) {

  if (!array) {
    return DSA_STATUS_FAILURE;
  }

  if (index < 0 || index >= array->size) {
    return DSA_STATUS_OUT_OF_RANGE;
  }

  memmove(
    &array->data[index],
    &array->data[index + 1],
    (array->size - index - 1) * sizeof(int)
  );

  array->size--;
  return DSA_STATUS_SUCCESS;
}

dsa_status_t darray_get_at(darray_t *array, int index, int *data) {

  if (!array) {
    return DSA_STATUS_FAILURE;
  }

  if (index < 0 || index >= array->size) {
    return DSA_STATUS_OUT_OF_RANGE;
  }

  if (!data) {
    return DSA_STATUS_FAILURE;
  }

  *data = array->data[index];
  return DSA_STATUS_SUCCESS;
}

dsa_status_t darray_reserve(darray_t *array, int new_capacity) {
  if (!array) {
    return DSA_STATUS_FAILURE;
  }

  if (new_capacity <= array->capacity) {
    return DSA_STATUS_SUCCESS;
  }

  int *new_data = realloc(array->data, sizeof(int) * new_capacity);
  if (!new_data) {
    return DSA_STATUS_MEM_ALLOC_FAILED;
  }

  array->data = new_data;
  array->capacity = new_capacity;
  return DSA_STATUS_SUCCESS;
}

dsa_status_t darray_clear(darray_t *array) {
  if (!array) {
    return DSA_STATUS_FAILURE;
  }
  array->size = 0;
  return DSA_STATUS_SUCCESS;
}

bool darray_is_empty(darray_t *array) {
  return array ? (array->size == 0) : true;
}

int darray_get_size(darray_t *array) {
  return array ? array->size : 0;
}

int darray_get_capacity(darray_t *array) {
  return array ? array->capacity : 0;
}