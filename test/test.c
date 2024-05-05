#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "../src/hashtable.h"

#define CAP 1000

int test_create() {
  hashtable *ht = ht_alloc(CAP);

  assert(ht->cap == CAP);
  assert(ht->size == 0);

  ht_free(ht);

  return 0;
}

int test_insert_get() {
  hashtable *ht = ht_alloc(CAP);

  ht_insert(ht, "test1", "value1", 7 * sizeof(char));
  ht_insert(ht, "test2", "value2", 7 * sizeof(char));

  node *n1 = ht_get(ht, "test1");
  node *n2 = ht_get(ht, "test2");
  node *n3 = ht_get(ht, "test3");

  assert(strcmp(n1->data, "value1") == 0);
  assert(strcmp(n2->data, "value2") == 0);
  assert(n3 == NULL);

  ht_free(ht);

  return 0;
}

int test_insert_has() {
  hashtable *ht = ht_alloc(CAP);

  ht_insert(ht, "test1", "value1", 7 * sizeof(char));
  ht_insert(ht, "test2", "value2", 7 * sizeof(char));

  bool has_n1 = ht_has(ht, "test1");
  bool has_n2 = ht_has(ht, "test2");
  bool has_n3 = ht_has(ht, "test3");

  assert(has_n1 == true);
  assert(has_n2 == true);
  assert(has_n3 == false);

  assert(ht->size == 2);

  ht_free(ht);

  return 0;
}

int test_insert_delete() {
  hashtable *ht = ht_alloc(CAP);

  ht_insert(ht, "test1", "value1", 7 * sizeof(char));
  ht_insert(ht, "test2", "value2", 7 * sizeof(char));

  assert(ht->size == 2);

  ht_result res1 = ht_delete(ht, "test1");
  ht_result res2 = ht_delete(ht, "test2");

  assert(res1 == HT_SUCCESS);
  assert(res2 == HT_SUCCESS);

  bool has_n1 = ht_has(ht, "test1");
  bool has_n2 = ht_has(ht, "test2");

  assert(has_n1 == false);
  assert(has_n2 == false);

  assert(ht->size == 0);

  ht_free(ht);

  return 0;
}

int test_delete_invalid() {
  hashtable *ht = ht_alloc(CAP);

  ht_insert(ht, "test1", "value1", 7 * sizeof(char));
  ht_insert(ht, "test2", "value2", 7 * sizeof(char));

  assert(ht->size == 2);

  ht_result res = ht_delete(ht, "test3");

  assert(res == HT_ERROR_KEY_NOT_FOUND);

  assert(ht->size == 2);

  ht_free(ht);

  return 0;
}

int main() {
  printf("test_create: ");
  if (test_create() == 0) {
    printf("passed!\n");
  }

  printf("test_insert_get: ");
  if (test_insert_get() == 0) {
    printf("passed!\n");
  }

  printf("test_insert_has: ");
  if (test_insert_has() == 0) {
    printf("passed!\n");
  }

  printf("test_insert_delete: ");
  if (test_insert_delete() == 0) {
    printf("passed!\n");
  }

  printf("test_delete_invalid: ");
  if (test_delete_invalid() == 0) {
    printf("passed!\n");
  }

  return 0;
}
