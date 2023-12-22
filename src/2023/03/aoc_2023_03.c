#include "aoc_file.h"
#include "aoc_str.h"

#include <stdio.h>

typedef struct Tokenizer
{
    char* at;
} Tokenizer;

typedef struct s8
{
    char* str;
    i64 len;
} s8;

typedef struct S8FindResult
{
    s8 str;
    i64 index; // NOTE(lucas): index into the larger string containing this one
} S8FindResult;

// NOTE(lucas): Look through a string until a number is found.
// Return a copy of the number string.
S8FindResult get_num(char* str, Arena* arena)
{
    ASSERT(str);
    S8FindResult result = {0};
    result.index = -1;

    char* at = str;

    if (!at)
        return result;

    while (*at && !char_is_digit(*at))
        ++at;
    
    if (!*at)
        return result;

    char* num_start = at;
    while(*at && char_is_digit(*at))
        ++at;

    result.str.len = at - num_start;
    result.str.str = push_array(arena, result.str.len+1, char);
    for (i64 i = 0; i < result.str.len; ++i)
        result.str.str[i] = num_start[i];

    result.str.str[result.str.len] = '\0';
    result.index = num_start - str;

    return result;
}

i64 get_asterisk_index(char* str)
{
    ASSERT(str);
    i64 result = -1;

    char* at = str;

    if (!at)
        return result;

    while (*at && *at != '*')
        ++at;

    if (!*at)
        return result;

    result = at - str;
    ++at;
    return result;
}

u32 pow_u32(u32 num, u32 pow)
{
    u32 result = 1;

    for (u32 i = 0; i < pow; ++i)
        result *= num;

    return result;
}

u32 parse_num(char* num)
{
    u32 result = 0;
    char* at = num;

    while(*at && char_is_digit(*at))
        ++at;

    u32 digits = (u32)(at - num);
    for (u32 i = 0; i < digits; ++i)
        result += pow_u32(10, digits-1-i) * (u32)(num[i] - '0');

    return result;
}

b32 char_is_symbol(char c)
{
    b32 result = (c == '@' || c == '#' || c == '$' || c == '%' || c == '&' || c == '*' ||
                  c == '-' || c == '=' || c == '+' || c == '/');
    return result;
}

void solve_part_one(char* input, Arena* arena)
{
    Tokenizer tokenizer = {0};
    tokenizer.at = input;

    char* prev_line = 0;
    char* curr_line = get_line(tokenizer.at, arena);

    i64 line_len = str_len(curr_line);
    tokenizer.at += line_len+1;

    char* next_line = get_line(tokenizer.at, arena);

    u32 sum = 0;
    usize num_lines = 0;
    while (*curr_line)
    {
        char* curr_line_start = str_copy(curr_line, arena);
        char* next_line_start = str_copy(next_line, arena);

        while(*curr_line)
        {
            // TODO(lucas): Can this work with the tokenizer and not be inconvenient?
            S8FindResult num_str = get_num(curr_line, arena);
            if (num_str.index == -1)
                *curr_line = 0;
            else
            {
                b32 add = false;
                u32 num = parse_num(num_str.str.str);

                // NOTE(lucas): If a number starts or ends a line, do not check the invalid side.
                i64 min_index = 0;
                if (num_str.index > 0)
                    min_index = num_str.index - 1;
                i64 max_index = num_str.index + num_str.str.len;
                i64 iterations = max_index+1 - min_index;

                // NOTE(lucas): Make sure indices are in bounds, and if they are, check either side of the number
                // on the current line.
                if ((num_str.index > 0) && char_is_symbol(*(curr_line + num_str.index-1)))
                    add = true;
                if (*(curr_line + num_str.index + num_str.str.len) && char_is_symbol(*(curr_line + num_str.index + num_str.str.len)))
                    add = true;
                curr_line += num_str.index + num_str.str.len;

                if (prev_line && *prev_line)
                {
                    prev_line += min_index;
                    for(i64 i = 0; *prev_line && i < iterations; ++i, ++prev_line)
                    {
                        if (char_is_symbol(*prev_line))
                            add = true;
                    }
                    --prev_line;
                }
                if (next_line && *next_line)
                {
                    next_line += min_index;
                    for (i64 i = 0; *next_line && i < iterations; ++i, ++next_line)
                    {
                        if (char_is_symbol(*next_line))
                            add = true;
                    }
                    --next_line;
                }

                if (add)
                    sum += num;
            }
        }

        tokenizer.at += line_len+1;

        prev_line = curr_line_start;
        curr_line = next_line_start;
        next_line = get_line(tokenizer.at, arena);
        ++num_lines;
    }

    // NOTE(lucas): Can't free from the middle, so at least free the memory used for
    // copying each line.
    arena_pop(arena, num_lines*line_len);

    printf("Part 1: %d\n", sum);
}

void solve_part_two(char* input, Arena* arena)
{
    Tokenizer tokenizer = {0};
    tokenizer.at = input;

    char* prev_line = 0;
    char* curr_line = get_line(tokenizer.at, arena);

    i64 line_len = str_len(curr_line);
    tokenizer.at += line_len+1;

    char* next_line = get_line(tokenizer.at, arena);

    u64 sum = 0;
    usize num_lines = 0;
    while (*curr_line)
    {
        ++num_lines;
        char* curr_line_start = str_copy(curr_line, arena);
        char* next_line_start = str_copy(next_line, arena);

        while(*curr_line)
        {
            // TODO(lucas): Can this work with the tokenizer and not be inconvenient?
            i64 index = get_asterisk_index(curr_line);
            u32 neighbors = 0;
            if (index == -1)
                *curr_line = 0;
            else
            {
                // NOTE(lucas): If a number starts or ends a line, do not check the invalid side.
                i64 min_index = 0;
                if (index > 0)
                    min_index = index - 1;
                i64 max_index = index + 1;
                i64 iterations = max_index+1 - min_index;

                S8FindResult num_str[2];
                u32 num[2];

                if ((index > 0) && char_is_digit(*(curr_line + index-1)))
                {
                    if (neighbors < 2)
                    {
                        i64 new_index = index;
                        while(*curr_line && char_is_digit(*(curr_line + new_index-1)))
                            --new_index;
                        num_str[neighbors] = get_num(curr_line + new_index, arena);
                        num[neighbors] = parse_num(num_str[neighbors].str.str);
                    }
                    ++neighbors;
                }
                if (*(curr_line + index + 1) && char_is_digit(*(curr_line + index + 1)))
                {
                    if (neighbors < 2)
                    {
                        num_str[neighbors] = get_num(curr_line + index + 1, arena);
                        num[neighbors] = parse_num(num_str[neighbors].str.str);
                    }
                    ++neighbors;
                }
                curr_line += index + 1;

                char* prev_line_it = prev_line;
                if (prev_line_it && *prev_line_it)
                {
                    prev_line_it += min_index;
                    b32 found = false;
                    for (i64 i = 0; *prev_line_it && i < iterations; ++i, ++prev_line_it)
                    {
                        if (!found && char_is_digit(*prev_line_it))
                        {
                            if (neighbors < 2)
                            {
                                u32 correction = 0;
                                while(*prev_line_it && char_is_digit(*(prev_line_it-1)))
                                {
                                    --prev_line_it;
                                    ++correction;
                                }
                                num_str[neighbors] = get_num(prev_line_it, arena);
                                num[neighbors] = parse_num(num_str[neighbors].str.str);
                                found = true;
                                prev_line_it += correction;
                            }
                            ++neighbors;
                        }
                        else if (found && !char_is_digit(*prev_line_it))
                            found = false;
                    }
                    prev_line += max_index;
                }

                char* next_line_it = next_line;
                if (next_line_it && *next_line_it)
                {
                    next_line_it += min_index;
                    b32 found = false;
                    for (i64 i = 0; *next_line_it && i < iterations; ++i, ++next_line_it)
                    {
                        if (!found && char_is_digit(*next_line_it))
                        {
                            if (neighbors < 2)
                            {
                                u32 correction = 0;
                                while(*next_line_it && char_is_digit(*(next_line_it-1)))
                                {
                                    --next_line_it;
                                    ++correction;
                                }
                                num_str[neighbors] = get_num(next_line_it, arena);
                                num[neighbors] = parse_num(num_str[neighbors].str.str);
                                found = true;
                                next_line_it += correction;
                            }
                            ++neighbors;
                        }
                        else if (found && !char_is_digit(*next_line_it))
                            found = false;
                    }
                    next_line += max_index;
                }

                if (neighbors == 2)
                    sum += num[0]*num[1];
            }
        }


        tokenizer.at += line_len+1;

        prev_line = curr_line_start;
        curr_line = next_line_start;
        next_line = get_line(tokenizer.at, arena);
    }

    arena_pop(arena, num_lines*line_len);

    printf("Part 2: %lld\n", sum);
}

int main(void)
{
    /* NOTE(lucas):
     * PART 1:
     * Each line in input.txt contains numbers and symbols, and the empty space is filled with periods, which do not
     * count as symbols. A number is a part number if it is adjacent (even diagonally) to a symbol. We want to find the
     * sum of all part numbers.
     * There are three buffers: prev_line, curr_line, and next_line. On the first iteration, prev_line
     * is null, and next_line should be null on the last iteration. On the first iteration, there should be two calls to
     * get_line. For the current line, find and parse all the numbers on that line. For each number, also find the first
     * and last index from the beginning of the line, and extend them by one in either direction. Then, and loop over
     * those indices for the previous and next line and determine if there is a symbol in one of those spots. Also check
     * the character directly to either side of the number. If there is a symbol adjacent to the number, add the number
     * to the sum. At the end of each loop, previous line becomes current line, current line becomes next line, and next
     * line is gotten with get_line.  
     * 
     * PART 2:
     * Now, we want to find gears, which are asterisks adjacent to two part numbers. Note that any two numbers will do,
     * since being adjacent to the asterisk makes them part numbers. The gear ratio is the product of the two part
     * numbers adjacent to the gear, and we want to find the sum of all gear ratios.
     * Follow a similar scheme as before, but instead check if an asterisk has two neighboring numbers.
     */

    // NOTE(lucas): Part 1 answer: 520135
    // NOTE(lucas): Part 2 answer: 72514855

    // TODO(lucas): These methods became increasingly complex as more edge cases were introduced, especiall in part 2.
    // This day could use some compression. See if anything common can be pulled out, or if anything
    // can be simplified.
    Arena arena = arena_alloc(KILOBYTES(128));
    char* contents = read_file_into_memory("input.txt", &arena);
    solve_part_one(contents, &arena);
    solve_part_two(contents, &arena);
    getchar();
}
