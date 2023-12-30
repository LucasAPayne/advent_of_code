#include "aoc_file.h"
#include "aoc_parser.h"

#include <stdio.h>

// NOTE(lucas): Get the next word in a string and return a null-terminated copy,
// or return 0 if no word found
s8 get_word(Tokenizer* tokenizer, Arena* arena)
{
    ASSERT(tokenizer->at);
    s8 result = {0};
    if (!tokenizer->at)
        return result;

    while (*tokenizer->at && !char_is_alpha(*tokenizer->at))
        ++tokenizer->at;
    
    if (!*tokenizer->at)
        return result;

    char* start = tokenizer->at;
    while(*tokenizer->at && char_is_alpha(*tokenizer->at))
        ++tokenizer->at;

    if (!*tokenizer->at)
        return result;

    i64 len = tokenizer->at - start;
    result.str = push_array(arena, len+1, char);
    for (i64 i = 0; i < len; ++i)
        result.str[i] = start[i];

    result.str[len] = '\0';
    result.len = len;

    return result;
}

void solve_part_one(char* input, Arena* arena)
{
    Tokenizer tokenizer = {0};
    tokenizer.at = input;

    u32 max_red = 12;
    u32 max_green = 13;
    u32 max_blue = 14;
    u32 sum = 0;

    while (*tokenizer.at)
    {
        char* line = get_line(tokenizer.at, arena);
        usize line_len = str_len(line);

        u32 game_id = get_num(&tokenizer, arena);

        b32 valid = true;
        while (*tokenizer.at != '\n')
        {
            u32 num = get_num(&tokenizer, arena);
            if (tokenizer.at)
            {
                s8 word = get_word(&tokenizer, arena);
                if ((str_eq(word.str, "red")   && num > max_red) ||
                    (str_eq(word.str, "green") && num > max_green) ||
                    (str_eq(word.str, "blue")  && num > max_blue))
                {
                    valid = false;
                }
                arena_pop(arena, (usize)(word.len+1));
            }
        }
        ++tokenizer.at;

        if (valid)
            sum += game_id;
        
        arena_pop(arena, line_len+1);
    }

    printf("Sum: %d\n", sum);
}

void solve_part_two(char* input, Arena* arena)
{
    Tokenizer tokenizer = {0};
    tokenizer.at = input;

    u32 sum = 0;

    while (*tokenizer.at)
    {
        char* line = get_line(tokenizer.at, arena);
        usize line_len = str_len(line);

        u32 game_id = get_num(&tokenizer, arena);

        u32 max_red = 0;
        u32 max_green = 0;
        u32 max_blue = 0;

        while (*tokenizer.at != '\n')
        {
            u32 num = get_num(&tokenizer, arena);
            if (tokenizer.at)
            {
                s8 word = get_word(&tokenizer, arena);
                if (str_eq(word.str, "red") && num > max_red)
                    max_red = num;
                if (str_eq(word.str, "green") && num > max_green)
                    max_green = num;
                if (str_eq(word.str, "blue") && num > max_blue)
                    max_blue = num;

                arena_pop(arena, (usize)(word.len+1));
            }
        }
        ++tokenizer.at;

        sum += max_red*max_green*max_blue;

        arena_pop(arena, line_len+1);
    }

    printf("Sum: %d\n", sum);
}

int main(void)
{
    /* NOTE(lucas):
     * PART 1:
     * Each line in input.txt contains information about a game played with red, green, and blue cubes.
     * There are 12 red cubes, 13 green cubes, and 14 blue cubes. Some cubes are revealed in each turn of the game
     * and then are put back. A game is possible if for each color in each turn, at most the maximum number of cubes
     * for that color was shown. So, for example, 12 red cubes can be shown at once, but not 13 or more.
     * Each game has an ID number, and each game has semicolon-separated lists of how many cubes were revealed.
     * The lists vary in size, and the order of colors changes. The goal is to find the sum of the IDs of the games
     * that were posible. 
     * Get the game ID, then find a number, look at the following word that specifies color, and determine whether
     * it is at most the maximum number for that color. 
     * 
     * Part 2:
     * Now, instead of determining whether each game was valid, determine the minimum number of each color cube
     * for each game. Then, multiply these minimums together to find the power for the game. Find the sum of
     * powers for each game. This problem is very similar. The parsing stays the same, and finding the minimum
     * required cubes can still be framed as a maximum problem since it is the same as finding the max number of
     * each color.
     */

    // NOTE(lucas): Part 1 answer: 2348
    // NOTE(lucas): Part 2 answer: 76008

    Arena arena = arena_alloc(KILOBYTES(64));
    char* contents = read_file_into_memory("input.txt", &arena);
    solve_part_one(contents, &arena);
    solve_part_two(contents, &arena);
    getchar();
}