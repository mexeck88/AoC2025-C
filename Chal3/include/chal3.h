/**
 * @file chal3.h
 *
 * @brief This is the header file for Advent of Code 2025 Challenge 3.
 *
 * @author Mexeck88
 *
 * @date 02DEC25
 */

#ifndef CHAL3_H
#define CHAL3_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define FILE_PATH "src/input.txt"

/**
 * @enum numbers_t
 * @brief Numeric constants
 */
typedef enum numbers_t
{
    TWO     = 2,
    TWELVE  = 12,
    BASE_10 = 10,
} numbers_t;

/**
 * @enum line_info_t
 * @brief Line information constants
 */
typedef enum line_info_t
{
    LINE_SIZE = 101,
    NUM_LINES = 200,
} line_info_t;

/**
 * @enum retcode_t
 * @brief Return code constants
 */
typedef enum retcode_t
{
    RET_SUCCESS = 0,
    RET_FAILURE = 1,
    RET_NULL_POINTER = 2,
} retcode_t;

/**
 * @struct main_args_t
 * @brief Main arguments structure containing input lines and solutions
 */
typedef struct main_args_t
{
    char **   pp_lines;
    int       line_count;
    long      solution_1;
    long long solution_2;

} main_args_t;

static uint8_t chal3_load_input (const char *  p_file_path,
                                 main_args_t * p_main_args);
static uint8_t chal3_process_input (main_args_t * p_main_args);
static uint8_t chal3_process_line_part1 (const char *  p_line,
                                         main_args_t * p_main_args);
static uint8_t chal3_process_line_part2 (const char *  p_line,
                                         main_args_t * p_main_args);

#endif /* CHAL3_H  */