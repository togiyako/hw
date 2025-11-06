#include "dlist.h"
#include <stdlib.h>
#include <stdbool.h>

typedef struct dlist_node {
  int data;
  struct dlist_node *next;
  struct dlist_node *prev;
} dlist_node_t;

struct dlist {
  dlist_node_t *head;
  dlist_node_t *tail;
  int length;
};

static dlist_node_t* dlist_create_node(int data) {
  dlist_node_t *new_node = malloc(sizeof(dlist_node_t));
  if (!new_node) {
    return NULL;
  }
  new_node->data = data;
  new_node->next = NULL;
  new_node->prev = NULL;
  return new_node;
}

static dlist_node_t* dlist_get_node_at(dlist_t *list, int index) {
  if (index < 0 || index >= list->length) {
    return NULL;
  }

  if (index < list->length / 2) {
    dlist_node_t *current = list->head;
    for (int i = 0; i < index; i++) {
      current = current->next;
    }
    return current;
  } else {

    dlist_node_t *current = list->tail;
    for (int i = list->length - 1; i > index; i--) {
      current = current->prev;
    }
    return current;
  }
}

dsa_status_t dlist_create(dlist_t **list) {
  if (!list) {
    return DSA_STATUS_FAILURE;
  }
  
  *list = malloc(sizeof(dlist_t));
  if (!(*list)) {
    return DSA_STATUS_MEM_ALLOC_FAILED;
  }

  (*list)->head = NULL;
  (*list)->tail = NULL;
  (*list)->length = 0;

  return DSA_STATUS_SUCCESS;
}

void dlist_destroy(dlist_t *list) {
  if (!list) {
    return;
  }
  dlist_clear(list);
  free(list);
}

dsa_status_t dlist_insert_front(dlist_t *list, int data) {
  if (!list) {
    return DSA_STATUS_FAILURE;
  }

  dlist_node_t *new_node = dlist_create_node(data);
  if (!new_node) {
    return DSA_STATUS_MEM_ALLOC_FAILED;
  }

  if (dlist_is_empty(list)) {
    list->head = new_node;
    list->tail = new_node;
  } else {
    new_node->next = list->head;
    list->head->prev = new_node;
    list->head = new_node;
  }

  list->length++;
  return DSA_STATUS_SUCCESS;
}

dsa_status_t dlist_insert_back(dlist_t *list, int data) {
  if (!list) {
    return DSA_STATUS_FAILURE;
  }

  dlist_node_t *new_node = dlist_create_node(data);
  if (!new_node) {
    return DSA_STATUS_MEM_ALLOC_FAILED;
  }

  if (dlist_is_empty(list)) {
    list->head = new_node;
    list->tail = new_node;
  } else {
    new_node->prev = list->tail;
    list->tail->next = new_node;
    list->tail = new_node;
  }

  list->length++;
  return DSA_STATUS_SUCCESS;
}

dsa_status_t dlist_insert_at_index(dlist_t *list, int index, int data) {
  if (!list) {
    return DSA_STATUS_FAILURE;
  }
  if (index < 0 || index > list->length) {
    return DSA_STATUS_OUT_OF_RANGE;
  }

  if (index == 0) {
    return dlist_insert_front(list, data);
  }
  if (index == list->length) {
    return dlist_insert_back(list, data);
  }

  dlist_node_t *current_at_index = dlist_get_node_at(list, index);
  if (!current_at_index) {
    return DSA_STATUS_FAILURE;
  }
  
  dlist_node_t *new_node = dlist_create_node(data);
  if (!new_node) {
    return DSA_STATUS_MEM_ALLOC_FAILED;
  }

  dlist_node_t *prev_node = current_at_index->prev;

  new_node->next = current_at_index;
  new_node->prev = prev_node;
  
  prev_node->next = new_node;
  current_at_index->prev = new_node;

  list->length++;
  return DSA_STATUS_SUCCESS;
}

int dlist_find(dlist_t *list, int data) {
  if (!list || dlist_is_empty(list)) {
    return -1;
  }

  dlist_node_t *current = list->head;
  for (int i = 0; i < list->length; i++) {
    if (current->data == data) {
      return i;
    }
    current = current->next;
  }

  return -1;
}

dsa_status_t dlist_remove(dlist_t *list, int index) {
  if (!list || dlist_is_empty(list)) {
    return DSA_STATUS_FAILURE;
  }
  if (index < 0 || index >= list->length) {
    return DSA_STATUS_OUT_OF_RANGE;
  }

  if (index == 0) {
    return dlist_pop_front(list);
  }
  if (index == list->length - 1) {
    return dlist_pop_back(list);
  }

  dlist_node_t *node_to_remove = dlist_get_node_at(list, index);
  if (!node_to_remove) {
    return DSA_STATUS_FAILURE;
  }

  node_to_remove->prev->next = node_to_remove->next;
  node_to_remove->next->prev = node_to_remove->prev;

  free(node_to_remove);
  list->length--;
  
  return DSA_STATUS_SUCCESS;
}

dsa_status_t dlist_pop_back(dlist_t *list) {
  if (!list || dlist_is_empty(list)) {
    return DSA_STATUS_FAILURE;
  }

  dlist_node_t *node_to_remove = list->tail;

  if (list->length == 1) {
    list->head = NULL;
    list->tail = NULL;
  } else {
    list->tail = node_to_remove->prev;
    list->tail->next = NULL;
  }

  free(node_to_remove);
  list->length--;
  return DSA_STATUS_SUCCESS;
}

dsa_status_t dlist_pop_front(dlist_t *list) {
  if (!list || dlist_is_empty(list)) {
    return DSA_STATUS_FAILURE;
  }

  dlist_node_t *node_to_remove = list->head;

  if (list->length == 1) {
    list->head = NULL;
    list->tail = NULL;
  } else {
    list->head = node_to_remove->next;
    list->head->prev = NULL;
  }

  free(node_to_remove);
  list->length--;
  return DSA_STATUS_SUCCESS;
}

dsa_status_t dlist_clear(dlist_t *list) {
  if (!list) {
    return DSA_STATUS_FAILURE;
  }

  dlist_node_t *current = list->head;
  while (current != NULL) {
    dlist_node_t *next = current->next;
    free(current);
    current = next;
  }

  list->head = NULL;
  list->tail = NULL;
  list->length = 0;

  return DSA_STATUS_SUCCESS;
}

bool dlist_is_empty(dlist_t *list) {
  return list ? (list->length == 0) : true;
}

int dlist_len(dlist_t *list) {
  return list ? list->length : 0;
}
