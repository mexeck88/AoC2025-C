/**
 * @file chal3.c
 *
 * @brief Advent of Code 2025 Challenge 3 Solution
 *
 * @author Mexeck88
 *
 * @date 03DEC25
 */

#include "chal3.h"

/**
 * @brief Processes the input lines to compute the two solutions
 *
 * @param p_file_path Path to the input file
 * @param p_main_args Pointer to the main arguments structure containing input
 * lines
 *
 * @return RET_SUCCESS on success, RET_FAILURE on failure
 */
static uint8_t chal3_load_input (const char *  p_file_path,
                                 main_args_t * p_main_args)
{
    uint8_t retcode               = RET_FAILURE;
    char    buffer[LINE_SIZE + 1] = { 0 }; // fgets null terminator

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
        if (NUM_LINES + 1 <= p_main_args->line_count)
        {
            printf("ERROR: Exceeded maximum number of lines (%d)\n",
                   p_main_args->line_count);
            goto CLEAN;
        }

        p_main_args->pp_lines[p_main_args->line_count] = malloc(
            (LINE_SIZE) * sizeof(char)); // replace the \n with null terminator
        if (NULL == p_main_args->pp_lines[p_main_args->line_count])
        {
            printf("ERROR: Unable to allocate memory for line %d\n",
                   p_main_args->line_count);
            goto CLEAN;
        }

        strncpy(
            p_main_args->pp_lines[p_main_args->line_count], buffer, LINE_SIZE);
        p_main_args->pp_lines[p_main_args->line_count][LINE_SIZE - 1]
            = '\0'; // Null terminate

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
 * @brief Processes all input lines to compute solutions
 *
 * @param p_main_args Pointer to the main arguments structure containing input
 * lines
 *
 * @return true on success, false otherwise
 */
static uint8_t chal3_process_input (main_args_t * p_main_args)
{
    uint8_t      retcode = RET_FAILURE;
    const char * p_line  = NULL;
    // Loop through all lines
    if (NULL == p_main_args)
    {
        perror("ERROR: NULL pointer passed to process_input\n");
        goto EXIT;
    }

    for (int idx = 0; idx < p_main_args->line_count; idx++)
    {
        p_line = (const char *)p_main_args->pp_lines[idx];
        // Process line for part 1
        if (RET_FAILURE == chal3_process_line_part1(p_line, p_main_args))
        {
            perror("ERROR: Unable to process line for part 1");
            goto EXIT;
        }

        // Process line for part 2
        if (RET_FAILURE == chal3_process_line_part2(p_line, p_main_args))
        {
            perror("ERROR: Unable to process line for part 2");
            goto EXIT;
        }
    }

EXIT:
    retcode = RET_SUCCESS;
    return retcode;
}

/**
 * @brief Processes a single line for part 1 solution
 *
 * @param p_line The input line to process
 * @param p_main_args Pointer to the main arguments structure
 *
 * @return RET_SUCCESS on success, RET_FAILURE on failure
 */
static uint8_t chal3_process_line_part1 (const char *  p_line,
                                         main_args_t * p_main_args)
{
    uint8_t retcode     = RET_FAILURE;
    long    max_sum     = 0;
    long    num_1       = 0;
    long    num_2       = 0;
    long    sum         = 0;
    char *  p_endptr    = NULL;
    char    buffer[TWO] = { 0 };

    if ((NULL == p_line) || (NULL == p_main_args))
    {
        perror("ERROR: NULL pointer passed to process_line_part1\n");
        goto EXIT;
    }

    for (int idx = 0; idx < LINE_SIZE - 1; idx++)
    {
        for (int jdx = idx + 1; jdx < LINE_SIZE - 1; jdx++)
        {
            memcpy(buffer, &p_line[idx], 1);
            buffer[1] = '\0';

            num_1 = strtol(buffer, &p_endptr, BASE_10);
            if (p_endptr == &buffer[0])
            {
                printf("ERROR: Unable to parse first number: %d\n", buffer[0]);
                p_endptr = NULL;
                goto EXIT;
            }

            memset(buffer, 0, TWO);
            memcpy(buffer, &p_line[jdx], 1);
            buffer[1] = '\0';

            num_2 = strtol(buffer, &p_endptr, BASE_10);
            if (p_endptr == &buffer[0])
            {
                printf("ERROR: Unable to parse second number: %d\n", buffer[0]);
                p_endptr = NULL;
                goto EXIT;
            }

            sum = num_1 * BASE_10 + num_2;
            if (max_sum < sum)
            {
                max_sum = sum;
            }
        }
    }

    p_main_args->solution_1 += max_sum;
    retcode = RET_SUCCESS;
EXIT:
    return retcode;
}

/**
 * @brief Processes a single line for part 2 solution (sliding window)
 *
 * @param p_line The input line to process
 * @param p_main_args Pointer to the main arguments structure
 *
 * @return RET_SUCCESS on success, RET_FAILURE on failure
 */
static uint8_t chal3_process_line_part2 (const char *  p_line,
                                         main_args_t * p_main_args)
{
    uint8_t   retcode          = RET_FAILURE;
    int       deque[LINE_SIZE] = { 0 }; // Deque to store selected digits
    int       front            = 0;     // Front of deque
    int       rear             = -1;    // Rear of deque
    long long result           = 0;     // Result from 12 digits
    int       line_len         = 0;
    int       selected_count   = 0;
    int       remaining_digits = 0;
    int       digit            = 0;
    int       full_digits      = 0;

    if ((NULL == p_line) || (NULL == p_main_args))
    {
        printf("ERROR: NULL pointer passed to process_line_part2\n");
        goto EXIT;
    }

    line_len = strlen(p_line);

    // Greedy algorithm with deque: select 12 largest digits while preserving
    // order
    for (int idx = 0; idx < line_len; idx++)
    {
        digit = p_line[idx] - '0'; // This is certainly not allowed in Barr C
        selected_count   = rear - front + 1;
        remaining_digits = line_len - idx - 1;

        // Remove smaller digits from rear if we can still reach 12 total
        while ((rear >= front) && (digit > deque[rear])
               && (TWELVE <= (selected_count - 1 + remaining_digits + 1)))
        {
            rear--;
            selected_count--;
        }

        // Add current digit if we haven't selected 12 yet
        if (selected_count < TWELVE)
        {
            deque[++rear] = digit;
        }
    }

    // Build result from selected 12 digits
    full_digits = (rear - front + 1 > TWELVE) ? TWELVE : (rear - front + 1);

    for (int idx = front; idx < front + full_digits; idx++)
    {
        result = result * BASE_10 + deque[idx];
    }

    p_main_args->solution_2 += result;
    retcode = RET_SUCCESS;
EXIT:
    return retcode;
}

/**
 * @brief Main function for Advent of Code 2025 Challenge 3
 *
 * @return int Exit code
 */
int main (void)
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

    // Load input file
    if (RET_FAILURE == chal3_load_input(FILE_PATH, p_main_args))
    {
        perror("ERROR: Unable to load input file\n");
        goto CLEAN;
    }

    // Process input file to get solutions
    if (RET_FAILURE == chal3_process_input(p_main_args))
    {
        perror("ERROR: Unable to process input file\n");
        goto CLEAN;
    }

    printf("Solution 1: %ld\n", p_main_args->solution_1);
    printf("Solution 2: %lld\n", p_main_args->solution_2);

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

// try to turn part 2 into barr C (Maybe review barr C to see what is NOT
// allowed for this section)