// Physical memory allocator, for user processes,
// kernel stacks, page-table pages,
// and pipe buffers. Allocates whole 4096-byte pages.

#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "riscv.h"
#include "defs.h"

#define REF_INDEX(pa) PGROUNDDOWN(pa) / PGSIZE

int refs[REF_INDEX(PHYSTOP) + 1];
struct spinlock ref_lock;
extern char end[];  // first address after kernel.
                    // defined by kernel.ld.
void kinit() {
  memset(refs, 0, REF_INDEX(PHYSTOP) * sizeof(int));
  initlock(&ref_lock, "ref lock");
  char *p = (char *)PGROUNDUP((uint64)end);
  bd_init(p, (void *)PHYSTOP);
}

void acquire_ref_lock() {
    acquire(&ref_lock);
}

void release_ref_lock() {
    release(&ref_lock);
}

void dec_ref(void *pa){
    uint index = REF_INDEX((uint64)pa);
    if (refs[index] <= 0) {
        panic("dec ref");
    }
    --refs[index];
}

void inc_ref(void *pa){
    uint index = REF_INDEX((uint64)pa);
    ++refs[index];
}

// Free the page of physical memory pointed at by v,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
void kfree(void *pa) {
    acquire_ref_lock();
    dec_ref(pa);
    if (refs[REF_INDEX((uint64)pa)] <= 0) {
        bd_free(pa);
    }
    release_ref_lock();
}

// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
void *kalloc(void) {
    void *pa = bd_malloc(PGSIZE);
    acquire_ref_lock();
    inc_ref(pa);
    release_ref_lock();
    return pa;
}
