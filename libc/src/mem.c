#include <mem.h>

#define PAGE_SIZE 4096


static void __seg_fs *initBlock(size_t size, _block_meta_t __seg_fs *next);
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
	
	_memory_pool[index].base = new_block;
	_memory_pool[index].ptr = new_block;
	_memory_pool[index].size = PAGE_SIZE*SBRK_NEW_BLOCK_SIZE;
	_memory_pool[index].full = false;
	
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
		if ((size + ((size_t)_memory_pool[i].ptr - (size_t)_memory_pool[i].base)) > size || _memory_pool[i].full) { // oh no, there isn't enough memory
			if (_memory_pool[i+1].base != NULL) continue;
			if (i>=SBRK_MAX_ALLOCATION) return NULL;
			initSbrkBlock(++i);
			final_index = i;
			break;
		} else {
			final_index = i;
			break;
		}
	}
	if ((size + (size_t)_memory_pool[final_index].ptr) == (_memory_pool[final_index].size+(size_t)_memory_pool[final_index].base)) {
		// oh no, all the memory has been used!
		_memory_pool[final_index].full = true;
	}
	void __seg_fs *fptr = _memory_pool[final_index].ptr;
	_memory_pool[final_index].ptr = (void __seg_fs *)(size+(size_t)_memory_pool[final_index].ptr);
	return fptr;
}

/*
 *
 * lista cosas hara malloc, revisara en su cache de bloques buscando uno
 * de un tamaño q sirva, si hay enviara ese, si no pedira uno nuevo
 *
 *
 *
 *
 *
 *
 *
 *
 */

static void __seg_fs *initBlock(size_t size, _block_meta_t __seg_fs *next) {
	_block_meta_t __seg_fs *block = sbrk(size);
	block->size = size;
	block->next = next;	
	return (void __seg_fs *)block;
}

void __seg_fs *malloc(size_t size) {
	size_t real_size = size + META_SIZE;
	if (_block_pool==NULL) { // ok, no tenemos bloques en cache
		return (void __seg_fs *)((size_t)initBlock(real_size, NULL) + META_SIZE)
	}
	_block_meta_t __seg_fs *last = NULL;
	_block_meta_t __seg_fs *current = _block_pool;
	while (1) {
		if (current == NULL) break;
		if (current->size!=real_size) { //no nos sirve
			last = current;
			current = current->next;
		} else {
			if (last==NULL) {
				_block_pool = current->next;
			} else {
				last->next = current->next;
			}
			return (void __seg_fs *)((size_t)current + META_SIZE);
		}
	}
	return (void __seg_fs *)((size_t)initBlock(real_size, NULL) + META_SIZE)

}

void free(void __seg_fs *ptr) {
	_block_meta_t __seg_fs *real_ptr = (_block_meta_t __seg_fs*)((size_t)ptr - META_SIZE);
	real_ptr->next = _block_pool;
	_block_pool = real_ptr;
}
