/**
 * @file chal1.c
 *
 * @brief Advent of Code 2025 Challenge 1 Solution
 *
 * @author mexeck88
 *
 * @date 01DEC25
 */

#include "chal1.h"

/**
 * @brief Determines the number of steps to rotate the dial based on the input
 * line (left versus right)
 *
 * @param p_line The input line indicating the direction and steps
 * @param p_rotation_steps Pointer to store the number of rotation steps
 *
 * @return true on success, false otherwise
 */
bool chal1_determine_steps (const char * p_line, int * p_rotation_steps)
{
    bool b_retval = false;
    if (NULL == p_line || NULL == p_rotation_steps)
    {
        printf("ERROR: NULL pointer passed to determine_steps\n");
        goto EXIT;
    }

    // Grab steps
    char count[MAX_LINE_LENGTH];
    if (0 == snprintf(count, MAX_LINE_LENGTH, "%s", p_line + 1))
    {
        printf("ERROR: Unable to copy steps from line: %s\n", p_line);
        goto EXIT;
    }

    if (0 == strncmp(RIGHT, p_line, 1))
    {
        *p_rotation_steps = atoi(count);
    }
    else if (0 == strncmp(LEFT, p_line, 1))
    {
        *p_rotation_steps = -atoi(count);
    }
    else
    {
        printf("ERROR: Invalid direction in line: %s\n", p_line);
        goto EXIT;
    }

    b_retval = true;
EXIT:
    return b_retval;
}

/**
 * @brief Tests if the dial is at 0 and enumerates a password if so
 *
 * @param current_position Current position of the dial
 * @param password The current password value
 *
 * @return true on success, false otherwise
 */
bool chal1_test_position (int current_position, int * password)
{
    bool b_retval = false;
    printf("Testing position: %d\n", current_position);
    if (0 == current_position)
    {
        (*password)++;
        printf("  -> Landed on 0! Password now: %d\n", *password);
    }

    b_retval = true;
    return b_retval;
}

/**
 * @brief Counts how many times a rotation lands on position 0
 * (counts each step that lands on 0, not just final position)
 *
 * @param start_position The starting position of the dial
 * @param rotation_steps The number of steps to rotate
 *
 * @return The number of times position 0 is landed on during rotation
 */
int chal1_count_zero_crossings (int start_position, int rotation_steps)
{
    int count     = 0;
    int dial_size = DIAL_MAX + 1;

    if (0 == rotation_steps)
    {
        return 0;
    }

    printf(
        "Start Pos: %d, Rotation Steps: %d\n", start_position, rotation_steps);

    // Simulate each step of the rotation
    int current = start_position;

    if (0 < rotation_steps)
    {
        // Rotate right
        for (int i = 0; i < rotation_steps; i++)
        {
            current++;
            if (current >= dial_size)
            {
                current = 0;
            }
            if (current == 0)
            {
                count++;
            }
        }
    }
    else
    {
        // Rotate left
        for (int i = 0; i < -rotation_steps; i++)
        {
            current--;
            if (current < 0)
            {
                current = dial_size - 1;
            }
            if (current == 0)
            {
                count++;
            }
        }
    }

    return count;
}

/**
 * @brief Loads the input file and stores each line in an array
 *
 * @param p_file_path Path to the input file
 * @param ppp_lines Pointer to an array of strings to store the lines
 * @param p_line_count Pointer to an integer to store the number of lines
 *
 * @return true on success, false on failure
 */
bool chal1_load_input (const char * p_file_path,
                       char ***     ppp_lines,
                       int *        p_line_count)
{
    int  capacity   = INIT_CAPACITY;
    int  line_count = 0;
    bool b_retval   = false;

    if ((NULL == p_file_path) || (NULL == ppp_lines) || (NULL == p_line_count))
    {
        printf("ERROR: NULL pointer passed to load_input\n");
        *ppp_lines    = NULL;
        *p_line_count = 0;
        goto EXIT;
    }

    char ** pp_lines = malloc(sizeof(char *) * capacity); // Must be later freed
    if (NULL == pp_lines)
    {
        printf("ERROR: Unable to allocate memory for lines\n");
        *ppp_lines    = NULL;
        *p_line_count = 0;
        goto EXIT;
    }

    char buffer[MAX_LINE_LENGTH];

    FILE * p_file = fopen(p_file_path, "r");
    if (NULL == p_file)
    {
        printf("ERROR: Unable to open file %s\n", p_file_path);
        *ppp_lines    = NULL;
        *p_line_count = 0;
        free(pp_lines);
        goto EXIT;
    }

    while (NULL != fgets(buffer, MAX_LINE_LENGTH, p_file))
    {
        if (line_count >= capacity)
        {
            capacity *= REALLOC_SCALE;
            char ** pp_temp = realloc(pp_lines, sizeof(char *) * capacity);

            if (NULL == pp_temp)
            {
                printf("ERROR: Unable to reallocate memory for lines\n");
                fclose(p_file);
                free(pp_lines);
                goto EXIT;
            }
            pp_lines = pp_temp;
        }

        pp_lines[line_count] = malloc(strlen(buffer) + 1);
        if (NULL == pp_lines[line_count])
        {
            printf("ERROR: Unable to allocate memory for line %d\n",
                   line_count);
            fclose(p_file);
            goto EXIT;
        }

        memcpy(pp_lines[line_count], buffer, strlen(buffer) + 1);
        line_count++;
    }

    fclose(p_file);
    *ppp_lines    = pp_lines;
    *p_line_count = line_count;
    b_retval      = true;

EXIT:
    return b_retval;
}

int main (void)
{
    int     retcode          = 0;
    char ** pp_lines         = NULL;
    int     line_count       = 0;
    int     current_position = STARTING_POINT;
    int     password         = 0;
    int     rotation_steps   = 0;
    int     passes           = 0;

    // Load input file
    if (false == chal1_load_input(FILE_PATH, &pp_lines, &line_count))
    {
        printf("ERROR: Unable to load input file\n");
        retcode = 1;
        goto EXIT;
    }

    // Loop input array
    for (int idx = 0; idx < line_count; idx++)
    {
        // Determine rotation steps
        if (false == chal1_determine_steps(pp_lines[idx], &rotation_steps))
        {
            printf("ERROR: Unable to determine rotation steps\n");
            retcode = 1;
            goto EXIT;
        }

        // Part 2: Count zero crossings
        int zero_crossings
            = chal1_count_zero_crossings(current_position, rotation_steps);
        passes += zero_crossings;

        // Rotate dial
        current_position += rotation_steps;
        current_position
            = ((current_position % (DIAL_MAX + 1)) + (DIAL_MAX + 1))
              % (DIAL_MAX + 1);

        // Test position (part 1)
        if (false == chal1_test_position(current_position, &password))
        {
            printf("ERROR: Unable to test position\n");
            retcode = 1;
            goto EXIT;
        }
    }
    // Output password
    printf("Part 1 Password: %d\n", password);
    printf("Part 2 Password: %d\n", passes);
    retcode = 0;

EXIT:
    // Clean up allocated memory
    if (pp_lines != NULL)
    {
        for (int idx = 0; idx < line_count; idx++)
        {
            free(pp_lines[idx]);
        }
        free(pp_lines);
    }
    return retcode;
}

/** END OF FILE **/