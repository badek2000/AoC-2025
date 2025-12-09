#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#define MAX_BATTERIES_ON_1 ((size_t)(2))
#define MAX_BATTERIES_ON_2 ((size_t)(12))

static size_t parseLine_1(char* line, size_t len) {
    char buf[MAX_BATTERIES_ON_1 + 1] = {}; // +1 for null termination
    buf[MAX_BATTERIES_ON_1] = '\0';

    char *endptr = NULL;
    static const int base = 10; 
    
    size_t max = 0;
    size_t new_val = 0;

    for (size_t i = 0; i < len; ++i) {
        buf[0] = line[i];
        for (size_t j = i + 1; j < len; ++j) {
            buf[1] = line[j];
            new_val = (size_t)strtol(buf, &endptr, base);
            if (new_val > max) {
                max = new_val;
            }
        }
    }

    return max;
}

static bool isNumeric(char c) {
    return c >= '0' && c <= '9';
}

static char findNextMax(char* line, size_t len, size_t *idx) {
    char max = 0;
    for(size_t i = *idx; i < len; ++i) {
        if (line[i] > max) {
            max = line[i];
            *idx = i;
        }
    }

    *idx += 1;
    return max;
}

static size_t parseLine_2(char* line, size_t len) {
    char buf[MAX_BATTERIES_ON_2 + 1] = {}; // +1 for null termination
    buf[MAX_BATTERIES_ON_2] = '\0';

    char *endptr = NULL;
    static const int base = 10;

    size_t max = 0;
    size_t new_val = 0;
    size_t batteries_on = 0;
    size_t idx = 0;

    while(batteries_on < MAX_BATTERIES_ON_2) {
        size_t max_len = len - (MAX_BATTERIES_ON_2 - batteries_on);
        buf[batteries_on] = findNextMax(line, max_len, &idx);
        ++batteries_on;
    }

    size_t ret = (size_t)strtol(buf, &endptr, base);
    return ret;
}

int main(int argc, char **argv) {
    FILE *fp = NULL;
    char* file_name = argv[1];
    char* line = NULL;
    size_t len = 0;
    ssize_t read = 0;

    uint64_t answer_1 = 0;
    uint64_t answer_2 = 0;

    fp = fopen(file_name, "r");

    if (fp == NULL) {
        fprintf(stderr, "[ERROR] Couldn't open a file: %s\n", file_name);
        return -1;
    }
    
    while (-1 != (read = getline(&line, &len, fp))) {
        answer_1 += parseLine_1(line, read);
        answer_2 += parseLine_2(line, read);
    }

    printf("Answer p1: %lu\n", answer_1);
    printf("Answer p2: %lu\n", answer_2);

    return 0;
}