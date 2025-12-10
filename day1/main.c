#include <stdio.h>
#include <limits.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdlib.h>

#define DIAL_UNIQUE_POS ((int)100)
#define DIAL_STARTING_VAL ((int) 50)
#define DIAL_LEFT_C 'L'
#define DIAL_RIGHT_C 'R'

static int current_val = DIAL_STARTING_VAL;
static size_t total_marked = 0;

typedef enum {
    DIAL_NONE = 0,
    DIAL_RIGHT = 1,
    DIAL_LEFT = 2,
} rotate_dial_e;

static void rotateDial(rotate_dial_e dir, int val) {
    int prev = current_val;

    if (dir == DIAL_NONE) {
        return;    
    }
    
    total_marked += val / DIAL_UNIQUE_POS;
    val %= DIAL_UNIQUE_POS;
    
    if (val == 0) return;

    if (dir == DIAL_LEFT) {
        current_val -= val;
    } else if (dir == DIAL_RIGHT) {
        current_val += val;
    } else {
        printf("Error undefined rotation dir!\n");
        return;
    }

    if (current_val == 0) {
        ++total_marked;
    } else if (current_val < 0) {
        if (prev != 0) ++total_marked;
        current_val += DIAL_UNIQUE_POS;
    } else if (current_val >= DIAL_UNIQUE_POS) {
        ++total_marked;
        current_val -= DIAL_UNIQUE_POS;
    }
}

void parseLine(char* line, size_t len) {
    char* endptr;
    int num;

    rotate_dial_e op = DIAL_NONE;
    
    if (len == 0) {
        return;
    }

    if (line[0] == DIAL_LEFT_C) {
        op = DIAL_LEFT;
    } else if (line[0] == DIAL_RIGHT_C) {
        op = DIAL_RIGHT;
    }

    num = strtod(&line[1], &endptr);
    rotateDial(op, num);
}

int main(int argc, char** argv) {
    FILE* input = fopen(argv[1], "r");
    size_t len = 0;
    ssize_t read;
    char* line = NULL;

    if (!input) {
        printf("[MAIN] ERROR: Could not open file\n");
        return 1;
    }

    while ((read = getline(&line, &len, input)) != -1) {
        parseLine(line, len);
    }

    printf("Answer: %lu\n", total_marked);

    return 0;
}
