#include "interrupts.h"

void disable_interrupts() {
	asm volatile("cli");
}

void enable_interrupts() {
	asm volatile("sti");
}

void send_eoi(uint8_t irq) {
	if (irq >= 8) {
		send_byte_to_port(0xA0, 0x20); // EOI for slave PIC
	}
	send_byte_to_port(0x20, 0x20); // EOI for master PIC
}

void default_interrupt_handler() {
	send_byte_to_port(0xA0, 0x20);
	send_byte_to_port(0x20, 0x20);
}
