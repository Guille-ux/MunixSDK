CC = gcc
ASM = nasm
LD = i386-elf-ld

LDFLAGS = -nostdlib
ASMFLAGS = -f elf32
CFLAGS = -g -mno-sse -fno-stack-protector -fno-builtin -nostdlib -m32 -I$(LIBC_INCLUDE)

LINKER_SCRIPT = linker.ld

LIBC_SRC = libc/src
LIBC_INCLUDE = libc/include
LIBC_OBJ = build/libc

APP_SRC = src
APP_INCLUDE = include

BUILD = build

LIBC_ASM_LIST = $(LIBC_SRC)/crt0.asm $(LIBC_SRC)/syscalls.asm
LIBC_SRC_LIST = $(LIBC_SRC)/string.c $(LIBC_SRC)/ctype.c $(LIBC_SRC)/sys.c
LIBC_OBJ_LIST = $(LIBC_OBJ)/string.o $(LIBC_OBJ)/ctype.o $(LIBC_OBJ)/crt0.o $(LIBC_OBJ)/syscalls.o $(LIBC_OBJ)/sys.o

APP_SRC_LIST = $(SRC)/main.c # here your list of sources
APP_ASM_LIST = # if you use some assembly code
APP_OBJ_LIST = $(BUILD)/main.o # here your list of object files

APP_OUT = $(BUILD)/app.bin # change this name for whatever you want
