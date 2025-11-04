#include "slist.h"

dsa_status_t slist_create(slist_t **list) {
  return DSA_STATUS_SUCCESS;
}

void slist_destroy(slist_t *list) {
}

dsa_status_t slist_insert_front(slist_t *list, int data) {
  return DSA_STATUS_SUCCESS;
}

dsa_status_t slist_insert_back(slist_t *list, int data) {
  return DSA_STATUS_SUCCESS;
}

dsa_status_t slist_insert_at_index(slist_t *list, int index, int data) {
  return DSA_STATUS_SUCCESS;
}

int slist_find(slist_t *list, int data) {
  return -1;
}

dsa_status_t slist_remove(slist_t *list, int index) {
  return DSA_STATUS_SUCCESS;
}

dsa_status_t slist_pop_back(slist_t *list) {
  return DSA_STATUS_SUCCESS;
}

dsa_status_t slist_pop_front(slist_t *list) {
  return DSA_STATUS_SUCCESS;
}

dsa_status_t slist_clear(slist_t *list) {
  return DSA_STATUS_SUCCESS;
}

bool slist_is_empty(slist_t *list) {
  return true;
}

int slist_len(slist_t *list) {
  return 0;
}
