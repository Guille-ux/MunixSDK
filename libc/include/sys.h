#ifndef MUNIX_LIBC_SYS_H
#define MUNIX_LIBC_SYS_H

#include <stddef.h>
#include <stdint.h>

extern uint32_t where;

int _sys(uint32_t type, int arg1, int arg2, int arg3, int arg4);

// syscalls

void _exit();
uint32_t _whoami();
int _openg(int fd);
int _open(char *name); // name relative to the far ptr
int _close(int fd);
int _name(char *name);
int _release_mem();
void *_register_mem(int hmp);
int _read(int fd, void *buffer, uint32_t size);
int _write(int fd, void *buffer, uint32_t size);
int _spawn(uint32_t ram_amount, void *blob, uint32_t length, uint32_t start_pos); 

// syscalls ids

#define _EXIT_ 0x17
#define _WHOAMI_ 0x10
#define _OPENG_ 0x0C
#define _OPEN_ 0x07
#define _CLOSE_ 0x13
#define _NAME_ 0x12
#define _RELEASE_MEM_ 0x11
#define _REIGSTER_MEM_ 0x0D
#define _READ_ 0x08
#define _WRITE_ 0x09
#define _SPAWN_ 0x06

#endif
