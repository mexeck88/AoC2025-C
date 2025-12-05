/**
 * @file chal4.h
 *
 * @brief This is the header file for Advent of Code 2025 Challenge 4.
 *
 * @author Mexeck88
 *
 * @date 02DEC25
 */

#ifndef CHAL4_H
#define CHAL4_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

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
    NEIGHBOR_COUNT = 4
} numbers_t;

/**
 * @enum line_info_t
 * @brief Line information constants
 */
typedef enum line_info_t
{
    LINE_SIZE  = 140, // Actual data size
    NUM_LINES  = 140, // Number of data lines in input
    GRID_SIZE  = 142, // 140 + 2 (padding rows/cols)
} line_info_t;

/**
 * @enum retcode_t
 * @brief Return code constants
 */
typedef enum retcode_t
{
    RET_SUCCESS      = 0,
    RET_FAILURE      = 1,
    RET_NULL_POINTER = 2,
} retcode_t;

/**
 * @struct main_args_t
 * @brief Main arguments structure containing input lines and solutions
 */
typedef struct main_args_t
{
    char ** pp_lines;
    int     line_count;
    int     grid[GRID_SIZE][GRID_SIZE]; // 141x141: padding on edges + 139x139 data
    long    solution_1;
    long    solution_2;

} main_args_t;

static uint8_t chal4_load_input (const char *  p_file_path,
                                 main_args_t * p_main_args);
static uint8_t chal4_process_input (main_args_t * p_main_args);
static uint8_t chal4_process_line_part1 (main_args_t * p_main_args);
// static uint8_t chal4_process_line_part2 (const char *  p_line, main_args_t * p_main_args);

#endif /* CHAL3_H  */