#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mastik/fr.h"


int main() {

    fr_t fr;
    char *addr;
    int threshold;
    uint16_t results[1];

    printf("[*] Flush+Reload attacker (Mastik API)\n");

    addr = malloc(4096);
    addr[0] = 1;

    fr = fr_prepare();
    if (!fr) {
        printf("fr_prepare failed\n");
        return 1;
    }

    fr_monitor(fr, addr);

    threshold = fr_probethreshold();
    printf("[*] Threshold = %d cycles\n", threshold);

    while (1) {
        fr_probe(fr, results);

        if (results[0] < threshold)
            printf("HIT  %u cycles\n", results[0]);
        else
            printf("MISS %u cycles\n", results[0]);

        usleep(1000);
    }

    fr_release(fr);
    return 0;
}

// ubuntu@ubuntu-VirtualBox:~/Mastik/demo$ 
// gcc fr_attacker.c -o fr_attacker \
// -I.. -I../src \
// ../src/libmastik.a \
// -lpthread
// ubuntu@ubuntu-VirtualBox:~/Mastik/demo$ ./fr_attacker
// [*] Flush+Reload attacker (Mastik API)
// [*] Threshold = 208 cycles