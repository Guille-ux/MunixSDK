#include <stdio.h>
#include <sys.h>
#include <string.h>
#include <stdarg.h>

int _screen_fd = -1;
int _keyboard_fd = -1;

uint16_t __seg_fs *_screen=NULL;

int _cursor_x=0;
int _cursor_y=0;

uint8_t _color= (VGA_COLOR_BLACK << 4) | VGA_COLOR_WHITE;

int init_stdio() {
	_screen_fd = _openg(_SYS_SCREEN_FD);
	_keyboard_fd = _openg(_SYS_KEYBOARD_FD);

	if (_screen_fd==-1 || _keyboard_fd==-1) {
		return -1;
	}

	_screen = (uint16_t __seg_fs*)(uintptr_t)_register_mem(1); 
	// we only need like 4000 bytes so 4098 is enough
	if (_screen==NULL) {
		return -1;
	}

	// ahora toca limpiar la pantalla
	for (int i=0;i<VGA_HEIGHT*VGA_WIDTH;i++) {
		_screen[i] = (VGA_DEFAULT_COLOR << 8) | ' ';
	}

	return 0;
}

int refresh() {
	_write(_SYS_SCREEN_FD, _screen, VGA_SIZE);
	return 0;
}

int putch(char ch, int x, int y) {
	if (_screen == NULL) {
		return -1; // oh no, screen not initialized
	}
	_screen[x*y] = ch;
	_screen[x*y + 1] = _color;

	return 0;
}

int print_string(const char *string, int x, int y) {
	if (_screen == NULL) return -1;

	char *ch = string;
	while (ch!='\0') {
		if (x >= VGA_WIDTH) {
			x=0;
			y++;
		} else if (y >= VGA_HEIGHT) {
			return -1; // oh no, no hay suficiente espacio
		}
		putch(x++, y);
	}
	return 0;
}

void setcur(int x, int y) {
	_cursor_x = x;
	_cursor_y = y;
}

void scroll() {
	memcpy_seg((void*)((uintptr_t)_screen - (uintptr_t)VGA_WIDTH), _screen, VGA_SIZE);
	for (size_t i=0;i<VGA_WIDTH;i++) {
		putch(' ', i, VGA_HEIGHT);
	}

	_cursor_y--;
}

// AHORA LO DE PRINTF
static void uint_to_hex_string(size_t value, char *buffer) {
	const char hex_chars[]="0123456789ABCDEF";
	int i=0;

	if (value == 0) {
		buffer[0]='0';
		buffer[1]='\0';
		return;
	}
	char tmp_buffer[sizeof(size_t)*2+1];
	int k=0;

	while (value > 0) {
		tmp_buffer[k++] = hex_chars[value % 16];
		value /= 16;
	}

	while (k > 0) {
		buffer[i++]=tmp_buffer[--k];
	}
	buffer[i]='\0';
}

static void int_to_dec_string(int value, char *buffer) {
	int i=0;
	if (value==0) {
		buffer[0]='0';
		buffer[1]='\0';
		return;
	}
	bool negative=false;
	if (value < 0) {
		negative=true;
		value = -value;
	}
	
	char tmp_buffer[24];

	int k=0;

	while (value > 0) {
		tmp_buffer[k++] = (value % 10) + '0';
		value /=  10;
	}
	if (negative) tmp_buffer[k++]='-';
	
	while (k > 0) {
		buffer[i++]=tmp_buffer[--k];
	}

	buffer[i]='\0';
}

void putchar(char c) {

	switch (c) {
		case '\n': {
			size_t current_y = _cursor_y;
			size_t next_y = current_y + 1;

			size_t max_y = VGA_HEIGHT;
			if (next_y >= max_y) {
				scroll();
				next_y = max_y - 1;
			}
			setcur(0, next_y);
			return;
		}
		case '\r': {
			_cursor_x = 0;
			return;
			   }
		case '\t': {
			size_t current_x=_cursor_x;
			size_t next_x=8-(current_x%8);
			for (char i=0;i<next_x;i++) {
				putchar(' ');
			}
			return;
			   }
		default: {
			size_t current_x=_cursor_x;
			size_t current_y=_cursor_y;

			putch(c, current_x, current_y);

			size_t next_x=current_x+1;
			size_t next_y=current_y;
			if (next_x >= VGA_WIDTH) {
				next_x=0;
				next_y++;
				size_t max_y=VGA_HEIGHT;
				if (next_y>=max_y) {
					scroll();
					next_y=max_y-1;
				}
			}
			setcur(next_x, next_y);
			return;
			 }
	}
}

int printf(const char *format, ...) {
	va_list args;
	va_start(args, format);

	while (*format) {
		if (*format=='%') {
			format++;
			switch (*format) {
				case 's': {
						const char *str=va_arg(args, const char*);
						while (*str) {
							putchar(*str);
							str++;
						}
						break;
					  }
				case 'c': {
						char c=va_arg(args, char);
						char *b=&c;
						putchar(*b);
						break;
					  }
				case 'd': {
						int number=va_arg(args, int);
						char buffer[25];
						int_to_dec_string(number, buffer);
						char *buffptr=buffer;
						while (*buffptr) {
							putchar(*buffptr);
							buffptr++;
						}
						break;
					  }
				case 'l': {
						if (*(++format)!='u') {
							long num = va_arg(args, long);
							char buffer[64];
							ltoa(num, buffer, 10);
							char *b = buffer;
							while (*b) {
								putchar(*b);
								b++;
							}
							format--;
						} else {
							unsigned long n = va_arg(args, unsigned long);
							char buffer[128];
							char *b = buffer;
							ultoa(n, buffer, 10);
							while (*b) {
								putchar(*b);
								b++;
							}
						}
						break;
					   }
				case 'x': {
						unsigned long hex=va_arg(args, unsigned long);
						char buffer[64];
						ultoa(hex, buffer, 16);
						char *b=buffer;
						while (*b) {
							putchar(*b);
							b++;
						}
						break;
					  }
				case 'p': {
						unsigned long pointer=va_arg(args, unsigned long);
						char buffer[64]; 
						ultoa(pointer, &buffer[2], 16);
						buffer[0]='0';
						buffer[1]='x';
						char *b=buffer;
						while (*b) {
							putchar(*b);
							b++;
						}
						break;
					  }
				case 'f': {
						float num = va_arg(args, float);
						char buffer[64];
						ftoa(num, buffer, 6);

						char *b=buffer;
						while (*b) {
							putchar(*b);
							b++;
						}
						break;
					  }
				case '%': {
						putchar('%');
						break;
					  }
				default: {
					putchar('%');
					putchar(*format);
					break;
					 }
			}
		} else {
			putchar(*format);
		}
		format++;
	}

	va_end(args);
	return 0;
}
