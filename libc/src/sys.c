#include <sys.h>
#include <mem.h>

uint32_t where;

void _init() {
	// here is all the initialization code
	where = sys_whoami();
	initMemSys();
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

void _wait() {
	_sys(_WAIT_, 0, 0, 0, 0);
}

int _get_pid() {
	return _sys(_GETPID_, 0, 0, 0, 0);
}

void _kill(int pid) {
	_sys(_KILL_, pid, 0, 0, 0);
}

void _ipc_send(int pid, msg_t *message, bool m) {
	_sys(_IPC_SEND_, pid, (int)message, (int)m, 0);
}

void _ipc_receive(msg_t *message, bool m) {
	_sys(_IPC_RECEIVE_, (int)message, (int)m, 0, 0);
}

void _awake(int pid) {
	_sys(_AWAKE_, pid, 0, 0, 0);
}

int _searchPid(char *name) {
	return _sys(_SEARCH_PID_, (int)name, 0, 0, 0);
}
