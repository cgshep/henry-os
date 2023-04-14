#include "memman.h"
#include "string.h"

#include <stddef.h>

extern volatile uint32_t stack_base;
extern volatile uint32_t stack_top;
extern volatile uint32_t heap_base;
extern volatile uint32_t heap_top;

static uint32_t stack_base_addr, stack_top_addr;
static uint32_t heap_base_addr, heap_top_addr;

static uint32_t global_heap_size, global_stack_size;
static uint8_t *aligned_heap_ptr;

typedef struct mem_block {
    size_t size;
    struct mem_block *next_block;
} mem_block_t;

static mem_block_t *free_list;

#ifdef DEBUG_MEMMAN
void kmalloc_error()
{
    puts("kmalloc failure!\n");
}

void print_free_list()
{
    puts("\n@@@ free_list: ");
    mem_block_t *current = free_list;
    size_t i = 0;
    while(current) {
	puts("block ");
	putint(i);
	puts(" - addr: ");
	puthex((uint32_t)current);
	puts(", ->size= ");
	puthex((uint32_t)current->size);
	puts(", ->next_block: ");
	puthex((uint32_t)current->next_block);
	current = current->next_block;
	putch('\n');
	i++;
    }
}

void debug_memman()
{
    /*
     * Test kmalloc
     */
    size_t test_vec[] = {0x10, 0x100, 0x1000};
    uint32_t i, *a;

    for(i = 0; i < sizeof(test_vec)/sizeof(test_vec[0]); i++) {
	puts("\n*** kmalloc test start: ");
	puthex((uint32_t)test_vec[i]);
	puts(" ***\n");
	a = kmalloc(test_vec[i]);
	if (a != NULL) {
	    puts("->  kmalloc addr: ");
	    puthex((uint32_t)a);
	}
	else{
	    kmalloc_error();
	}
	print_free_list();
    }
}
#endif

int memman_init()
{
    stack_base_addr = (uint32_t)&stack_base;
    stack_top_addr = (uint32_t)&stack_top;
    heap_base_addr = (uint32_t)&heap_base;
    heap_top_addr = (uint32_t)&heap_top;

    global_stack_size = stack_top_addr - stack_base_addr;
    global_heap_size = heap_top_addr - heap_base_addr;
    free_list = (mem_block_t *)heap_base_addr;
    free_list->size = global_heap_size - sizeof(mem_block_t);
    free_list->next_block = NULL;
    return 0;
}

void *kmalloc(size_t size)
{
    if (size == 0) {
	puts("Invalid size!");
	return NULL;
    }

    mem_block_t *prev = NULL, *new_block = NULL;
    mem_block_t *current = free_list;

    while(current) {
	puts("current: ");
	puthex((uint32_t)&current);
	puts(", current->size: ");
	puthex((uint32_t)current->size);
	puts(", current->next_block: ");
	puthex((uint32_t)current->next_block);
	puts("\n");

	if (current->size >= size + sizeof(mem_block_t) + 1) {
	    /*
	     * Split the current block
	     */
	    new_block = (mem_block_t*)((uintptr_t)current + sizeof(mem_block_t) + size);
	    new_block->size = current->size - size - 2*sizeof(mem_block_t);
	    new_block->next_block = current->next_block;
	    
	    puts("new_block: ");
	    puthex((uint32_t)new_block);
	    puts(", new_block->size: ");
	    puthex((uint32_t)new_block->size);
	    puts(", new current->next: ");
	    puthex((uint32_t)current->next_block);
	    puts("\n");

	    if (prev) {
		prev->next_block = new_block;
	    }
	    else {
		free_list = new_block;
	    }
	    return (void *)((uint32_t*)current + sizeof(mem_block_t));
	}

	prev = current;
        current = current->next_block;
    }

    return NULL;
    /*if ((uint32_t)aligned_heap_ptr + aligned_size < &heap_top) {
	aligned_heap_ptr += aligned_size;
	puts("New aligned_heap_ptr: ");
	puthex((uint32_t)aligned_heap_ptr);
	puts("\n");
	return aligned_heap_ptr - aligned_size;
    }
    else {
	puts("NO MORE HEAP SPACE! :(");
	return NULL;
    }*/
}

int global_mem_info(char **args, int n_args)
{
    puts("&stack_bottom: ");
    puthex(stack_base_addr);
    puts(", &stack_top: ");
    puthex((uint32_t)stack_top_addr);
    puts(", stack size: ");
    puthex(global_stack_size);
    puts("\n");

    puts("&heap_base: ");
    puthex(heap_base_addr);
    puts(", &heap_top: ");
    puthex(heap_top_addr);
    puts(", heap size: ");
    puthex(global_heap_size);

    puts("\naligned_heap_ptr: ");
    puthex((uint32_t)aligned_heap_ptr);

#ifdef DEBUG_MEMMAN
    debug_memman();
#endif
    return 0;
}
