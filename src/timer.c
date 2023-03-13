#include "timer.h"
#include "string.h"
#include "pic.h"

uint32_t timer_ticks=0;

void timer_handler()
{
    timer_ticks++;
    if(timer_ticks % 18 == 0) {
	puts("One second has elapsed!\n");
    }
}

void timer_install()
{
    //irq_install_handler(0, timer_handler);
}
