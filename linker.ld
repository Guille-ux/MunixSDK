OUTPUT_FORMAT("binary")
ENTRY(_start)
SECTIONS
{
	. = 0x0; 
	.text BLOCK(4K) : ALIGN(4K) {
		*(.crt0)
		*(.text)
	}	
	.rodata BLOCK(32K) : ALIGN(4K) {
		*(.rodata)
	}
	.data BLOCK(16K) : ALIGN(4K) {
		*(.data)
	}
	.bss BLOCK(4M) : ALIGN(4K) {
		*(.bss)
		*(COMMON)
	}
}	
