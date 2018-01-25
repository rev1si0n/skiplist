#include <stdio.h>
#include <time.h>
#include "skiplist.h"

#define LEVEL 32


void skiplist_dump_all(skiplist_t* sl) {
  int32_t idx;
  skipnode_t* t;

  for (idx = 0x0, t = sl->root; t; t = t->socket[SKIPLIST_BOTTOM]) {
    printf("node %p :: %u/%u :: key=%-12d level=%-3u order %u\n",
      t,
      sl->size,
      idx++,
      t->key,
      t->level,
      t->order
    );
  }
}

int main(int argc, char* argv[]) {
  clock_t c;
  skiplist_t s;
  skipnode_t* t;
  uint32_t idx, e, i, k, times;

  if (argc == 2) {
    times = atoi(argv[1]);
  } else {
    times = 2000000;
  }

  skiplist_init(&s, LEVEL);

  skiplist_insert(&s, 32, NULL);
  skiplist_insert(&s, 33, NULL);
  skiplist_insert(&s, 33, NULL);
  skiplist_insert(&s, 35, NULL);
  skiplist_insert(&s, 36, NULL);

  if ((t = skiplist_find(&s, 35)) != NULL) {
    printf("skiplist_find(&s, 35) = %d, %p\n", t->key, t);
  }
  if ((t = skiplist_find(&s, 36)) != NULL) {
    printf("skiplist_find(&s, 35) = %d, %p\n", t->key, t);
  }

  skiplist_dump_all(&s);

  skiplist_delete(&s, 33);
  skiplist_delete(&s, 36);
  printf("after free key 33, 36\n");
  if ((t = skiplist_find(&s, 36)) == NULL) {
    printf("skiplist_find(&s, 36) = not found\n");
  }
  skiplist_dump_all(&s);
  skiplist_destroy(&s);

  skiplist_init(&s, LEVEL);
  idx = times, i = 0, e = 0, c = clock();
  printf("random %d hash keys\n", times);
  while (idx--) {
  	k = rand() * rand();
  	if (!skiplist_find(&s, k)){
      skiplist_insert(&s, k, NULL);
      i ++;
    } else {
      e ++;
    }
  }
  printf("%d hash keys inserted, %d keys exist, %ldms\n", i, e, clock() - c);

  idx = times, i = 0, e = 0, c = clock();
  printf("finding %d random hash keys\n", times);
  while (idx--) {
    k = rand() * rand();
  	if (skiplist_find(&s, k)) {
      e ++;
    }
  }
  printf("%d random hash keys finded, %ldms\n", e, clock() - c);

  c = clock();
  printf("destroy skiplist, ");
  skiplist_destroy(&s);
  printf("%ldms\n", clock() - c);
  return 0;
}
