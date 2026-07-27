#ifndef MUNIX_LIBC_SYS_H
#define MUNIX_LIBC_SYS_H

#include <stddef.h>
#include <stdint.h>
#include <ipc.h>
#include <stdbool.h>

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
void __seg_fs *_register_mem(int hmp);
int _read(int fd, void *buffer, uint32_t size);
int _write(int fd, void *buffer, uint32_t size);
int _spawn(uint32_t ram_amount, void *blob, uint32_t length, uint32_t start_pos); 
void _wait();
int _get_pid();
void _kill(int pid);
int _extend(int fd);

// 1 if program's own memory or 0 if far pointer (at m)
void _ipc_send(int pid, msg_t *message, bool m);
void _ipc_receive(msg_t *message, bool m);

void _awake(int pid);
int _searchPid(char *name);

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
#define _WAIT_ 0x00
#define _GETPID_ 0x01
#define _KILL_ 0x02
#define _IPC_SEND_ 0x03
#define _IPC_RECEIVE_ 0x04
#define _AWAKE_ 0x05
#define _SEARCH_PID_ 0x19
#define _EXTEND_ 0x0A

#endif
