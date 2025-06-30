/* malloc and free */
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// Memory block metadata
typedef struct block {
  size_t size;
  int free;
  struct block *next;
} block_t;

// Head of the free list
static block_t *head = NULL;

// Memory alignment (8 bytes)
size_t align_size(size_t size) { return (size + 7) & ~7; }

void *m_alloc(size_t size) {
  if (size == 0)
    return NULL;

  block_t *current;
  block_t *prev;
  void *heap_ptr;

  // Align size with 8 bytes
  size = align_size(size);

  // Traverse the free list to find a suitable block
  current = head;
  while (current != NULL) {
    if (current->free && current->size >= size) {
      // Split block if remaining space is large enough
      if (current->size > size + sizeof(block_t)) {
        block_t *new_block =
            (block_t *)((char *)current + sizeof(block_t) + size);
        new_block->size = current->size - size - sizeof(block_t);
        new_block->free = 1;
        new_block->next = current->next;
        current->size = size;
        current->next = new_block;
      }
      current->free = 0;
      // Return memory after metadata
      return (void *)(current + 1);
    }
    prev = current;
    current = current->next;
  }

  // No suitable block found, request more memory from OS
  heap_ptr = sbrk(size + sizeof(block_t));
  // sbrk failed
  if (heap_ptr == (void *)-1)
    return NULL;

  // Initialize new block
  block_t *new_block = (block_t *)heap_ptr;
  new_block->size = size;
  new_block->free = 0;
  new_block->next = NULL;

  if (prev)
    prev->next = new_block;
  else
    head = new_block;

  // Return memory after metadata
  return (void *)(new_block + 1);
}

void free_mem(void *ptr) {
  if (ptr == NULL)
    return;

  // Get header from data pointer
  block_t *block_ptr = (block_t *)ptr - 1;
  block_ptr->free = 1;

  // Coalesce adjacent free blocks
  block_t *current = head;
  while (current != NULL && current->next != NULL) {
    if (current->free && current->next->free) {
      current->size += current->next->size + sizeof(block_t);
      current->next = current->next->next;
    }
    current = current->next;
  }
}

void *m_calloc(size_t num, size_t size) {
  size_t total_size = num * size;
  void *ptr = m_alloc(total_size);

  // Zero-fill memory
  if (ptr != NULL)
    memset(ptr, 0, total_size);

  return ptr;
}

void *m_realloc(void *ptr, size_t new_size) {
  if (ptr == NULL)
    return m_alloc(new_size);

  if (new_size == 0) {
    free_mem(ptr);
    return NULL;
  }

  block_t *block = (block_t *)((char *)ptr - sizeof(block_t));
  size_t old_size = block->size;

  if (new_size <= old_size)
    return ptr;

  void *new_ptr = m_alloc(new_size);
  if (new_ptr == NULL)
    return NULL;

  memcpy(new_ptr, ptr, old_size);

  free_mem(ptr);

  return new_ptr;
}

int main() {
  printf("[*] Implementation of malloc!\n");
  int *arr = (int *)m_alloc(10 * sizeof(int));
  if (arr == NULL) {
    printf("[!] Error: Memory allocation failed!\n");
    return 1;
  }

  for (int i = 0; i < 10; i++) {
    arr[i] = i;
    printf("%d\n", arr[i]);
  }
  free_mem(arr);

  printf("[*] Implementation of calloc!\n");
  int *arr1 = (int *)m_calloc(5, sizeof(int));
  if (arr1 == NULL) {
    printf("[!] Error: calloc failed!\n");
    return 1;
  }

  for (int i = 0; i < 5; i++) {
    printf("%d ", arr1[i]);
  }
  printf("\n");

  printf("[*] Implementation of realloc!\n");
  arr1 = (int *)m_realloc(arr1, 10 * sizeof(int));
  if (arr1 == NULL) {
    printf("[!] Error: realloc failed!\n");
    return 1;
  }

  for (int i = 5; i < 10; i++) {
    arr1[i] = i;
  }

  for (int i = 0; i < 10; i++) {
    printf("%d ", arr1[i]);
  }
  printf("\n");

  return 0;
}
