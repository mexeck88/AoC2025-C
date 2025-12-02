/**
 * @file chal2.h
 *
 * @brief This is the header file for Advent of Code 2025 Challenge 2.
 *
 * @author Mexeck88
 *
 * @date 02DEC25
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define INIT_CAPACITY   10
#define MAX_LINE_LENGTH 100
#define REALLOC_SCALE   2
#define ELEMENT_MAX     20
#define FILE_PATH       "src/input.txt"
#define TOKEN_DELIM     ","
#define ELEMENT_DELIM   "-"

bool chal2_load_input (const char * p_file_path,
                       char ***     ppp_lines,
                       int *        p_line_count);
bool chal2_process_element (const char * p_element, long * p_password, long * p_password_two);
bool chal2_is_value_counted (long * p_password, long element);
bool chal2_is_value_counted_part2 (long * p_password_two, long element);

/** END OF FILE **/