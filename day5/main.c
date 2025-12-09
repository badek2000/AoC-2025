#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define RANGE_DELIM '-'

static size_t parseRange(char* line, size_t len) {
    return 0;
}

static size_t parseId(char* line, size_t len) {
    return 0;
}

static size_t parseLine(char *line, size_t len) {
    bool range = (strchr(line, RANGE_DELIM) == NULL) ? false : true;

    if (range) {
        parseRange(line, len);
    } else {
        parseId(line, len);
    }

    return 0;
}

int main(int argc, char **argv) {
    char* file_name = argv[1];
    FILE *fp = NULL;

    size_t len = 0;
    ssize_t read = 0;
    char *line = NULL;

    fp = fopen(file_name, "r");
    if (fp == NULL) {
        fprintf(stderr, "[ERROR] Couldn't open a file: %s\n", file_name);
        return -1;
    }

    while (-1 != (read = getline(&line, &len, fp))) {
        parseLine(line, read);
    }

    return 0;
}
