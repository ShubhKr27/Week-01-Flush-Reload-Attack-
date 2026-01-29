#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

#define ARRAY_SIZE (8 * 1024 * 1024)   // 8 MB

int main() {

    printf("[Victim] Prime+Probe Victim Started\n");

    char *array = malloc(ARRAY_SIZE);
    if (!array) {
        perror("malloc");
        return 1;
    }

    for (size_t i = 0; i < ARRAY_SIZE; i++)
        array[i] = 1;

    printf("[Victim] Accessing memory in loop...\n");

    size_t index = 0;

    while (1) {
        array[index]++;
        index = (index + 4096) % ARRAY_SIZE;  // jump across cache sets
    }

    return 0;
}

// gcc PP_VICTIM.c -O0 -o pp_victim

// sudo taskset -c 0 ./pp_victim
