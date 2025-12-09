#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define EMPTY_CHAR '.'
#define PAPER_ROLL_CHAR '@'
#define ACCESS_PAPER_ROLL_CHAR 'x'

#define MAX_PAPER_ROLLS (4)

typedef enum {
    SPOT_EMPTY = 0,
    SPOT_OCCUPIED = 1,
    SPOT_ACCESSIBLE = 2,
} spot_state_e;

static spot_state_e **warehouse;
static size_t warehouse_size = 0;
static size_t warehouse_cap = 0;
static size_t warehouse_arr_size = 0;

static void destroyWarehouse(void) {
    for (size_t i = 0; i < warehouse_arr_size; ++i) {
        free(warehouse[i]);
    }

    free(warehouse);
}

static void printWarehouseState(void) {
    for (size_t i = 0; i < warehouse_size; ++i) {
        for (size_t j = 0; j < warehouse_arr_size; j++) {
            switch (warehouse[i][j])
            {
            case SPOT_EMPTY:
                printf("%c", EMPTY_CHAR);
                break;
            case SPOT_OCCUPIED:
                printf("%c", PAPER_ROLL_CHAR);
                break;
            case SPOT_ACCESSIBLE:
                printf("%c", ACCESS_PAPER_ROLL_CHAR);
            default:
                break;
            }
        }
        printf("\n");
    }
}

static bool paperAccessible(size_t _i, size_t _j) {
    size_t min_i = (_i == 0) ? 0 : (_i - 1); 
    size_t max_i = (_i > warehouse_size) ? warehouse_size : (_i + 1); 
    size_t min_j = (_j == 0) ? 0 : (_j - 1); 
    size_t max_j = (_j > warehouse_arr_size) ? warehouse_arr_size : (_j + 1);
    
    size_t not_accessable = 0;

    for (size_t i = min_i; i <= max_i; ++i) {
        for (size_t j = min_j; j <= max_j; ++j) {
            if (warehouse_size <= i || warehouse_arr_size <= j) {
                continue;
            }
            if (warehouse[i][j] != SPOT_EMPTY) {
                not_accessable += 1;
            }
        }
    }

    return (not_accessable <= MAX_PAPER_ROLLS);
}

static size_t analyzeWarehouseState(void) {
    size_t removed = 0;
    for (size_t i = 0; i < warehouse_size; ++i) {
        for (size_t j = 0; j < warehouse_arr_size; j++) {
            if (warehouse[i][j] != SPOT_OCCUPIED) {
                continue;
            }

            if (paperAccessible(i, j)) {
                warehouse[i][j] = SPOT_EMPTY;
                ++removed;
            }
        }
    }

    return removed;
}

static void parseLine(char* line, size_t len) {
    spot_state_e *arr = malloc(warehouse_arr_size * sizeof(spot_state_e));
    memset(arr, 0x00, sizeof(spot_state_e));

    for (size_t i = 0; i < len; ++i) {
        if (line[i] == EMPTY_CHAR) {
            arr[i] = SPOT_EMPTY;
        } else if (line[i] == PAPER_ROLL_CHAR) {
            arr[i] = SPOT_OCCUPIED;
        } else {
            fprintf(stderr, "[ERROR] Could not parse character: %c\n", line[i]);
            exit(EXIT_FAILURE);
        }
    }

    warehouse[warehouse_size] = arr;
    ++warehouse_size;
} 

int main(int argc, char **argv) {
    char* file_name = argv[1];
    FILE *fp = NULL;

    size_t len = 0;
    ssize_t read = 0;
    char *line = NULL;
    size_t arr_cnt = 0;

    fp = fopen(file_name, "r");
    if (fp == NULL) {
        fprintf(stderr, "[ERROR] Couldn't open a file: %s\n", file_name);
        return -1;
    }

    while (-1 != (read = getline(&line, &len, fp))) {
        ++arr_cnt;
        if (warehouse_arr_size < read) {
            warehouse_arr_size = read - 1; // -1 for null terminator
        }
    }

    printf("%lu\n", warehouse_arr_size);

    warehouse = malloc((arr_cnt) * sizeof(spot_state_e*));
    memset(warehouse, 0x00, sizeof(spot_state_e*));
    warehouse_cap = arr_cnt;

    rewind(fp);
    while (-1 != (read = getline(&line, &len, fp))) {
        parseLine(line, warehouse_arr_size);
    }

    printf("Warehouse:\n");
    printWarehouseState();

    size_t removed = 0;
    size_t removed_agg = 0;
    size_t round = 1;
    do {
        printf("Round %lu\n", round);
        removed = analyzeWarehouseState();

        printWarehouseState();

        removed_agg += removed;
        ++round;
    } while (removed != 0);
    
    destroyWarehouse();

    printf("Total removed: %lu\n", removed_agg);
    return 0;
}