#include <stdio.h>
#include "skiplist.h"

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

void main(void) {
  skiplist_t s;
  int32_t r, z, i;

  while (1) {
    skiplist_init(&s, 32);

    skiplist_insert(&s, 32, NULL);
  	skiplist_insert(&s, 33, NULL);
  	skiplist_insert(&s, 33, NULL);
  	skiplist_insert(&s, 35, NULL);
  	skiplist_insert(&s, 36, NULL);

    skipnode_t* x;
    if ((x = skiplist_find(&s, 35)) != NULL) {
      printf("skiplist_find(&s, 35) = %d, %p\n", x->key, x);
    }
    if ((x = skiplist_find(&s, 36)) != NULL) {
      printf("skiplist_find(&s, 35) = %d, %p\n", x->key, x);
    }

  	skiplist_dump_all(&s);

    skiplist_delete(&s, 33);
    skiplist_delete(&s, 36);
    printf("after free key 33, 36\n");
  	skiplist_dump_all(&s);

    i = 1000000;
  	while (i--) {
  		r = rand()*rand();
  		if (!skiplist_find(&s, r)){
        skiplist_insert(&s, r, NULL);
      }
  	}
    skiplist_destroy(&s);
    printf("destroy ok");
  }
}
