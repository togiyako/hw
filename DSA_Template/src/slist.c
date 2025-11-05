#include "slist.h"
#include <stdlib.h>
#include <stdbool.h>

typedef struct slist_node {
  int data;
  struct slist_node *next;
} slist_node_t;

struct slist {
  slist_node_t *head;
  slist_node_t *tail;
  int length;
};

static slist_node_t* slist_create_node(int data) {
  slist_node_t *new_node = malloc(sizeof(slist_node_t));
  if (!new_node) {
    return NULL;
  }
  new_node->data = data;
  new_node->next = NULL;
  return new_node;
}

dsa_status_t slist_create(slist_t **list) {
  if (!list) {
    return DSA_STATUS_FAILURE;
  }
  
  *list = malloc(sizeof(slist_t));
  if (!(*list)) {
    return DSA_STATUS_MEM_ALLOC_FAILED;
  }

  (*list)->head = NULL;
  (*list)->tail = NULL;
  (*list)->length = 0;

  return DSA_STATUS_SUCCESS;

}

void slist_destroy(slist_t *list) {
  if (!list) {
    return;
  }
  
  slist_clear(list);
  free(list);
}

dsa_status_t slist_insert_front(slist_t *list, int data) {
  if (!list) {
    return DSA_STATUS_FAILURE;
  }

  slist_node_t *new_node = slist_create_node(data);
  if (!new_node) {
    return DSA_STATUS_MEM_ALLOC_FAILED;
  }

  if (slist_is_empty(list)) {
    list->head = new_node;
    list->tail = new_node;
  } else {
    new_node->next = list->head;
    list->head = new_node;
  }

  list->length++;
  return DSA_STATUS_SUCCESS;
}

dsa_status_t slist_insert_back(slist_t *list, int data) {
  if (!list) {
    return DSA_STATUS_FAILURE;
  }

  slist_node_t *new_node = slist_create_node(data);
  if (!new_node) {
    return DSA_STATUS_MEM_ALLOC_FAILED;
  }

  if (slist_is_empty(list)) {
    list->head = new_node;
    list->tail = new_node;
  } else {
    list->tail->next = new_node;
    list->tail = new_node;
  }

  list->length++;
  return DSA_STATUS_SUCCESS;
}


dsa_status_t slist_insert_at_index(slist_t *list, int index, int data) {
  if (!list) {
    return DSA_STATUS_FAILURE;
  }
  if (index < 0 || index > list->length) {
    return DSA_STATUS_OUT_OF_RANGE;
  }

  if (index == 0) {
    return slist_insert_front(list, data);
  }

  if (index == list->length) {
    return slist_insert_back(list, data);
  }

  slist_node_t *new_node = slist_create_node(data);
  if (!new_node) {
    return DSA_STATUS_MEM_ALLOC_FAILED;
  }

  slist_node_t *prev = list->head;
  for (int i = 0; i < index - 1; i++) {
    prev = prev->next;
  }

  new_node->next = prev->next;
  prev->next = new_node;

  list->length++;
  return DSA_STATUS_SUCCESS;
}

int slist_find(slist_t *list, int data) {
  if (!list || slist_is_empty(list)) {
    return -1;
  }

  slist_node_t *current = list->head;
  for (int i = 0; i < list->length; i++) {
    if (current->data == data) {
      return i;
    }
    current = current->next;
  }

  return -1;
}

dsa_status_t slist_remove(slist_t *list, int index) {
  if (!list || slist_is_empty(list)) {
    return DSA_STATUS_FAILURE;
  }
  if (index < 0 || index >= list->length) {
    return DSA_STATUS_OUT_OF_RANGE;
  }

  if (index == 0) {
    return slist_pop_front(list);
  }

  slist_node_t *prev = list->head;
  for (int i = 0; i < index - 1; i++) {
    prev = prev->next;
  }

  slist_node_t *node_to_remove = prev->next;
  
  if (node_to_remove == list->tail) {
    list->tail = prev;
  }

  prev->next = node_to_remove->next;
  free(node_to_remove);
  
  list->length--;
  return DSA_STATUS_SUCCESS;
}

dsa_status_t slist_pop_back(slist_t *list) {
  if (!list || slist_is_empty(list)) {
    return DSA_STATUS_FAILURE;
  }

  if (list->length == 1) {
    return slist_pop_front(list);
  }

  slist_node_t *prev = list->head;
  while (prev->next != list->tail) {
    prev = prev->next;
  }
  
  free(list->tail);
  list->tail = prev;
  list->tail->next = NULL;
  
  list->length--;
  return DSA_STATUS_SUCCESS;
}

dsa_status_t slist_pop_front(slist_t *list) {
  if (!list || slist_is_empty(list)) {
    return DSA_STATUS_FAILURE;
  }

  slist_node_t *node_to_remove = list->head;

  list->head = list->head->next;
  
  if (list->head == NULL) {
    list->tail = NULL;
  }
  
  free(node_to_remove);
  list->length--;
  
  return DSA_STATUS_SUCCESS;
}

dsa_status_t slist_clear(slist_t *list) {
  if (!list) {
    return DSA_STATUS_FAILURE;
  }

  slist_node_t *current = list->head;
  while (current != NULL) {
    slist_node_t *next = current->next;
    free(current);
    current = next;
  }

  list->head = NULL;
  list->tail = NULL;
  list->length = 0;

  return DSA_STATUS_SUCCESS;
}

bool slist_is_empty(slist_t *list) {
  return list ? (list->length == 0) : true;
}

int slist_len(slist_t *list) {
  return list ? list->length : 0;
}
