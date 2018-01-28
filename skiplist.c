/*
* MIT License

* Copyright (c) 2018 Zhang Yi Da

* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:

* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.

* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/
#include "skiplist.h"

// BKDR hash
uint64_t BKDR_hash(const char *ptr) {
  uint64_t hash = ZERO_NULL;

  // 0x0521 -> 1313
  while (*ptr) hash = hash * 0x0521 + (*ptr++);
  return hash;
}

skipnode_t* skiplist_create_node(uint8_t level) {
  skipnode_t *node;

  #define _SIZE sizeof(skipnode_t) + level * sizeof(skipnode_t*)
  if ((node = (skipnode_t*)malloc(_SIZE)) != NULL) {
    memset(node, ZERO_NULL, _SIZE);
    node->level = level;
  }
  return node;
}

void skiplist_destroy(skiplist_t* sl) {
  skipnode_t *node, *t;

  for (t = sl->root; t; t = node) {
    node = t->socket[SKIPLIST_BOTTOM];
    free(t);
  }
}

void skiplist_init(skiplist_t* sl, uint8_t level) {
  skipnode_t *node;

  if (SKIPLIST_MIN_LEVEL > level || UINT8_MAX < level)
    level = SKIPLIST_LEVEL;

  if ((node = skiplist_create_node(level)) != NULL) {
    sl->root  = node;
    sl->level = level;
    sl->count  = ZERO_NULL;
    node->key = SKIPLIST_KEY_SOL;
  }
}

void skiplist_find_prev_nodes(skiplist_t* sl, int32_t key, skipnode_t* prev[]) {
  skipnode_t *node = sl->root;

  memset(prev, ZERO_NULL, sizeof(skipnode_t*) * sl->level);
  for (uint8_t i = sl->level; i-- ; prev[i] = node) {
    while (node->socket[i] && node->socket[i]->key < key) {
      node = node->socket[i];
    }
  }
}

skipnode_t* skiplist_find(skiplist_t* sl, int32_t key) {
  skipnode_t* prev[sl->level], *node = NULL;

  skiplist_find_prev_nodes(sl, key, prev);
  node = prev[SKIPLIST_BOTTOM]->socket[SKIPLIST_BOTTOM];
  if (!node || node->key != key) {
    node = NULL;
  }
  return node;
}

void skiplist_delete(skiplist_t* sl, int32_t key) {
  skipnode_t* prev[sl->level], *node = NULL;

  skiplist_find_prev_nodes(sl, key, prev);
  node = prev[SKIPLIST_BOTTOM]->socket[SKIPLIST_BOTTOM];
  if (node && node->key == key) {
    uint8_t i = node->level;
    while (i--) {
      prev[i]->socket[i] = prev[i]->socket[i]->socket[i];
    }
    sl->count--;
  }
}

bool skiplist_insert(skiplist_t* sl, int32_t key, object* ptr) {
  uint8_t level = 0x01;
  skipnode_t* prev[sl->level], *node = NULL;
  // random level
  while ((rand() & 0xff) < (0.25 * 0xff))
    level++;

  level = (level <= sl->level)?level: sl->level;

  if ((node = skiplist_create_node(level)) == NULL) {
    return false;
  }

  node->key    = key;
  node->order  = sl->count++;
  node->object = ptr;

  skiplist_find_prev_nodes(sl, key, prev);
  while (level--) {
    node->socket[level] = prev[level]->socket[level];
    prev[level]->socket[level] = node;
  }
  return true;
}
