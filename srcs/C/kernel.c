#include "display.h"
#include "interrupts.h"
#include "screen.h"
#include "keyboard.h"

void kernel_main() {
#ifndef BONUS

	vga_clear();
	vga_putchar(0, 0, vga_char('4'));
	vga_putchar(1, 0, vga_char('2'));
	asm volatile("hlt");

#else

	idt_init();
	enable_interrupts();

	vga_clear();
	reset_screens();

	for(;;) {
		asm volatile("hlt");
		char c = get_last_pressed_char();
		if (c >= 11 && c <= 19) {
			switch_screen(c - 11);
		} else if (c != 0) {
			switch (c) {
				case '\n':
					add_newline_to_current_screen();
					break;
				case '\b':
					delete_last_char_from_current_screen();
					break;
				default:
					add_to_current_screen(c);
					break;
			}
		}
	}
	
#endif
}