#ifndef MUNIXSDK_LIBC_MEM_H
#define MUNIXSDK_LIBC_MEM_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>



/*
 * IMPORTANT, HERE MALLOC USES MEMORY FROM THE EXTENDED USER SEGMENT
 * SO YOU HAVE TO STORE IT AND USE IT AS A __seg_fs VAR, IF NOT
 * IT ISN'T WORKING
 */


// thanks to the tutorial from Dan luu [https://danluu.com/malloc-tutorial/]
// a lot of things are from that, i mean, i have experience with buddy
// allocators but not with standard allocators and i think a buddy allocator
// isn't the best for user programs
// because of the tutorial is made for unix systems it has been a headache

int initMemSys(); // if you don't call this there can be some problems
		  // because we don't know if all the memory was zeroed
		  // before this program runs

// my sbrk imitation
void __seg_fs *sbrk(size_t size); // will only split a bigger block
			       // to lend the malloc some mem

#define SBRK_NEW_BLOCK_SIZE 1 // one page
#define SBRK_MAX_ALLOCATION 32

// malloc and free prototypes
void __seg_fs *malloc(size_t size);
void free(void __seg_fs *ptr);

// structs

// structs for sbrk
typedef struct _mem_pool {
	void __seg_fs *base;
	void __seg_fs *ptr;
	size_t size;
	bool full;
} __attribute__((packed)) _mem_pool_t;

// structs for malloc and free
// no uso el valor free xq si esta en la lista no lo necesitamos
typedef struct _block_meta {
	size_t size;
	struct _block_meta __seg_fs *next;
} __attribute__((packed)) _block_meta_t;

#define META_SIZE sizeof(_block_meta_t)

#endif
