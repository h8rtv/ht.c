#ifndef HASHTABLE_H

#include <stdbool.h>
#include <stdint.h>
#include <sys/types.h>

typedef enum {
  HT_SUCCESS = 0,
  HT_ERROR_KEY_NOT_FOUND,
  HT_ERROR_MEMORY_ALLOCATION,
} ht_result;

typedef struct node_t {
  const char *key;
  void *data;
  size_t size;
  struct node_t *next;
} node;

typedef struct {
  size_t size;
  size_t cap;
  node **nodes;
} hashtable;

hashtable *ht_alloc(size_t capacity);
void ht_free(hashtable *);

uint32_t ht_hash(const char *key, size_t capacity);

ht_result ht_insert(hashtable *, const char *key, void *value, size_t size);

bool ht_has(hashtable *, const char *key);

node *ht_get(hashtable *, const char *key);

ht_result ht_delete(hashtable *, const char *key);

#endif // !HASHTABLE_H
