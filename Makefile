CC				=	clang
CFLAGS			=	-target i386-unknown-elf -fno-exceptions -fno-builtin -fno-stack-protector -fno-asynchronous-unwind-tables -masm=intel
ifeq ($(BONUS), 1)
	CFLAGS		+= -D BONUS
endif
INCLUDES		=	-I includes/
C_SRCS			=	\
					display.c \
					io.c \
					kernel.c \
					screen.c \
					utils.c \
					$(addprefix interrupts/, \
						idt.c \
						pic.c \
						keyboard.c \
						interrupts.c \
					)

ASMC			=	nasm
ASMFLAGS		=	-f elf32
ASM_SRCS		=	\
					boot.s \
					interrupts.s

C_OBJS			=	$(addprefix C/,$(C_SRCS:.c=.o))
C_OBJS_DEPEND	=	${C_OBJS:.o=.d}
ASM_OBJS		=	$(addprefix ASM/,$(ASM_SRCS:.s=.o))
_OBJS			=	$(C_OBJS) $(ASM_OBJS)
OBJS			=	$(addprefix build/,$(_OBJS))

LD				=	clang
LD_FILE			=	linker.ld
LDFLAGS			=	-target i386-unknown-elf -nostdlib -static -fno-exceptions -fno-builtin -fno-stack-protector -Wl,--build-id=none -T linker.ld -masm=intel

KERNEL_BIN		=	iso/boot/kernel.bin
KERNEL_ISO		=	kernel.iso

DOCKER_IMAGE	=	kfs_build_tools


all:	$(KERNEL_ISO)

docker_image:
	docker build -t $(DOCKER_IMAGE) tools/

build: docker_image
	docker run -v $(PWD):/project -w /project -it $(DOCKER_IMAGE) make

build_bonus: docker_image
	docker run -v $(PWD):/project -w /project -it $(DOCKER_IMAGE) make BONUS=1

$(KERNEL_BIN): $(OBJS)
	$(LD) $(LDFLAGS) -o $(KERNEL_BIN) $(OBJS)

-include $(C_OBJS_DEPEND)
build/C/%.o: srcs/C/%.c
	@if [ ! -d $(dir $@) ]; then\
		mkdir -p $(dir $@);\
	fi
	$(CC) ${CFLAGS} -MMD -MF $(@:.o=.d) ${INCLUDES} -c $< -o $@


build/ASM/%.o: srcs/ASM/%.s
	@if [ ! -d $(dir $@) ]; then\
		mkdir -p $(dir $@);\
	fi
	$(ASMC) ${ASMFLAGS} $< -o $@

$(KERNEL_ISO): $(KERNEL_BIN)
	grub-mkrescue -o $(KERNEL_ISO) iso

run: $(KERNEL_ISO)
	qemu-system-i386 -cdrom $(KERNEL_ISO)

clean:
	rm -rf build/

fclean: clean
	rm -rf $(KERNEL_BIN) $(KERNEL_ISO)

re: fclean
	$(MAKE) all

.PHONY: all clean fclean re docker_image build build_bonus
