#ifndef IO_H
# define IO_H

# include <stdint.h>

uint8_t io_inb(uint16_t port);
void io_wait(void);
void io_outb(uint16_t port, uint8_t val);

#endif
