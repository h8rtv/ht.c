#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashtable.h"

hashtable *ht_alloc(size_t capacity) {
  hashtable *ht = (hashtable *)malloc(sizeof(hashtable));
  if (ht == NULL) {
    fprintf(stderr, "[ERR] Couldn't allocate the HashTable, Errno: %d\n",
            HT_ERROR_MEMORY_ALLOCATION);
    return NULL;
  }

  ht->size = 0;
  ht->cap = capacity;
  ht->nodes = malloc(sizeof(node *) * capacity);
  for (size_t i = 0; i < capacity; ++i) {
    ht->nodes[i] = NULL;
  }


  if (ht->nodes == NULL) {
    fprintf(stderr,
            "[ERR] Couldn't allocate data for the HashTable, Errno: %d\n",
            HT_ERROR_MEMORY_ALLOCATION);

    free(ht);
    return NULL;
  }

  return ht;
}

void ht_free(hashtable *ht) {
  for (size_t i = 0; i < ht->cap; ++i) {
    node *n = ht->nodes[i];
    while (n != NULL) {
      node *next = n->next;
      free(n->data);
      free(n);
      n = next;
    }
  }

  free(ht->nodes);
  free(ht);
}

// Polynomial rolling hash
uint32_t ht_hash(const char *key, size_t capacity) {
  if (key == NULL) {
    return 0;
  }

  uint32_t hash = 0;
  size_t i = 0;
  uint32_t p_pow = 1;
  char c = key[i];

  while (c != '\0') {
    hash = (hash + (c - 'a' + 1) * p_pow) % capacity;
    p_pow = (p_pow * 31) % capacity;
    c = key[++i];
  }

  return hash;
}

ht_result ht_insert(hashtable *ht, const char *key, void *value, size_t size) {
  uint32_t hash = ht_hash(key, ht->cap);
  node *n = ht->nodes[hash];

  while (n != NULL) {
    if (strcmp(n->key, key) == 0) {
      free(n->data);
      n->size = size;
      void *new_data = malloc(n->size);
      memcpy(new_data, value, n->size);
      n->data = new_data;
      return HT_SUCCESS;
    }
    n = n->next;
  }

  node *new_node = malloc(sizeof(node));
  if (new_node == NULL) {
    fprintf(stderr,
            "[ERR] Couldn't allocate node for the HashTable, Errno: %d\n",
            HT_ERROR_MEMORY_ALLOCATION);
    return HT_ERROR_MEMORY_ALLOCATION;
  }

  new_node->key = key;
  new_node->size = size;
  void *new_data = malloc(new_node->size);
  memcpy(new_data, value, new_node->size);
  new_node->data = new_data;

  new_node->next = ht->nodes[hash];
  ht->nodes[hash] = new_node;

  ht->size += 1;

  return HT_SUCCESS;
}

node *ht_get(hashtable *ht, const char *key) {
  uint32_t hash = ht_hash(key, ht->cap);
  node *n = ht->nodes[hash];

  while (n != NULL) {
    if (strcmp(n->key, key) == 0) {
      return n;
    }
    n = n->next;
  }

  return NULL;
}

bool ht_has(hashtable *ht, const char *key) {
  uint32_t hash = ht_hash(key, ht->cap);
  node *n = ht->nodes[hash];
  while (n != NULL) {
    if (strcmp(n->key, key) == 0) {
      return true;
    }
    n = n->next;
  }

  return false;
}

ht_result ht_delete(hashtable *ht, const char *key) {
  uint32_t hash = ht_hash(key, ht->cap);
  node *n = ht->nodes[hash];
  node *prev = NULL;
  while (n != NULL) {
    if (strcmp(n->key, key) == 0) {
      if (prev != NULL) {
        prev->next = n->next;
      } else {
        ht->nodes[hash] = n->next;
      }

      free(n->data);
      free(n);
      ht->size -= 1;

      return HT_SUCCESS;
    }

    prev = n;
    n = n->next;
  }

  return HT_ERROR_KEY_NOT_FOUND;
}
