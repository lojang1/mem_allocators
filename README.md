# Custom Memory Allocator - malloc(), free(), calloc() and realloc()

## Implementation of Malloc and Free in C
** mem_alloc.c
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

## Implementation of Calloc in C
** Coming Soon
