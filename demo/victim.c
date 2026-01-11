// ✅ First: Why You Are Seeing Only MISS

// Flush+Reload gives HIT only if all these are true:

// ✅ Victim and attacker map same physical memory

// ✅ Victim actually accesses that exact cache line

// ✅ Cache is shared between cores (usually L3)

// ✅ Mastik monitor is set on correct address

// ✅ OS is not breaking sharing (e.g., copy-on-write)

// If any fails → always MISS.

#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

int main() {
    int fd = open("shared.bin", O_RDONLY);
    if (fd < 0) { perror("open"); return 1; }

    char *addr = mmap(NULL, 4096, PROT_READ, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED) { perror("mmap"); return 1; }

    printf("[Victim] Running and accessing memory...\n");

    while (1) {
        volatile char x = addr[0];
        (void)x;
        usleep(1000);   // 1 ms (much faster)
    }
}
