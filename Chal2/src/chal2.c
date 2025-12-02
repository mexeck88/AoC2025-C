/**
 * @file chal2.c
 *
 * @brief Advent of Code 2025 Challenge 2 Solution
 *
 * @author Mexeck88
 *
 * @date 02DEC25
 */

#include "chal2.h"

/**
 * @brief Loads the input file and stores each element in an array
 *
 * @param p_file_path Path to the input file
 * @param ppp_lines Pointer to an array of strings to store the lines
 * @param p_line_count Pointer to an integer to store the number of elements
 *
 * @return true on success, false on failure
 */
bool chal2_load_input (const char * p_file_path,
                       char ***     ppp_lines,
                       int *        p_line_count)
{
    int  capacity   = INIT_CAPACITY;
    int  line_count = 0;
    bool b_retval   = false;

    if ((NULL == p_file_path) || (NULL == ppp_lines) || (NULL == p_line_count))
    {
        printf("ERROR: NULL pointer passed to load_input\n");
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

    FILE * p_file = fopen(p_file_path, "r");
    if (NULL == p_file)
    {
        printf("ERROR: Unable to open file %s\n", p_file_path);
        *ppp_lines    = NULL;
        *p_line_count = 0;
        free(pp_lines);
        goto EXIT;
    }

    fseek(p_file, 0, SEEK_END);
    long file_size = ftell(p_file);
    fseek(p_file, 0, SEEK_SET);

    char * p_file_buffer = malloc(file_size + 1);
    if (NULL == p_file_buffer)
    {
        printf("ERROR: Unable to allocate memory for file\n");
        fclose(p_file);
        free(pp_lines);
        goto EXIT;
    }

    fread(p_file_buffer, 1, file_size, p_file);
    p_file_buffer[file_size] = '\0';
    fclose(p_file);

    // Make copy for strtok
    char * p_file_buffer_copy = malloc(file_size + 1);
    if (NULL == p_file_buffer_copy)
    {
        printf("ERROR: Unable to allocate memory for file copy\n");
        free(p_file_buffer);
        free(pp_lines);
        goto EXIT;
    }

    strncpy(p_file_buffer_copy, p_file_buffer, file_size + 1);

    char * p_token = strtok(p_file_buffer_copy, TOKEN_DELIM);
    while (NULL != p_token)
    {
        if (line_count >= capacity)
        {
            capacity *= REALLOC_SCALE;
            char ** pp_temp = realloc(pp_lines, sizeof(char *) * capacity);

            if (NULL == pp_temp)
            {
                printf("ERROR: Unable to reallocate memory for lines\n");
                free(pp_lines);
                free(p_file_buffer_copy);
                free(p_file_buffer);
                goto EXIT;
            }
            pp_lines = pp_temp;
        }

        pp_lines[line_count] = malloc(strlen(p_token) + 1);
        if (NULL == pp_lines[line_count])
        {
            printf("ERROR: Unable to allocate memory for line %d\n",
                   line_count);
            free(p_file_buffer_copy);
            free(p_file_buffer);
            goto CLEAN;
        }

        strncpy(pp_lines[line_count], p_token, strlen(p_token) + 1);
        line_count++;
        p_token = strtok(NULL, TOKEN_DELIM);
    }

    free(p_file_buffer_copy);
    free(p_file_buffer);
    *ppp_lines    = pp_lines;
    *p_line_count = line_count;
    b_retval      = true;

CLEAN:
    if (false == b_retval)
    {
        for (int idx = 0; idx < line_count; idx++)
        {
            free(pp_lines[idx]);
        }
        free(pp_lines);
    }

EXIT:
    return b_retval;
}

/**
 * @brief Checks if the element is a valid ID and adds it to the password count
 *
 * @param p_password Current password value pointer
 * @param element Element to check
 *
 * @return true if the function succeeded, false otherwise
 */
bool chal2_is_value_counted (long * p_password, long element)
{
    bool b_retval  = false;
    bool b_matches = false;

    if (NULL == p_password)
    {
        printf("ERROR: NULL pointer passed to is_value_counted\n");
        goto EXIT;
    }

    // Convert element to string
    char element_str[ELEMENT_MAX];
    snprintf(element_str, ELEMENT_MAX, "%ld", element);

    int length = strlen(element_str);

    // Only even-length numbers can be made of a pattern repeated twice
    if (0 == length % 2)
    {
        int half = length / 2;

        b_matches = true;
        for (int idx = 0; idx < half; idx++)
        {
            // Compare first half with second half
            if (element_str[idx] != element_str[idx + half])
            {
                b_matches = false;
                break;
            }
        }
    }

    if (true == b_matches)
    {
        printf("Adding element to password (part 1): %ld\n", element);
        *p_password += element;
    }

    b_retval = true;

EXIT:
    return b_retval;
}

/**
 * @brief Checks if the element is a valid ID and adds it to the password count
 * (part 2)
 *
 * @param p_password Current password value pointer
 * @param element Element to check
 *
 * @return true if the function succeeded, false otherwise
 */
bool chal2_is_value_counted_part2 (long * p_password_two, long element)
{
    bool b_retval  = false;
    bool b_matches = false;

    if (NULL == p_password_two)
    {
        printf("ERROR: NULL pointer passed to is_value_counted\n");
        goto EXIT;
    }

    // Convert element to string
    char element_str[ELEMENT_MAX];
    snprintf(element_str, ELEMENT_MAX, "%ld", element);

    int length = strlen(element_str);

    // Check all possible pattern lengths
    for (int pattern_len = 1; pattern_len <= length / 2; pattern_len++)
    {
        // Only check if pattern can repeat at least twice
        if (0 == length % pattern_len)
        {
            int repeats = length / pattern_len;

            // If pattern repeats at least 2 times, check if it matches
            if (repeats >= 2)
            {
                b_matches = true;
                for (int idx = 0; idx < length; idx++)
                {
                    if (element_str[idx] != element_str[idx % pattern_len])
                    {
                        b_matches = false;
                        break;
                    }
                }

                if (true == b_matches)
                {
                    break;
                }
            }
        }
    }

    if (true == b_matches)
    {
        printf("Adding element to password (part 2): %ld\n", element);
        *p_password_two += element;
    }

    b_retval = true;

EXIT:
    return b_retval;
}

/**
 * @brief Processes a single element from the input (loop through its digits)
 *
 * @param p_element The element string to process
 * @param p_password Pointer to the current password value (to be passed into
 * chal2_is_value_counted)
 * @param p_password_two Pointer to the current password value for part 2
 *
 * @return true if the function succeeded, false otherwise
 */
bool chal2_process_element (const char * p_element,
                            long *       p_password,
                            long *       p_password_two)
{
    bool b_retval = false;

    // Null pointer check
    if ((NULL == p_element) || (NULL == p_password))
    {
        printf("ERROR: NULL pointer passed to process_element\n");
        goto EXIT;
    }

    // Delim p_element to get start and end of loop
    char * p_element_copy = malloc(strlen(p_element) + 1);
    if (NULL == p_element_copy)
    {
        printf("ERROR: Unable to allocate memory for element copy\n");
        goto EXIT;
    }

    strncpy(p_element_copy, p_element, strlen(p_element) + 1);
    char * p_start_str = strtok(p_element_copy, ELEMENT_DELIM);
    char * p_end_str   = strtok(NULL, ELEMENT_DELIM);

    long start = atol(p_start_str);
    long end   = atol(p_end_str);

    printf("Processing element: %s (from %ld to %ld)\n", p_element, start, end);

    // Loop from start to end
    for (long idx = start; idx <= end; idx++)
    {
        if (false == chal2_is_value_counted(p_password, idx))
        {
            printf("ERROR: Unable to check if value is counted: %ld\n", idx);
            free(p_element_copy);
            goto EXIT;
        }

        if (false == chal2_is_value_counted_part2(p_password_two, idx))
        {
            printf("ERROR: Unable to check if value is counted (part 2): %ld\n", idx);
            free(p_element_copy);
            goto EXIT;
        }
    }

    free(p_element_copy);
    b_retval = true;

EXIT:
    return b_retval;
}

/**
 * @brief Main function for Advent of Code 2025 Challenge 2
 *
 * @return int Exit code
 */
int main (void)
{
    int     retcode      = 1;
    long    password     = 0;
    long    password_two = 0;
    char ** pp_lines     = NULL;
    int     line_count   = 0;

    if (false == chal2_load_input(FILE_PATH, &pp_lines, &line_count))
    {
        printf("ERROR: Unable to load input file\n");
        goto EXIT;
    }

    // Call processing function on each element (part 1)
    for (int idx = 0; idx < line_count; idx++)
    {
        if (false
            == chal2_process_element(pp_lines[idx], &password, &password_two))
        {
            printf("ERROR: Unable to process element: %s\n", pp_lines[idx]);
            goto CLEAN;
        }
    }

    printf("Password Part 1: %ld\n", password);
    printf("Password Part 2: %ld\n", password_two);
    retcode = 0;

CLEAN:
    // To clean up pp_lines elements
    for (int idx = 0; idx < line_count; idx++)
    {
        if (NULL != pp_lines[idx])
        {
            free(pp_lines[idx]);
        }
    }

    if (NULL != pp_lines)
    {
        free(pp_lines);
    }

EXIT:
    return retcode;
}

/** END OF FILE **/