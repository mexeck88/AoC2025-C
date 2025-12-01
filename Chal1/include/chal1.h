/**
 * @file chal1.h
 *
 * @brief This is the header file for Advent of Code 2025 Challenge 1.
 *
 * @author Mexeck88
 *
 * @date 01DEC25
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define STARTING_POINT  50
#define INIT_CAPACITY   50
#define MAX_LINE_LENGTH 8
#define DIAL_MAX        99
#define DIAL_MIN        0
#define REALLOC_SCALE   2
#define FILE_PATH       "src/chal1_input.txt"
#define RIGHT           "R"
#define LEFT            "L"

bool chal1_load_input (const char * p_file_path,
                       char ***     ppp_lines,
                       int *        p_line_count);
bool chal1_test_position (int current_position, int * password);
bool chal1_determine_steps (const char * p_line, int * p_rotation_steps);
int  chal1_count_zero_crossings (int start_position, int rotation_steps);

/** END OF FILE **/