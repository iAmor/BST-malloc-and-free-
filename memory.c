#include<stdio.h>
#include<stdlib.h>
#include "memory.h"

/* memory_new
 * create a new memory struct, initialze its address and size
 */
memory* memory_new(void *addr, unsigned int size){
  memory* m = (memory*)malloc(sizeof(memory));
  m->addr = addr;
  m->size = size;
  return m;
}

/* free the dynamically allocated memory struct
 */
void memory_free(void* p){
  memory* m = (memory*)p;
  free(m);
}

/* compare two memory variables x and y by address 
 * if x is less than y, return -1
 * if x is greater than y, return 1
 * if they are equal, return 0
 */
int memory_addr_cmp(const void* x, const void* y){
	memory *m1 = (memory *) x;
	memory *m2 = (memory *) y;
	int addr1 = (int) m1->addr;
	int addr2 = (int) m2->addr;
	if (addr1 > addr2) {
		return 1;
	}
	else if (addr1 < addr2) {
		return -1;
	}
  return 0;
}

/* compare two memory variables x and y by size 
 * if x is less than y, return -1
 * if x is greater than y, return 1
 * if they are equal, return 0
 */
int memory_size_cmp(const void* x, const void* y){
  	memory *m1 = (memory *) x;
	memory *m2 = (memory *) y;
	if (m1->size > m2->size) {
		return 1;
	}
	else if (m2->size > m1->size) {
		return -1;
	}
	return 0;
}

/* print the memory address and size
 */
void memory_print(void* data){
  if (data == NULL) return;
  memory* m = (memory*)data;
  printf("address: %p, size: %u\n", m->addr, m->size);
}

memory *allocate_memory_page()
{
	memory *retval = malloc(sizeof(memory));
	retval->addr = malloc(4096);
	retval->size = 4088;
	return retval;
}

void *split_memory(memory* data, unsigned int size_desired)
{
	int advsize = (data->size - size_desired + 8);
	char *retval = (char *) data->addr;
	retval += advsize;
	data->size = data->size - size_desired - 8;
	*(retval - 8) = size_desired;
	return (void *) retval;
}

memory *merge_memory(memory *first, memory *second)
{
	memory *retval = malloc(sizeof(memory));
	return retval;
}
