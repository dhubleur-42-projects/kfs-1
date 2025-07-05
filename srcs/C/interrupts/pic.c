#include "interrupts.h"

// Remap the Programmable Interrupt Controller (PIC) to use IRQs 0x20-0x2F
void pic_remap() {
    send_byte_to_port(0x20, 0x11); // Init master PIC
	send_byte_to_port(0x80, 0);
    send_byte_to_port(0xA0, 0x11); // Init slave PIC
	send_byte_to_port(0x80, 0);

    send_byte_to_port(0x21, 0x20); // Master PIC vector offset: 0x20
	send_byte_to_port(0x80, 0);
    send_byte_to_port(0xA1, 0x28); // Slave PIC vector offset: 0x28
	send_byte_to_port(0x80, 0);

    send_byte_to_port(0x21, 0x04); // Master tells slave is at IRQ2
	send_byte_to_port(0x80, 0);
    send_byte_to_port(0xA1, 0x02); // Slave identity
	send_byte_to_port(0x80, 0);

    send_byte_to_port(0x21, 0x01); // Master: 8086 mode
	send_byte_to_port(0x80, 0);
    send_byte_to_port(0xA1, 0x01); // Slave: 8086 mode
	send_byte_to_port(0x80, 0);

    send_byte_to_port(0x21, 0x0);  // Unmask all on master
    send_byte_to_port(0xA1, 0x0);  // Unmask all on slave
	send_byte_to_port(0x80, 0);
}
