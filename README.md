# Custom Memory Allocators - malloc(), free(), calloc() and realloc()

## Table of Contents
- [Implementation of Malloc and Free](#implementation-of-malloc-and-free)
- [Implementation of Calloc](#implementation-of-calloc)
- [Implementation of Realloc](#implementation-of-realloc)


## Implementation of Malloc and Free

Goal: Understanding malloc() and free() in C

The functions malloc() and free() are fundamental functions in C for dynamic memory management.
They allow programs to request and release memory at runtime.

- malloc(size_t size)
  - allocates a block of size bytes on the heap
  - returns a void pointer to the allocated memory or NULL if the allocation fails
  - memory is uninitialized (contains garbage values)

- free(void *ptr)
  - releases memory previously allocated by malloc(), calloc() and realloc()
  - failing to free() memory leads to memory leaks

Key Concepts
1. Block Structure: Each block/header contains metadata -> size, status(free), next pointer.
2. First-Fit Allocation: Scans list from beginning until finding first suitable free block.
3. Block Splitting: Divides large free blocks to minimize wasted space.
4. Coalescing: Merges adjacent free blocks to combat fragmentation.
5. sbrk() System Call: Used to request more memory from the OS.
6. Alignment: Ensures all allocations are 8-byte aligned for efficiency.

## Implementation of Calloc

Goal: Understanding calloc() in C

- calloc(size_t num, size_t size)
  - allocates memory for an array of (num) elements, each of size (size).
  - initializes all bytes to zero (differs from malloc(), which leaves memory uninitialized).
  - returns NULL if allocation fails.

- Steps of calloc()
  1. compute total size (num * size).
  2. use malloc() to allocate memory.
  3. zero-fill the allocated memory (using memset()).

Key Concepts
1. Allocates and zero-initializes memory.
2. malloc() + memset(0).


## Implementation of Realloc

Goal: Understanding realloc() in C

- realloc(void *ptr, size_t new_size)
  - resizes a previously allocated memory block (from malloc(), calloc() or realloc()).
  - if ptr is NULL, behaves like malloc(new_size).
  - if new_size is 0, behaves like free(ptr).
  - returns a pointer to the new memory block (or NULL on failure).

- Steps of realloc()
  1. if ptr is NULL, call malloc(new_size).
  2. if new_size is 0, call free(ptr).
  3. if possible, expand or shrink the existing block without moving data.
  4. if expansion is not possible:
    - allocate a new block with malloc(new_size)
    - copy old data to the new block
    - free old block

Key Concepts
1. resizes an existing memory block.
2. expands/shrinks or moves memory.
