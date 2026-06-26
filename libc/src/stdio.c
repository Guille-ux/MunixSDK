#include <stdio.h>

int _screen_fd = -1;
int _keyboard_fd = -1;

uint16_t __seg_fs *_screen=NULL;

int _cursor_x=0;
int _cursor_y=0;

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
