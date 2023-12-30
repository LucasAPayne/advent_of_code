#pragma once

#include "aoc_memory.h"
#include "aoc_types.h"

typedef struct s8
{
    char* str;
    i64 len;
} s8;

// IMPORTANT(lucas): Strings are assumed to be null-terminated in these functions
// TODO(lucas): Optimize string methods
// TODO(lucas): Check for null strings as well as asserting
i64 str_len(char* s);
char* str_str(char* str, char* sub);
char* str_str_rev(char* str, char* sub);

b32 str_eq(char* a, char* b);

char* str_copy(char* src, Arena* arena);

b32 char_is_digit(char c);
b32 char_is_alpha(char c);
