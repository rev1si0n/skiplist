### compile & test
```bash
gcc -Wall -O3 *.c -o skiplist

 # insert & search 100000 random hash keys, default 2000000.
 # skiplist level set to 64
./skiplist 100000 64
```

> output
```
while(1000000) random hash test, skiplist level 64.

[DUMP NODES]
node 00000000026D1450 :: 5/0 :: key=-2147483648  level=64  order 0
node 00000000026D1670 :: 5/1 :: key=32           level=3   order 0
node 00000000026D16E0 :: 5/2 :: key=33           level=1   order 2
node 00000000026D16B0 :: 5/3 :: key=33           level=1   order 1
node 00000000026D1710 :: 5/4 :: key=35           level=1   order 3
node 00000000026D1740 :: 5/5 :: key=36           level=1   order 4

[AFTER FREE HASH 33, 36]
skiplist_find(&s, 35) = found, @00000000026D1710
skiplist_find(&s, 36) = not found

[DUMP NODES]
node 00000000026D1450 :: 5/0 :: key=-2147483648  level=64  order 0
node 00000000026D1670 :: 5/1 :: key=32           level=3   order 0
node 00000000026D16B0 :: 5/2 :: key=33           level=1   order 1
node 00000000026D1710 :: 5/3 :: key=35           level=1   order 3
base test finished.

994509 random hash keys inserted, 5491 keys exist, 1838ms
find 10923 random hash keys, 1585ms
destroy skiplist, 142ms
```
Done.
