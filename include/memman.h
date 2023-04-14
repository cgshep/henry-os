#ifndef MEMMAN_H
#define MEMMAN_H

#include <stdint.h>
#include <stddef.h>

/*
 * Initialisation
 */
int memman_init();

/*
 * Console utility functions
 */
int global_mem_info(char **args, int n_args);

/*
 * Heap allocation functions
 */
void *kmalloc(size_t size);
void kfree(uint32_t addr);

#endif // MEMMAN_H
