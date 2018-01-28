### compile & test
```bash
gcc -Wall -O3 *.c -o skiplist

 # insert & search 100000 random hash keys, default 2000000.
 # skiplist level set to 64
./skiplist 100000 64
```

> output
```
while(2000000) random hash test, skiplist level 32.

[DUMP NODES]
node 00000000027013F0 :: 5/-1    :: key=-2147483648  level=32  insert order 0
node 0000000002701510 :: 5/0     :: key=32           level=3   insert order 0
node 0000000002701580 :: 5/1     :: key=33           level=1   insert order 2
node 0000000002701550 :: 5/2     :: key=33           level=1   insert order 1
node 00000000027015B0 :: 5/3     :: key=35           level=1   insert order 3
node 00000000027015E0 :: 5/4     :: key=36           level=1   insert order 4

[AFTER FREE HASH 33, 36]
skiplist_find(&s, 35) = found, @00000000027015B0
skiplist_find(&s, 36) = not found

[DUMP NODES]
node 00000000027013F0 :: 3/-1    :: key=-2147483648  level=32  insert order 0
node 0000000002701510 :: 3/0     :: key=32           level=3   insert order 0
node 0000000002701550 :: 3/1     :: key=33           level=1   insert order 1
node 00000000027015B0 :: 3/2     :: key=35           level=1   insert order 3
base test finished.

1978541 random hash keys inserted, 21459 keys exist, 4118ms
find 42220 random hash keys, 3835ms
destroy skiplist, 342ms
```

### Example
```c
skiplist_t list;
skipnode_t *node;
uint32_t test_value = 256;

// initialize
skiplist_init(&list, 32);
// add a key & object
skiplist_insert(&list, 256, (object*)&test_value);
// find a key
node = skiplist_find(&list, 256);
if (node != NULL) {
  printf("key 256 find, stored value is %d.\n", *((uint32_t*)node->object));
} else {
  puts("WTF");
}
// delete key
skiplist_delete(&list, 256);

node = skiplist_find(&list, 256);
if (node == NULL) {
  puts("key 256 deleted successfully.");
} else {
  puts("this not gonna be happen.");
}
```
Done.
