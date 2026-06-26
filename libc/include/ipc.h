#ifndef MUNIX_SDK_LIBC_IPC_H
#define MUNIX_SDK_LIBC_IPC_H

#include <stdint.h>

// messages
typedef struct {
    int pid; // the one who sent this message
    uint8_t data[124]; // it isn't 128 because i wanted the messages were 128 bytes
} msg_t;

#endif

