#include "aoc_file.h"
#include "aoc_parser.h"

#include <stdio.h>

#define WINNING_NUM_COUNT 10
#define CARD_NUM_COUNT 25
#define NUM_CARDS 203

internal void get_winning_numbers(Tokenizer* tokenizer, u32* winning_numbers, Arena* arena)
{
    while (*tokenizer->at != ':')
        ++tokenizer->at;

    for (u32 i = 0; i < WINNING_NUM_COUNT; ++i)
        winning_numbers[i] = get_num(tokenizer, arena);
}

internal void get_card_numbers(Tokenizer* tokenizer, u32* card_numbers, Arena* arena)
{
    while (*tokenizer->at != '|')
        ++tokenizer->at;
    
    for (u32 i = 0; i < CARD_NUM_COUNT; ++i)
        card_numbers[i] = get_num(tokenizer, arena);
}

internal void solve_part_one(char* input, Arena* arena)
{
    Tokenizer tokenizer = {0};
    tokenizer.at = input;

    // NOTE(lucas): There are always 10 winning numbers and 25 card numbers
    u32 winning_numbers[WINNING_NUM_COUNT];
    u32 card_numbers[CARD_NUM_COUNT];

    u32 sum = 0;
    while (*tokenizer.at)
    {
        char* line = get_line(input, arena);
        usize line_len = str_len(line);

        get_winning_numbers(&tokenizer, winning_numbers, arena);
        get_card_numbers(&tokenizer, card_numbers, arena);
        flush_line(&tokenizer);

        u32 matches = 0;
        for (u32 winning_num_index = 0; winning_num_index < WINNING_NUM_COUNT; ++winning_num_index)
        {
            for (u32 card_num_index = 0; card_num_index < CARD_NUM_COUNT; ++card_num_index)
            {
                u32 winning_num = winning_numbers[winning_num_index];
                u32 card_num = card_numbers[card_num_index];
                if (card_num == winning_num)
                    ++matches;
            }
        }
        if (matches > 0)
            sum += pow_u32(2, matches-1);

        arena_pop(arena, line_len+1);
    }

    printf("Part 1: %d\n", sum);
}

internal void solve_part_two(char* input, Arena* arena)
{
    Tokenizer tokenizer = {0};
    tokenizer.at = input;

    u32 winning_numbers[WINNING_NUM_COUNT] = {0};
    u32 card_numbers[CARD_NUM_COUNT] = {0};
    u32 copies[NUM_CARDS] = {0};

    u32 card_index = 0;
    while (*tokenizer.at)
    {
        char* line = get_line(input, arena);
        usize line_len = str_len(line);

        get_winning_numbers(&tokenizer, winning_numbers, arena);
        get_card_numbers(&tokenizer, card_numbers, arena);
        flush_line(&tokenizer);

        ++copies[card_index];

        u32 matches = 0;
        for (u32 winning_num_index = 0; winning_num_index < WINNING_NUM_COUNT; ++winning_num_index)
        {
            for (u32 card_num_index = 0; card_num_index < CARD_NUM_COUNT; ++card_num_index)
            {
                u32 winning_num = winning_numbers[winning_num_index];
                u32 card_num = card_numbers[card_num_index];
                if (card_num == winning_num)
                    ++matches;
            }
        }

        for (u32 i = 1; i <= matches; ++i)
            copies[card_index + i] += copies[card_index];

        ++card_index;
        arena_pop(arena, line_len+1);
    }

    u32 sum = 0;
    for (u32 i = 0; i < NUM_CARDS; ++i)
        sum += copies[i];

    printf("Part 2: %d\n", sum);
}

int main(void)
{
    /* NOTE(lucas):
     * PART 1:
     * Each line in input.txt contains the winning numbers and card numbers for a scratchcard.
     * For each card, the first card number that matches a winning number earns a point.
     * Then, for each match after that in the same card, the points are doubled.
     * Find the total number of points across all cards.
     * For this part, simply collect the winning numbers and card numbers for each line.
     * Then, compare them all and determine the number of matches. If there were any matches,
     * The number of points for that card is just 2^matches-1.
     * 
     * Part 2:
     * This part gets more complicated. Instead of getting points for matches, you get copies
     * of cards below for each match. For example, if Card 1 has 3 mathces, you get a copy of
     * Cards 2, 3, and 4. Then, each of those can also get matches and propagate downwards.
     * This will never overflow past the final card. For this part, a simple solution is to
     * keep a list of the number of copies of each card. Each card starts with one copy,
     * so add a copy. Then, find the number of matches for that card and add a copy of each of
     * the next <matches> cards.
     */

    // NOTE(lucas): Part 1 answer: 25651
    // NOTE(lucas): Part 2 answer: 19499881

    Arena arena = arena_alloc(KILOBYTES(64));
    char* contents = read_file_into_memory("input.txt", &arena);
    solve_part_one(contents, &arena);
    solve_part_two(contents, &arena);
    getchar();
}
