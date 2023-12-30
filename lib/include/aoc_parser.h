#pragma once

#include "aoc_memory.h"
#include "aoc_str.h"
#include "aoc_types.h"

typedef struct Tokenizer
{
    char* at;
} Tokenizer;

typedef struct S8FindResult
{
    s8 str;
    i64 index; // NOTE(lucas): index into the larger string containing this one
} S8FindResult;

void flush_line(Tokenizer* tokenizer)
{
    while (*tokenizer->at != '\n')
        ++tokenizer->at;
    ++tokenizer->at;
}

// NOTE(lucas): Look through a string until a number is found.
// Return a copy of the number string.
s8 find_num(Tokenizer* tokenizer, Arena* arena)
{
    ASSERT(tokenizer->at);
    s8 result = {0};

    if (!tokenizer->at)
        return result;

    while (*tokenizer->at && !char_is_digit(*tokenizer->at))
        ++tokenizer->at;
    
    if (!*tokenizer->at)
        return result;

    char* start = tokenizer->at;
    while(*tokenizer->at && char_is_digit(*tokenizer->at))
        ++tokenizer->at;

    if (!*tokenizer->at)
        return result;

    result.len = tokenizer->at - start;
    result.str = push_array(arena, result.len+1, char);
    for (i64 i = 0; i < result.len; ++i)
        result.str[i] = start[i];

    result.str[result.len] = '\0';

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

u32 get_num(Tokenizer* tokenizer, Arena* arena)
{
    s8 num_str = find_num(tokenizer, arena);
    u32 result = parse_num(num_str.str);
    return result;
}

// NOTE(lucas): Look through a string until a number is found.
// Return a copy of the number string.
S8FindResult find_num_index(char* str, Arena* arena)
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
