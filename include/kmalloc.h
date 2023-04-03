#ifndef MEMMAN_H
#define MEMMAN_H

#include <stdint.h>

uint32_t global

uint32_t kmalloc(size_t size)
{
    
    return 0;
}


int mem_info(char **args, int n_args)
{
    puts("kernel.c - stack_bottom: ");
    terminal_write_int((uint32_t)&stack_bottom);
    puts(", stack_top: ");
    terminal_write_int((uint32_t)&stack_top);
    puts(", stack size: ");
    terminal_write_int((uint32_t)&stack_top -
		       (uint32_t)&stack_bottom);

    puts("\n");
    puts("kernel.c - heap_bottom: ");
    terminal_write_int((uint32_t)&heap_bottom);
    puts(", heap_top: ");
    terminal_write_int((uint32_t)&heap_top);
    puts(", heap size: ");
    terminal_write_int((uint32_t)&heap_top -
		       (uint32_t)&heap_bottom);
    return 0;
}



#endif // MEMMAN_H
