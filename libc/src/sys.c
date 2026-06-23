#include <sys.h>


uint32_t where;

void _init() {
	where = sys_whoami();
}

void _exit() {
	_sys(_EXIT_, 0, 0, 0, 0);
}

uint32_t _whoami() {
	return (uint32_t)_sys(_WHOAMI_, 0, 0, 0, 0);
}

int _openg(int fd) {
	return _sys(_OPENG_, fd, 0, 0, 0);
}

int _open(char *name) {
	return _sys(_OPEN_, (int)name, 0, 0, 0);
}

int _close(int fd) {
	return _sys(_CLOSE_, fd, 0, 0, 0);
}

int _name(char *name) {
	return _sys(_NAME_, (int)name, 0, 0, 0);
}

int _release_mem() {
	return _sys(_RELEASE_MEM_, 0, 0, 0, 0);
}

void *_register_mem(int hmp) {
	return (void*)_sys(_REGISTER_MEM, hmp, 0, 0, 0);
}

int _read(int fd, void *buffer, uint32_t size) {
	return _sys(_READ_, fd, (int)buffer, (int)size, 0);
}

int _write(int fd, void *buffer, uint32_t size) {
	return _sys(_WRITE_, fd, (int)buffer, (int)size, 0);
}

int _spawn(uint32_t ram_amount, void *blob, uint32_t length, uint32_t start_pos) {
	return _sys(_SPAWN_, (int)ram_amount, (int)blob, (int)length, (int)start_pos);
}
