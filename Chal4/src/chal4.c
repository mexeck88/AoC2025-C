/**
 * @file chal4.c
 *
 * @brief Advent of Code 2025 Challenge 4 Solution
 *
 * @author Mexeck88
 *
 * @date 03DEC25
 */

#include "chal4.h"

/**
 * @brief Processes the input lines to compute the two solutions
 *
 * @param p_file_path Path to the input file
 * @param p_main_args Pointer to the main arguments structure containing input
 * lines
 *
 * @return RET_SUCCESS on success, RET_FAILURE on failure
 */
static uint8_t
chal4_load_input (const char * p_file_path, main_args_t * p_main_args)
{
    uint8_t retcode               = RET_FAILURE;
    char    buffer[LINE_SIZE + 1] = { 0 }; // +1 for newline + null terminator

    if ((NULL == p_file_path) || (NULL == p_main_args))
    {
        perror("ERROR: NULL pointer passed to load_input\n");
        retcode = RET_NULL_POINTER;
        goto EXIT;
    }

    FILE * p_file = fopen(p_file_path, "r");
    if (NULL == p_file)
    {
        perror("ERROR: Unable to open file");
        goto EXIT;
    }

    p_main_args->pp_lines = malloc(sizeof(char *) * NUM_LINES);
    if (NULL == p_main_args->pp_lines)
    {
        perror("ERROR: Unable to allocate memory for lines");
        goto CLEAN;
    }

    // Copy into struct
    while (NULL != fgets(buffer, LINE_SIZE + 1, p_file))
    {
        // Check bounds before allocating
        if (NUM_LINES <= p_main_args->line_count)
        {
            printf("ERROR: Exceeded maximum number of lines (%d)\n", NUM_LINES);
            goto CLEAN;
        }

        p_main_args->pp_lines[p_main_args->line_count]
            = malloc((LINE_SIZE + 1) * sizeof(char));
        if (NULL == p_main_args->pp_lines[p_main_args->line_count])
        {
            printf("ERROR: Unable to allocate memory for line %d\n",
                   p_main_args->line_count);
            goto CLEAN;
        }

        strncpy(
            p_main_args->pp_lines[p_main_args->line_count], buffer, LINE_SIZE);

        char * p_newline
            = strchr(p_main_args->pp_lines[p_main_args->line_count], '\n');
        if (NULL != p_newline)
        {
            *p_newline = '\0';
        }
        else
        {

            p_main_args->pp_lines[p_main_args->line_count][LINE_SIZE] = '\0';
        }

        p_main_args->line_count++;
    }

    retcode = RET_SUCCESS;
    goto EXIT;

CLEAN:
    for (int idx = 0; idx < p_main_args->line_count; idx++)
    {
        if (NULL != p_main_args->pp_lines[idx])
        {
            free(p_main_args->pp_lines[idx]);
            p_main_args->pp_lines[idx] = NULL;
        }
    }

    if (NULL != p_main_args->pp_lines)
    {
        free(p_main_args->pp_lines);
        p_main_args->pp_lines = NULL;
    }

EXIT:
    if (NULL != p_file)
    {
        fclose(p_file);
    }

    return retcode;
}

/**
 * @brief Converts the input lines into the grid struct for processing
 *
 * @param p_main_args Pointer to the main arguments structure containing input
 * lines
 *
 * @return true on success, false otherwise
 */
static uint8_t
chal4_process_input (main_args_t * p_main_args)
{
    uint8_t      retcode = RET_FAILURE;
    const char * p_line  = NULL;

    if (NULL == p_main_args)
    {
        perror("ERROR: NULL pointer passed to process_input\n");
        goto EXIT;
    }

    // Read each line of the pp_lines, convert to 0/1 and store in grid
    // Store at grid[row+1] to leave padding at grid[0]
    for (int row = 0; row < p_main_args->line_count; row++)
    {
        p_line = p_main_args->pp_lines[row];

        for (int col = 0; col < LINE_SIZE; col++)
        {
            if (0 == strncmp(&p_line[col], "@", 1))
            {
                p_main_args->grid[row + 1][col + 1] = 1;
            }
            else
            {
                p_main_args->grid[row + 1][col + 1] = 0;
            }
        }
    }

    // Fill padding row and columns with 0s
    // Row 0 (top padding)
    for (int col = 0; col < GRID_SIZE; col++)
    {
        p_main_args->grid[0][col] = 0;
    }
    // Row 140 (bottom padding)
    for (int col = 0; col < GRID_SIZE; col++)
    {
        p_main_args->grid[GRID_SIZE - 1][col] = 0;
    }
    // Column 0 (left padding)
    for (int row = 0; row < GRID_SIZE; row++)
    {
        p_main_args->grid[row][0] = 0;
    }
    // Column 140 (right padding)
    for (int row = 0; row < GRID_SIZE; row++)
    {
        p_main_args->grid[row][GRID_SIZE - 1] = 0;
    }

EXIT:
    retcode = RET_SUCCESS;
    return retcode;
}

/**
 * @brief Checks the neighbors of each cell in the grid (reduces complexity)
 *
 * @param p_main_args Pointer to the main arguments structure containing input
 *
 * @return RET_SUCCESS on success, RET_FAILURE on failure
 */
static uint8_t
chal4_check_neighbors (main_args_t * p_main_args, int * neighbor_count, int row, int col)
{
    uint8_t retcode = RET_FAILURE;
    *neighbor_count = 0;

    if (neighbor_count == NULL)
    {
        perror("ERROR: NULL pointer passed to check_neighbors\n");
        goto EXIT;
    }

    if (NULL == p_main_args)
    {
        perror("ERROR: NULL pointer passed to check_neighbors\n");
        goto EXIT;
    }
    // Check 8 neighbors
    p_main_args->grid[row - 1][col - 1] == 1 ? (*neighbor_count)++
                                             : 0;                   // Top-Left
    p_main_args->grid[row - 1][col] == 1 ? (*neighbor_count)++ : 0; // Top
    p_main_args->grid[row - 1][col + 1] == 1 ? (*neighbor_count)++
                                             : 0;                   // Top-Right
    p_main_args->grid[row][col - 1] == 1 ? (*neighbor_count)++ : 0; // Left
    p_main_args->grid[row][col + 1] == 1 ? (*neighbor_count)++ : 0; // Right
    p_main_args->grid[row + 1][col - 1] == 1 ? (*neighbor_count)++
                                             : 0; // Bottom-Left
    p_main_args->grid[row + 1][col] == 1 ? (*neighbor_count)++ : 0; // Bottom
    p_main_args->grid[row + 1][col + 1] == 1 ? (*neighbor_count)++
                                             : 0; // Bottom-Right

    retcode = RET_SUCCESS;
EXIT:
    return retcode;
}


/**
 * @brief Processes the grid for part 1 solution
 *
 * @param p_grid Pointer to the grid structure
 *
 * @return RET_SUCCESS on success, RET_FAILURE on failure
 */
static uint8_t
chal4_process_line_part1 (main_args_t * p_main_args)
{
    uint8_t retcode        = RET_FAILURE;
    int     neighbor_count = 0;

    if (NULL == p_main_args)
    {
        perror("ERROR: NULL pointer passed to process_line_part1\n");
        goto EXIT;
    }

    // Read each element the grid and check its neighbors
    // Check rows 1-140 and columns 1-140 (the actual data, not padding)
    for (int row = 1; row < GRID_SIZE - 1; row++)
    {
        for (int col = 1; col < GRID_SIZE - 1; col++)
        {
            if (p_main_args->grid[row][col] == 1) // start check
            {
                if (RET_SUCCESS
                    != chal4_check_neighbors(p_main_args, &neighbor_count, row, col))
                {
                    perror("ERROR: Unable to check neighbors in part 1\n");
                    goto EXIT;
                }

                if (neighbor_count < NEIGHBOR_COUNT)
                {
                    p_main_args->solution_1++;
                }
            }
        }
    }

    retcode = RET_SUCCESS;
EXIT:
    return retcode;
}

/**
 * @brief Processes the grid for part 2 solution
 *
 * @param p_grid Pointer to the grid structure
 *
 * @return RET_SUCCESS on success, RET_FAILURE on failure
 */
static uint8_t
chal4_process_line_part2 (main_args_t * p_main_args)
{
    uint8_t retcode                         = RET_FAILURE;
    bool    b_stablized                     = false;
    int     neighbor_count                  = 0;
    int     test_count                      = 0;
    int     copy_grid[GRID_SIZE][GRID_SIZE] = { 0 };

    if (NULL == p_main_args)
    {
        perror("ERROR: NULL pointer passed to process_line_part2\n");
        goto EXIT;
    }

    // While loop that stops when the count is no longer updated
    while (false == b_stablized)
    {
        test_count = 0;
        // Copy grid
        for (int row = 0; row < GRID_SIZE; row++)
        {
            for (int col = 0; col < GRID_SIZE; col++)
            {
                copy_grid[row][col] = p_main_args->grid[row][col];
            }
        }

        // remove cells that have less than 4 neighbors
        for (int row = 1; row < GRID_SIZE - 1; row++)
        {
            for (int col = 1; col < GRID_SIZE - 1; col++)
            {
                if (p_main_args->grid[row][col] == 1) // start check
                {
                    if (RET_SUCCESS
                        != chal4_check_neighbors(p_main_args, &neighbor_count, row, col))
                    {
                        perror("ERROR: Unable to check neighbors in part 2\n");
                        goto EXIT;
                    }

                    if (NEIGHBOR_COUNT > neighbor_count)
                    {
                        // set copy grid cell to 0 so future checks aren't
                        // influenced
                        p_main_args->solution_2++;
                        copy_grid[row][col] = 0;
                    }
                }
            }
        }

        // Copy back to main grid (selected are now removed)
        for (int row = 0; row < GRID_SIZE; row++)
        {
            for (int col = 0; col < GRID_SIZE; col++)
            {
                p_main_args->grid[row][col] = copy_grid[row][col];
            }
        }

        // Check if the grid has stabilized
        for (int row = 1; row < GRID_SIZE - 1; row++)
        {
            for (int col = 1; col < GRID_SIZE - 1; col++)
            {
                if (p_main_args->grid[row][col] == 1) // start check
                {
                    if (RET_SUCCESS
                        != chal4_check_neighbors(p_main_args, &neighbor_count, row, col))
                    {
                        perror("ERROR: Unable to check neighbors in part 2\n");
                        goto EXIT;
                    }

                    if (neighbor_count < NEIGHBOR_COUNT)
                    {
                        test_count++;
                    }
                }
            }
        }

        if (0 == test_count)
        {
            b_stablized = true;
        }
    }

    retcode = RET_SUCCESS;

EXIT:
    return retcode;
}

/**
 * @brief Main function for Advent of Code 2025 Challenge 4
 *
 * @return int Exit code
 */
int
main (void)
{
    int retcode = 0;

    main_args_t * p_main_args = malloc(sizeof(main_args_t));
    if (NULL == p_main_args)
    {
        perror("ERROR: Unable to allocate memory for main args\n");
        goto EXIT;
    }

    p_main_args->solution_1 = 0;
    p_main_args->solution_2 = 0;
    p_main_args->pp_lines   = NULL;
    p_main_args->line_count = 0;
    memset(p_main_args->grid,
           0,
           sizeof(p_main_args->grid)); // Initialize grid to zero

    // Load input file
    if (RET_FAILURE == chal4_load_input(FILE_PATH, p_main_args))
    {
        perror("ERROR: Unable to load input file\n");
        goto CLEAN;
    }

    // Convert input to grid
    if (RET_FAILURE == chal4_process_input(p_main_args))
    {
        perror("ERROR: Unable to process input file to grid\n");
        goto CLEAN;
    }

    // Process input file to get solutions
    if (RET_FAILURE == chal4_process_line_part1(p_main_args))
    {
        perror("ERROR: Unable to process part 1\n");
        goto CLEAN;
    }

    if (RET_FAILURE == chal4_process_line_part2(p_main_args))
    {
        perror("ERROR: Unable to process part 2\n");
        goto CLEAN;
    }

    printf("Solution 1: %ld\n", p_main_args->solution_1);
    printf("Solution 2: %ld\n", p_main_args->solution_2);

    retcode = 1;

CLEAN:
    if (NULL != p_main_args)
    {
        if (NULL != p_main_args->pp_lines)
        {
            for (int idx = 0; idx < p_main_args->line_count; idx++)
            {
                if (NULL != p_main_args->pp_lines[idx])
                {
                    free(p_main_args->pp_lines[idx]);
                }
            }
            free(p_main_args->pp_lines);
        }
        free(p_main_args);
    }

EXIT:
    return retcode;
}

/** END OF FILE **/
