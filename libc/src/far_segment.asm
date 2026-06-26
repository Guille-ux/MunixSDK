global _read_far
global _write_far

_read_far:
	mov ecx, [esp+4]
	mov eax, [fs:ecx]
	ret

_write_far:
	mov ecx, [esp+4]
	mov edx, [esp+8]

	mov [fs:edx], ecx
	ret
