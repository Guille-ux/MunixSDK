#ifndef STDIO_H
#define STDIO_H

#include <sys.h>
#include <far.h> // ya no es necesario pero se queda x si acaso
#include <vga.h>

#define _SYS_SCREEN_FD 1
#define _SYS_KEYBOARD_FD 0

extern int _screen_fd;
extern int _keyboard_fd;

extern uint16_t __seg_fs *_screen;
extern uint16_t __seg_fs *_kb_buffer;

extern int _cursor_x;
extern int _cursor_y;

extern uint8_t _color;

int init_stdio();
int refresh(); // escribe a la pantalla el buffer

// Screen Utils

int putch(char ch, int x, int y);
int print_string(const char *string, int x, int y);
void setcur(int x, int y);
void scroll();

// Screen Funcs, (things like printf, putchar)
void putchar(char c);


// keyboard funcs
uint16_t getchar(); // returns the last key event on the kernel buffer

// Keyboard Special Def's

#define SPECIAL_KEY (1 << 8)
#define KEY_BREAK (1 << 7)

#define KEY_NOT_PRINTABLE (1 << 9)
#define KEY_HOME SPECIAL_KEY | 0x47
#define KEY_APPS SPECIAL_KEY | 0x5D
#define KEY_ESC 0x01
#define KEY_BACKSPACE 0x0E
#define KEY_TAB 0x0F
#define KEY_ENTER 0x1C
#define KEY_ALT_GR SPECIAL_KEY | 0x38
#define KEY_BLOQ_MAYUS 0x3A
#define KEY_BLOQ_NUM 0x45
#define KEY_LEFT_SHIFT 0x2A
#define KEY_LEFT_ALT 0x38
#define KEY_RIGHT_SHIFT 0x36

// teclas de función
#define KEY_F1 0x3B
#define KEY_F2 0x3C
#define KEY_F3 0x3D
#define KEY_F4 0x3E
#define KEY_F5 0x3F
#define KEY_F6 0x40
#define KEY_F7 0x41
#define KEY_F8 0x42
#define KEY_F9 0x43
#define KEY_F10 0x44
#define KEY_F11 0x57
#define KEY_F12 0x58

// las flechas
#define KEY_ARROW_UP SPECIAL_KEY & 0x48
#define KEY_ARROW_DOWN SPECIAL_KEY & 0x50
#define KEY_ARROW_RIGHT SPECIAL_KEY & 0x4D
#define KEY_ARROW_LEFT SPECIAL_KEY & 0x4B

#endif
