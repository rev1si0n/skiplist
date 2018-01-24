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
  uint64_t hash = 0x0;

  // 0x0521 -> 1313
  while (*ptr) hash = hash * 0x0521 + (*ptr++);
  return hash;
}

// 随机 level 来自 Redis Zset
// http://download.redis.io/redis-stable/src/t_zset.c
uint8_t skiplist_rand_level(uint8_t max) {
  uint8_t level = 0x01;

  while ((rand() & 0xff) < (0.25 * 0xff))
    level++;

  return (level < max)?level: max;
}

skipnode_t* skiplist_create_node(uint8_t level) {
  skipnode_t* n;
  int32_t msize = sizeof(skipnode_t) + level * sizeof(skipnode_t*);

  if ((n = (skipnode_t*)malloc(msize)) != NULL) {
    memset(n, ZERO_NULL, msize);
    n->level = level;
  }
  return n;
}

void skiplist_destroy(skiplist_t* sl) {
  skipnode_t *n, *t;

  for (t = sl->root; t; t = n) {
    n = t->socket[SKIPLIST_BOTTOM];
    free(t);
  }
}

void skiplist_init(skiplist_t* sl, uint8_t level) {
  skipnode_t* n;

  if ((n = skiplist_create_node(level)) != NULL) {
    sl->root = n;
    sl->size = 0x0;
    sl->level = level;
    n->key = SKIPLIST_KEY_SOL;
  }
}

skipnode_t* (*skiplist_find_previous(skiplist_t* sl, int32_t key))[] {
  uint8_t i;
  skipnode_t *(*prev)[], *n;
  uint32_t s = sizeof(skipnode_t*) * sl->level;

  if ((prev = (skipnode_t* (*)[])malloc(s)) != NULL) {
    memset(prev, ZERO_NULL, s);
    for (i = sl->level, n = sl->root; i--; (*prev)[i] = n) {
      while (n->socket[i] && n->socket[i]->key < key) {
        n = n->socket[i];
      }
    }
  }
  return prev;
}

skipnode_t* skiplist_find(skiplist_t* sl, int32_t key) {
  skipnode_t *(*prev)[], *n = NULL;

  prev = skiplist_find_previous(sl, key);
  if (prev != NULL) {
    n = (*prev)[SKIPLIST_BOTTOM]->socket[SKIPLIST_BOTTOM];
    if (!n || n->key != key) {
      n = NULL;
    }
    free(prev);
  }
  return n;
}

void skiplist_delete(skiplist_t* sl, int32_t key) {
  skipnode_t *(*prev)[], *n = NULL;

  prev = skiplist_find_previous(sl, key);
  if (prev != NULL) {
    n = (*prev)[SKIPLIST_BOTTOM]->socket[SKIPLIST_BOTTOM];
    if (n && n->key == key) {
      uint8_t i = n->level;
      while (i--) {
        (*prev)[i]->socket[i] = (*prev)[i]->socket[i]->socket[i];
      }
    }
    free(prev);
    free(n);
  }
}

bool skiplist_insert(skiplist_t* sl, int32_t key, object* ob) {
  skipnode_t *(*prev)[], *n;

  if ((n = skiplist_create_node(skiplist_rand_level(sl->level))) != NULL) {
    n->key = key;
    n->object = ob;
    n->order = sl->size++;

    if ((prev = skiplist_find_previous(sl, key)) != NULL) {
      uint8_t i = n->level;
      while (i--) {
        n->socket[i] = (*prev)[i]->socket[i];
        (*prev)[i]->socket[i] = n;
      }
      free(prev);
    }
  }
}
