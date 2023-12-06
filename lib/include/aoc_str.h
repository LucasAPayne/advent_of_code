#pragma once

#include "aoc_types.h"

// IMPORTANT(lucas): Strings are assumed to be null-terminated in these functions
// TODO(lucas): Optimize string methods
// TODO(lucas): Check for null strings as well as asserting
i64 str_len(char* s);
char* str_str(char* str, char* sub);
char* str_str_rev(char* str, char* sub);

b32 char_is_digit(char c);
