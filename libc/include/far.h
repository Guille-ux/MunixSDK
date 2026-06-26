#ifndef MUNIXSDK_LIBC_FAR_H
#define MUNIXSDK_LIBC_FAR_H

#include <stdint.h>
#include <stddef.h>

int _read_far(uint32_t offset) __attribute__((cdecl));
void _write_far(int value, uint32_t offset) __attribute__((cdecl));

#endif
