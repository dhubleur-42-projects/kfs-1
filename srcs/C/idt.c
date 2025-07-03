#include <stdint.h>

#define GATE_OFFSET(x)		(((((uint64_t)x) & 0xFFFF0000) << (48 - 16)) | (((x) & 0x0000FFFF)))
#define GATE_PRES(x)      (((uint64_t)x) << 47)			// Present
#define GATE_PRIV(x)     ((((uint64_t)x) & 3) << 45)		// Set privilege level (0 - 3)

#define GATE_TYPE_TASK			((uint64_t)0x5 << 40)
#define GATE_TYPE_16B_INTERRUPT	((uint64_t)0x6 << 40)
#define GATE_TYPE_16B_TRAP		((uint64_t)0x7 << 40)
#define GATE_TYPE_32B_INTERRUPT	((uint64_t)0xE << 40)
#define GATE_TYPE_32B_TRAP		((uint64_t)0xF << 40)

#define SEG_SELECTOR_INDEX(x)	((x) << (3+16))
#define SEG_SELECTOR_TI(x)		((x) << (2+16))		// 0 to use GDT, 1 to use LDT
#define SEG_SELECTOR_RPL(x)		(((x) & 0x3) << 16)		// Privilege level (0 - 3)

#define INTERRUPT_PL0 GATE_PRES(1) | GATE_PRIV(0) | GATE_TYPE_32B_INTERRUPT | SEG_SELECTOR_TI(0) | SEG_SELECTOR_RPL(0)

extern void keyboard_handler_wrapper();

extern uint32_t idt_table;

volatile uint8_t my_flag; //TEMP
struct interrupt_frame
{
    uint32_t ip;
    uint32_t cs;
    uint32_t flags;
    uint32_t sp;
    uint32_t ss;
};

//TODO AVOID uint64_t
void set_gate(uint8_t i_gate, uint32_t offset, uint16_t seg_selector_index, uint64_t flags)
{
    uint64_t descriptor;
	uint32_t *tmp_address; //TEMP
	tmp_address = &idt_table;
	descriptor = GATE_OFFSET(offset) | SEG_SELECTOR_INDEX(seg_selector_index) | flags;
	tmp_address[i_gate*2+1] = (descriptor & 0xFFFFFFFF00000000) >> 32;
	tmp_address[i_gate*2] = descriptor & 0xFFFFFFFF;
}

void io_outb(uint16_t, uint8_t);
uint8_t io_inb(uint16_t);
void io_wait();

void terminal_writestring(char const *str);
__attribute__ ((interrupt))
void interrupt_keyboard_handler (struct stack_frame *frame)
{
	(void)frame;
	
	my_flag = 1;
			terminal_writestring("OMG4");
	uint8_t a = io_inb(0x60);
			terminal_writestring("OMG4");
	io_outb(0x20,0x20); //TEMP TODO Use define
}

void terminal_writestring(char const *str);

void default_interrupt_handler_wrapper();

void pouet()
{
	//TEMP
	asm volatile("cli"); //TEMP
	for (int i = 1; i < 255; i++){
	set_gate(i, (uint32_t)&interrupt_keyboard_handler, 2, INTERRUPT_PL0);
}
//	set_gate(33, (uint32_t)&interrupt_keyboard_handler);
	asm volatile("sti"); //TEMP

	int i = 0;
	while (1)
	{
		asm volatile("hlt"); //TEMP
		if (i == 2)
			terminal_writestring("OMG3");
		if (my_flag == 1)
		{
			terminal_writestring("OMG");
			my_flag = 0;
			i += 1;
		}
	}
}



//OHR CODE
#if 0
void idt_set_gate(int n, uint32_t handler) {
    unsigned short cs;
    __asm__ ("mov %%cs, %0" : "=r" (cs));

    ((uint32_t *)idt_table)[n].base_low = handler & 0xFFFF;
    ((uint32_t *)idt_table)[n].base_high = (handler >> 16) & 0xFFFF;
    ((uint32_t *)idt_table)[n].selector = cs;
    ((uint32_t *)idt_table)[n].always0 = 0;
    ((uint32_t *)idt_table)[n].flags = 0x8E;
}
#endif

void keyboard_handler() {
    io_outb(0x20, 0x20);
    io_outb(0x20, 0x20);
	terminal_writestring("OMG8");
}
