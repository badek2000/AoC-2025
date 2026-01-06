#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>

#include "vec.h"
#include "file_parser.h"

#define RANGE_DELIM '-'

typedef uint64_t range_t;

typedef struct _ctx_s {
    AoC_vec_t ranges;
    size_t answer;
} ctx_t;

typedef int(*eq_t)(const void *a, const void *b);

static int rangeEq(const void* a, const void* b) {
    range_t _a = (range_t)a;
    range_t _b = (range_t)b;
    
    printf("%lu\n", _a - _b);

    return _a - _b;
}

static void addRange(uint64_t range_bot, uint64_t range_up, ctx_t* ctx) {
    if (!ctx) {
        return;
    }

    bool unique = false;

    unique = AoC_vecCheckIfUnique(&ctx->ranges, (const void*)&range_bot, rangeEq);
    printf("%lu is %s\n", range_bot, unique ? "UNIQUE" : "NOT UNIQUE");

    if (!unique) {
        printf("#########################################################\n");
    }

    AOC_VEC_PUSH_VAL(&ctx->ranges, range_t, range_bot);

    unique = AoC_vecCheckIfUnique(&ctx->ranges, (const void*)&range_up, rangeEq);
    printf("%lu is %s\n", range_up, unique ? "UNIQUE" : "NOT UNIQUE");

    if (!unique) {
        printf("#########################################################\n");
    }

    AOC_VEC_PUSH_VAL(&ctx->ranges, range_t, range_up);
}

static void parseRange(char* line, ctx_t *ctx) {
    if (!ctx) {
        fprintf(stderr, "[ERROR] BADARG\n");
        return;
    }

    const int base = 10;
    char* endptr = NULL;
    uint64_t range_bot = 0;
    uint64_t range_up = 0;

    errno = 0;

    range_bot = strtol(line, &endptr, base);    
    range_up = strtol((++endptr), &endptr, base);

    if (errno == ERANGE) {
        fprintf(stderr, "[ERROR] ERANGE\n");
        return;
    }

    addRange(range_bot, range_up, ctx);
}

static size_t parseId(char* line, size_t len) {
    (void)len;
    (void)line;
    return 0;
}

static void parseLine(char *line, size_t len, void* ctx) {
    if ((!line) || (!ctx)) {
        fprintf(stderr, "[ERROR] BADARG\n");
        return;
    }

    if (strlen(line) <= 0) return;

    char* delim_char = strchr(line, RANGE_DELIM);

    if (delim_char) {
        parseRange(line, ctx);
    } else {
        parseId(line, len);
    }
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input_file>", argv[0]);
        return EXIT_FAILURE;
    }

    ctx_t ctx = {0};
    AoC_vec_rc_e ret = AoC_vecInit(&ctx.ranges, sizeof(ctx_t));

    if (AOC_VEC_OK != ret) {
        fprintf(stderr, "[ERROR] %s\n", AoC_vecRcToString(ret));
        return ret; 
    }

    AoC_parseFile(argv[1], parseLine, &ctx);

    return 0;
}
