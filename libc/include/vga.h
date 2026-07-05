#ifndef MUNIXSDK_LIBC_VGA_H
#define MUNIXSDK_LIBC_VGA_H

#include <stdint.h>

// Colors
#define VGA_COLOR_BLACK 0
#define VGA_COLOR_BLUE 1
#define VGA_COLOR_GREEN 2
#define VGA_COLOR_CYAN 3
#define VGA_COLOR_RED 4
#define VGA_COLOR_MAGENTA 5
#define VGA_COLOR_BROWN 6
#define VGA_COLOR_LIGHT_GREY 7
#define VGA_COLOR_DARK_GREY 8
#define VGA_COLOR_LIGHT_BLUE 9
#define VGA_COLOR_LIGHT_GREEN 10
#define VGA_COLOR_LIGHT_CYAN 11
#define VGA_COLOR_LIGHT_RED 12
#define VGA_COLOR_LIGHT_MAGENTA 13
#define VGA_COLOR_LIGHT_BROWN 14 // aka Yellow
#define VGA_COLOR_WHITE 15

// Screen Special Def's
#define VGA_HEIGHT 25
#define VGA_WIDTH 80

#define VGA_SIZE VGA_HEIGHT * VGA_WIDTH

typedef struct {
	char ch;
	uint8_t attr;
} __attribute((packed)) vga_char_t;

#endif
