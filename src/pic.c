#include "kernel.h"
#include "idt.h"
#include "pic.h"
#include "tty.h"
#include "keyboard.h"

/*
 * PIC ports
 */
#define PIC_MASTER_CMD_PORT      0x20
#define PIC_MASTER_DATA_PORT     0x21
#define PIC_SLAVE_CMD_PORT       0xA0
#define PIC_SLAVE_DATA_PORT      0xA1

/*
 * PIC commands
 */
#define PIC_CMD_END_OF_INTERRUPT 0x20
#define PIC_CMD_INITIALISE       0x11

/*
 * IRQ numbers
 */
#define PIC_TIMER_IRQ         0
#define PIC_KEYBOARD_IRQ      1
#define PIC_CASCADE_IRQ       2
#define PIC_COM2_IRQ          3
#define PIC_COM1_IRQ          4
#define PIC_LPT2_IRQ          5
#define PIC_FLOPPY_IRQ        6
#define PIC_LPT1_IRQ          7
#define PIC_CMOS_IRQ          8
#define PIC_FREE1_IRQ         9
#define PIC_FREE2_IRQ         10
#define PIC_FREE3_IRQ         11
#define PIC_PS2_IRQ           12
#define PIC_FPU_IRQ           13
#define PIC_PRIMARY_ATA_IRQ   14
#define PIC_SECONDARY_ATA_IRQ 15

void enable_interrupts()
{
    asm volatile("sti":);
}

uint8_t inb(uint16_t port)
{
    uint8_t ret;
    asm volatile("inb %1, %0"
		 : "=a"(ret)
		 : "Nd"(port));
    return ret;
}

void outb(uint16_t port, uint8_t val)
{
    asm volatile("outb %0, %1"
		 : // No output
		 : "a"(val), "Nd"(port));    
}

static inline void pic_send_eoi(uint8_t irq)
{
    if(irq >= 8) {
	outb(PIC_SLAVE_CMD_PORT, PIC_CMD_END_OF_INTERRUPT);
    }

    outb(PIC_MASTER_CMD_PORT, PIC_CMD_END_OF_INTERRUPT);
}


static inline void pic_io_wait()
{
    outb(0x80, 0);
}

void pic_remap()
{
   /*
    * The PIC's IRQs {0x0, 0xF} clash with protected
    * mode's CPU exceptions, {0x0, 0x1F}. We must remap
    * them IRQs lest we want the PIC's interrupts to
    * be mixed in with the CPU's exceptions.
    *
    * Here, we'll map them as {0x0,0xF} -> {0x20, 0x2F}.
    */
    //uint8_t master_pic_mask, slave_pic_mask;

    /*
     * Use the master and slave PICs' data ports
     * to get the current interrupt masks.
     */
    //master_pic_mask = inb(PIC_MASTER_DATA_PORT);
    //slave_pic_mask = inb(PIC_SLAVE_DATA_PORT);

    /*
     * First, issue the 0x11 initialise command to
     * the master and slave PICs' command port. 
     * The PICs will expect three "initialisation 
     * words" on the data port: the vector offset
     * for each PIC; how the master and slave are
     * wired, i.e. the slave is connected to IRQ2;
     * and what mode the PIC is in, i.e. 8068/88.
     */
    outb(PIC_MASTER_CMD_PORT, PIC_CMD_INITIALISE);
    pic_io_wait();
    outb(PIC_SLAVE_CMD_PORT, PIC_CMD_INITIALISE);
    pic_io_wait();
    // 0x20 is the base of our master IRQ map.
    // (IRQs 0x20 - 0x27)
    outb(PIC_MASTER_DATA_PORT, 0x20);
    pic_io_wait();
    // 0x28 is the base of the slave IRQ map.
    // (IRQs 0x28 - 0x2F)
    outb(PIC_SLAVE_DATA_PORT, 0x28);
    pic_io_wait();
    outb(PIC_MASTER_DATA_PORT, 0x4);
    pic_io_wait();
    outb(PIC_SLAVE_DATA_PORT, 0x2);
    pic_io_wait();
    outb(PIC_MASTER_DATA_PORT, 0x1);
    pic_io_wait();
    outb(PIC_SLAVE_DATA_PORT, 0x1);
    pic_io_wait();

    outb(PIC_MASTER_DATA_PORT, 0x0);
    outb(PIC_SLAVE_DATA_PORT, 0x0); 
}

extern void *irq_stub_table[];

void irq_handler()
{
    uint32_t irq_code;
    uint8_t scan_code;
    /*
     * A bit hacky to store the triggered IRQ in 
     * ESI, but it works; X86 doesn't use ESI 
     * during interrupts.
     */
    asm("":"=S"(irq_code));

#ifdef DEBUG_MODE
    puts("IRQ code: ");
    terminal_write_hex(irq_code);
    puts("\n");
#endif

    switch(irq_code) {
    case PIC_KEYBOARD_IRQ:
	scan_code = inb(0x60);
	process_key(scan_code);
#ifdef DEBUG_MODE
	puts("Scan code: ");
	terminal_write_hex(scan_code);
	puts("\n");
#endif
	pic_send_eoi(PIC_KEYBOARD_IRQ);
	break;
    default:
	break;
    }
}

void irq_set_mask(uint8_t irq_line) {
    uint16_t port;
    uint8_t value;
 
    if(irq_line < 8) {
        port = PIC_MASTER_DATA_PORT;
    } else {
        port = PIC_SLAVE_DATA_PORT;
        irq_line -= 8;
    }
    value = inb(port) | (1 << irq_line);
    outb(port, value);        
}
 
void irq_clear_mask(uint8_t irq_line) {
    uint16_t port;
    uint8_t value;
 
    if(irq_line < 8) {
        port = PIC_MASTER_DATA_PORT;
    } else {
        port = PIC_SLAVE_DATA_PORT;
        irq_line -= 8;
    }
    value = inb(port) & ~(1 << irq_line);
    outb(port, value);        
}

void enable_keyboard_irq()
{
    irq_set_mask(PIC_KEYBOARD_IRQ);
}

void pic_init()
{
    /*
     * 1. Remap the PICs' IRQs
     * 2. Set the IDT gates for IRQs 0x20 - 0x2F
     */
    pic_remap();
    for(size_t i = 0; i < 16; i++) {
	idt_set_gate(i+0x20, irq_stub_table[i], 0x08, 0x8E);
    }

    enable_keyboard_irq();
    outb(PIC_MASTER_DATA_PORT,0xfd);
    //outb(PIC_SLAVE_DATA_PORT,0xff);
}
