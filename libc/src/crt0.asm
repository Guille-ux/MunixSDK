global _start

extern main
extern sys_exit
extern _init

.section crt0

_start:
	call _init

	call main

	call sys_exit

.halt:
	jmp .halt
