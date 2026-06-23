global _sys


.section text

_sys:
	push ebx
	push edi

	mov eax, [esp+12]
	mov ebx, [esp+16]
	mov ecx, [esp+20]
	mov edx, [esp+24]
	mov edi, [esp+28]

	int 0x80
	
	pop edi
	pop ebx

	ret


