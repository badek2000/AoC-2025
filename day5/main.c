#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "vec.h"
#include "file_parser.h"

#define RANGE_DELIM '-'

typedef struct _ctx_s {
    AoC_vec_t ranges;
    size_t answer;
} ctx_t;

static size_t parseRange(char* line, size_t len) {
    (void)line;
    (void)len;
    return 0;
}

static size_t parseId(char* line, size_t len) {
    (void)line;
    (void)len;
    return 0;
}

static void parseLine(char *line, size_t len, void* ctx) {
    (void)ctx;

    bool range = (strchr(line, RANGE_DELIM) == NULL) ? false : true;

    if (range) {
        parseRange(line, len);
    } else {
        parseId(line, len);
    }
}

int main(int argc, char **argv) {
        (void)argc;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input_file>", argv[0]);
        return EXIT_FAILURE;
    }

    ctx_t ctx = {0};

    AoC_parseFile(argv[1], parseLine, &ctx);

    return 0;
}
