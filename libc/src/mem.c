#include <mem.h>

#define PAGE_SIZE 4096

static int initSbrkBlock(int index);

_block_meta_t *_block_pool = NULL;
_mem_pool_t _memory_pool[SBRK_MAX_ALLOCATION];
int how_many_blocks = 0;

int initMemSys() {
	memset(_memory_pool, 0, sizeof(_memory_pool));
}

static int initSbrkBlock(int index) {
	void __seg_fs *new_block = _register_mem(SBRK_NEW_BLOCK_SIZE);
	if (new_block == NULL) return -1;
	
	_memory_pool[index]->base = new_block;
	_memory_pool[index]->ptr = new_block;
	_memory_pool[index]->size = PAGE_SIZE*SBRK_NEW_BLOCK_SIZE;
	_memory_pool[index]->full = false;
	
	how_many_blocks++;

	return 0;
}

void __seg_fs *sbrk(size_t size) {
	if (_memory_pool[0] == NULL) {
		// add more memory
		initSbrkBlock(0);
	}
	int final_index;
	for (int i=0;i<SBRK_MAX_ALLOCATION;i++) {
		if ((size + ((size_t)_memory_pool[i]->ptr - (size_t)_memory_pool[i]->base)) > size || _memory_pool[i]->full) { // oh no, there isn't enough memory
			if (_memory_pool[i+1]->base != NULL) continue;
			if (i>=SBRK_MAX_ALLOCATION) return NULL;
			initSbrkBlock(++i);
			final_index = i;
			break;
		} else {
			final_index = i;
			break;
		}
	}
	if ((size + (size_t)_memory_pool[final_index]->ptr) == (_memory_pool[final_index]->size+(size_t)_memory_pool[final_index]->base)) {
		// oh no, all the memory has been used!
		_memory_pool[final_index]->full = true;
	}
	void __seg_fs *fptr = _memory_pool[final_index]->ptr;
	_memory_pool[final_index]->ptr = (void __seg_fs *)(size+(size_t)_memory_pool[final_index]->ptr);
	return fptr;
}

