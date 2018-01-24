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
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>


#define ZERO_NULL           (0x0)
#define SKIPLIST_BOTTOM     (0x0)
#define SKIPLIST_KEY_SOL    INT32_MIN


typedef void object;

typedef struct skipnode {
  int32_t key; // key
  uint8_t level;
  uint32_t order; // add for lzw compress
  object* object; // data obj pointer
  struct skipnode *socket[];
} skipnode_t;

typedef struct skiplist {
  int32_t size;
  uint8_t level;
  skipnode_t* root;
} skiplist_t;


void skiplist_init(skiplist_t* sl, uint8_t level);
void skiplist_destroy(skiplist_t* sl);
bool skiplist_insert(skiplist_t* sl, int32_t key, object* ob);;
void skiplist_delete(skiplist_t* sl, int32_t key);
skipnode_t* skiplist_find(skiplist_t* sl, int32_t key);
uint64_t BKDR_hash(const char *ptr);
