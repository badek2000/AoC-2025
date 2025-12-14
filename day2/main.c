#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>

#define DELIM_CHAR ','

typedef size_t aoc_id_t;
typedef bool(*checkIfValid_t)(aoc_id_t);

static bool checkIfIdValid_1(aoc_id_t id) {
    size_t number_of_digits = log10(id) + 1;
    if (number_of_digits % 2 == 1) {
        return true;
    }

    number_of_digits /= 2;
    number_of_digits = powl(10, number_of_digits);

    aoc_id_t first_half = id / number_of_digits;
    aoc_id_t second_half = id % number_of_digits;
    
    return (first_half != second_half);
}

static bool checkIfIdValid_2(aoc_id_t id) {
    size_t number_of_digits = (log10(id) + 1);
    aoc_id_t pow = 0;
    aoc_id_t seq = 0;

    for (size_t seq_len = 1; seq_len <= number_of_digits / 2; ++seq_len) {
        pow = (aoc_id_t)powl(10, (number_of_digits - seq_len));
        
        if (pow == 0) {
            continue;
        }
        seq = id / pow;

        if (number_of_digits % seq_len != 0) {
            continue;
        }

        size_t max_seq_no = number_of_digits / seq_len;
        aoc_id_t seq_cmp;
        
        bool valid = true;
        for (size_t seq_no = 1; seq_no <= max_seq_no; ++seq_no) {
            aoc_id_t seq_offset = seq_len * seq_no;
            seq_cmp = id / (aoc_id_t)powl(10, number_of_digits - seq_offset);
            seq_cmp = seq_cmp % (aoc_id_t)powl(10, seq_len);

            if (seq != seq_cmp) {
                valid = true;
                break; 
            }
            valid = false;
        }

        if (!valid) return false;
    }

    return true;
}

static aoc_id_t parseRange(char* line, checkIfValid_t checkIfIdValid) {
    char *endptr = NULL;

    aoc_id_t ret = 0;

    aoc_id_t bot = strtod(line, &endptr);
    aoc_id_t up = strtod(endptr + 1, &endptr);

    for (aoc_id_t i = bot; i <= up; ++i) {
        if (!checkIfIdValid(i)) {
            ret += i;
        }
    }

    return ret;
}

int main(int argc, char**argv) {
    (void)argc;
    
    char* file_name = argv[1];
    FILE *fp = fopen(file_name, "r");

    char* line = NULL;
    size_t n;

    aoc_id_t answer_1 = 0;
    aoc_id_t answer_2 = 0;

    if (fp == NULL) {
        fprintf(stderr, "Couldn't open a file [%s]", file_name);
        return -1;
    }
    
    while(0 < getdelim(&line, &n, DELIM_CHAR, fp)) {
        answer_1 += parseRange(line, checkIfIdValid_1);
        answer_2 += parseRange(line, checkIfIdValid_2);
    }
    
    printf("1: %lu\n", answer_1);
    printf("2: %lu\n", answer_2);

    return 0;
}