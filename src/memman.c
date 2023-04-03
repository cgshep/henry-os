#include "memman.h"

extern volatile uint32_t stack_base;
extern volatile uint32_t stack_top;
extern volatile uint32_t heap_base;
extern volatile uint32_t heap_top;

static uint32_t global_heap_size, global_stack_size;
static uint8_t *global_heap_ptr;

int memman_init()
{
    global_stack_size = (uint32_t)&stack_top -  (uint32_t)&stack_base;
    global_heap_size = (uint32_t)&heap_top - (uint32_t)&heap_base;
    global_heap_ptr = &heap_base;

    return 0;
}

uint32_t kmalloc(uint32_t size)
{
    return 0;
}

int global_reg_info(char **args, int n_args)
{
    return 0;
}

int global_mem_info(char **args, int n_args)
{
    puts("stack_bottom: ");
    puthex((uint32_t)&stack_base);
    puts(", stack_top: ");
    puthex((uint32_t)&stack_top);
    puts(", stack size: ");
    puthex((uint32_t)global_stack_size);
    puts("\n");

    puts("heap_bottom: ");
    puthex((uint32_t)&heap_base);
    puts(", heap_top: ");
    puthex((uint32_t)&heap_top);
    puts(", heap size: ");
    puthex((uint32_t)global_heap_size);
    puts(", heap ptr: ");
    puthex((uint32_t)global_heap_ptr);
    
    return 0;
}
