global io_inb
global io_wait
global io_outb

section .text
; uint8_t io_inb(uint16_t port)
io_inb:
	mov dx, [esp + 4]	; _port = port;
	in al, dx		; _val = in(port);
	ret			; return;

; void io_wait(void)
io_wait:
	push 0			; io_outb(0x80, 0);
	push 0x80		; ...
	call io_outb		; ...
	add esp, 8		; ...
	ret			; return;

; void io_outb(uint16_t port, uint8_t val)
io_outb:
	mov dx, [esp + 4]	; _port = port;
	mov al, [esp + 8]	; _val = val;
	out dx, al		; out(port, val);
	ret			; return;
