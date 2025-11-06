#include "darray.h"
#include <stdlib.h>
#include <string.h>

#ifdef DSA_PERF_MEASURE
#include <time.h>
#include <stdio.h>

#define START_TIMER() \
  clock_t start_t = clock();

#define STOP_TIMER(name) \
  clock_t end_t = clock(); \
  double elapsed_t = (double)(end_t - start_t) / CLOCKS_PER_SEC; \
  printf("%s took: %.6f seconds\n", name, elapsed_t);
#else

#define START_TIMER()
#define STOP_TIMER(name)
#endif

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

// Sorting and searching //

static void swap(int *a, int *b){
  int temp = *a;
  *a = *b;
  *b = temp;
}

dsa_status_t darray_sort_bubble(darray_t *array){
  if (!array) {
    return DSA_STATUS_FAILURE;
  }

  if (array->size < 2) {
        return DSA_STATUS_SUCCESS;
  }

  START_TIMER();

  for(int i = 0; i < array->size - 1; i++){
        for (int j = 0; j < array->size - i - 1; j++) {
            if (array->data[j] > array->data[j + 1]) {
                swap(&array->data[j], &array->data[j + 1]);
            }
        }
    }
  
  STOP_TIMER("darray_sort_bubble");
    return DSA_STATUS_SUCCESS;
}

static void merge(int *arr, int *temp, int left, int mid, int right){
  int i = left, j = mid + 1, k = left;
    
    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j]) {
            temp[k++] = arr[i++];
        } else {
            temp[k++] = arr[j++];
        }
    }
    while (i <= mid) temp[k++] = arr[i++];
    while (j <= right) temp[k++] = arr[j++];
    
    for (i = left; i <= right; i++) {
        arr[i] = temp[i];
    }
}

static void merge_sort_recursive(int *arr, int *temp, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        merge_sort_recursive(arr, temp, left, mid);
        merge_sort_recursive(arr, temp, mid + 1, right);
        merge(arr, temp, left, mid, right);
    }
}

dsa_status_t darray_sort_merge(darray_t *array){
  if (!array) {
    return DSA_STATUS_FAILURE;
  }

  START_TIMER();
    
  int *temp = malloc(sizeof(int) * array->size);
  if (!temp) return DSA_STATUS_MEM_ALLOC_FAILED;
    
  merge_sort_recursive(array->data, temp, 0, array->size - 1);
    
  free(temp);
  STOP_TIMER("darray_sort_merge");
  return DSA_STATUS_SUCCESS;
}

static int partition(int *arr, int low, int high) {
    int pivot = arr[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

static void quick_sort_recursive(int *arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quick_sort_recursive(arr, low, pi - 1);
        quick_sort_recursive(arr, pi + 1, high);
    }
}

dsa_status_t darray_sort_quick(darray_t *array) {
    if (!array){ 
      return DSA_STATUS_FAILURE;
    }

    START_TIMER();
    quick_sort_recursive(array->data, 0, array->size - 1);
    STOP_TIMER("darray_sort_quick");
    
    return DSA_STATUS_SUCCESS;
}

int darray_search_linear(darray_t *array, int data) {
  if (!array){ 
      return DSA_STATUS_FAILURE;
  }

  START_TIMER();
  for(int i = 0; i < array->size; i++){
    if(array->data[i] == data){
      STOP_TIMER("darray_search_linear (found)");
      return i;
    }
  }

  STOP_TIMER("darray_search_linear (not found)");
  return -1;
}

int darray_search_binary(darray_t *array, int data){
  if (!array){ 
      return DSA_STATUS_FAILURE;
  }

  START_TIMER();
  int low = 0;
  int hight = array->size - 1;

  while (low <= hight)
  {
    int mid = low + (hight - low) / 2;

    if(array->data[mid] == data){
      STOP_TIMER("darray_search_binary (found)");
      return mid;
    }

    if(array->data[mid] < data)
      low = mid + 1;
    else
      hight = mid - 1;
  }
  
  STOP_TIMER("darray_search_binary (not found)");
  return -1;
}


