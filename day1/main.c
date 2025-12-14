#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "file_parser.h"

#define DIAL_UNIQUE_POS ((int)100)
#define DIAL_STARTING_VAL ((int) 50)
#define DIAL_LEFT_C 'L'
#define DIAL_RIGHT_C 'R'

typedef enum {
    DIAL_NONE = 0,
    DIAL_RIGHT = 1,
    DIAL_LEFT = 2,
} rotate_dial_e;

typedef struct _ctx_s {
    int current_val;
    size_t total_marked;
} ctx_t;

static void rotateDial(rotate_dial_e dir, int val, ctx_t* ctx) {
    int prev = ctx->current_val;

    if (dir == DIAL_NONE) {
        return;    
    }
    
    ctx->total_marked += val / DIAL_UNIQUE_POS;
    val %= DIAL_UNIQUE_POS;
    
    if (val == 0) return;

    if (dir == DIAL_LEFT) {
        ctx->current_val -= val;
    } else if (dir == DIAL_RIGHT) {
        ctx->current_val += val;
    } else {
        printf("Error undefined rotation dir!\n");
        return;
    }

    if (ctx->current_val == 0) {
        ++ctx->total_marked;
    } else if (ctx->current_val < 0) {
        if (prev != 0) ++ctx->total_marked;
        ctx->current_val += DIAL_UNIQUE_POS;
    } else if (ctx->current_val >= DIAL_UNIQUE_POS) {
        ++ctx->total_marked;
        ctx->current_val -= DIAL_UNIQUE_POS;
    }
}

void parseLine(char* line, size_t len, void* ctx) {
    assert(ctx);

    char* endptr;
    int num;

    rotate_dial_e op = DIAL_NONE;
    
    if (len == 0 || line[0] == '\n') {
        return;
    }

    if (line[0] == DIAL_LEFT_C) {
        op = DIAL_LEFT;
    } else if (line[0] == DIAL_RIGHT_C) {
        op = DIAL_RIGHT;
    } else {
        fprintf(stderr, "[ERROR] Invalid op: %c\n", line[0]);
        return;
    }

    num = strtod(&line[1], &endptr);
    if (num > INT_MAX || num < INT_MIN) {
        fprintf(stderr, "Number out of range %d\n", num);
        return;
    }
    
    rotateDial(op, num, (ctx_t*)ctx);
}

int main(int argc, char** argv) {
    (void)argc;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input_file>", argv[0]);
        return EXIT_FAILURE;
    }

    ctx_t ctx = {
        .current_val = DIAL_STARTING_VAL,
        .total_marked = 0,
    };

    AoC_parseFile(argv[1], parseLine, &ctx);
    printf("Answer: %lu\n", ctx.total_marked);

    return 0;
}
