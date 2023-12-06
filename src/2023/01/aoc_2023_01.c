#include "aoc_file.h"
#include "aoc_str.h"

#include <stdio.h>

// NOTE(lucas): Read until EOL or EOF, return null-terminated string
char* get_line(char* at, Arena* arena)
{
    char* start = at;
    while (*at && *at != '\n' && *at != '\r')
        ++at;
    
    i64 len = at - start;

    char* result = push_array(arena, len+1, char);
    for (usize i = 0; i < len; ++i)
        result[i] = start[i];
    
    result[-1] = 0;
    result[len] = '\0';
    return result;
}

u32 get_digit(char* line, b32 search_from_start)
{
    u32 result = 0;
    i64 len = str_len(line);
    if (search_from_start)
    {
        for (i64 i = 0; i < len; ++i)
        {
            if (char_is_digit(line[i]))
            {
                result = line[i] - '0';
                break;
            }
        }
    }
    else
    {
        for (i64 i = len-1; i >= 0; --i)
        {
            if (char_is_digit(line[i]))
            {
                result = line[i] - '0';
                break;
            }
        }
    }
    return result;
}

void solve_part_one(char* contents, Arena* arena)
{
    char* at = contents;
    
    u32 sum = 0;
    while (*at)
    {
        char* line = get_line(at, arena);
        usize line_len = str_len(line);

        at += line_len;
        if (*at == '\n')
            ++at;
        else if (*at == '\r')
            at += 2;

        u32 digit1 = get_digit(line, true);
        u32 digit2 = get_digit(line, false);
        sum += digit1*10 + digit2;

        arena_pop(arena, line_len+1);
    }

    printf("Sum: %d\n", sum);
}

void solve_part_two(char* contents, Arena* arena)
{
    char* at = contents;

    char* digits[] =
    {
        "1", "2", "3", "4", "5", "6", "7", "8", "9",
        "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"
    };
    u32 values[] =
    {
        1, 2, 3, 4, 5, 6, 7, 8, 9,
        1, 2, 3, 4, 5, 6, 7, 8, 9
    };

    u32 sum = 0;
    while (*at)
    {
        char* line = get_line(at, arena);
        usize line_len = str_len(line);

        at += line_len;
        if (*at == '\n')
            ++at;
        else if (*at == '\r')
            at += 2;

        i64 digit1_index = INT64_MAX;
        i64 digit2_index = -1;
        u32 digit1 = 0;
        u32 digit2 = 0;
        for (usize i = 0; i < 18; ++i)
        {
            char* found = str_str(line, digits[i]);
            if (found)
            {
                i64 found_index = found - line;
                if (digit1_index > found_index)
                {
                    digit1_index = found_index;
                    digit1 = values[i];
                }
            }

            char* found_rev = str_str_rev(line, digits[i]);
            if (found_rev)
            {
                i64 found_index = found_rev - line;
                if (digit2_index < found_index)
                {
                    digit2_index = found_index;
                    digit2 = values[i];
                }
            }
        }

        sum += digit1*10 + digit2;

        arena_pop(arena, line_len+1);
    }

    printf("Sum: %d\n", sum);
}

int main()
{
    /* NOTE(lucas):
     * PART 1:
     * Each line in input.txt contains at least one digit and a bunch of random characters.
     * The goal of this challenge is to combine the first and last digit on each line into one number,
     * then sum the resulting numbers from each line.
     * Some lines only contain one digit. In this case, both digits are the same since it is both the first and last.
     * For example, the number for the line "treb7uchet" would be 77.
     * Because of this, one viable strategy is to search for a digit twice, once from the beginning of the string,
     * and once from the end, taking the first digit we encounter each time.
     * 
     * PART 2:
     * The general problem is the same, but now spelled-out digits also count (e.g., "one" and '1' are both valid).
     * This makes the problem considerably more difficult.
     * The easiest solution to imagine is to look up all digits in number and spelled-out form,
     * keeping track of the lowest and highest index to the start of the string.
     * Because the same number can appear more than once in the same line,
     * take a similar approach to part 1 and search for each string starting from the beginning and end of the line.
     * Then, continue as normal, converting the strings to a two-digit number and keeping a running total.
     */ 

    Arena arena = arena_alloc(KILOBYTES(64));
    char* contents = read_file_into_memory("input.txt", &arena);
    solve_part_one(contents, &arena);
    solve_part_two(contents, &arena);
    getchar();
}
