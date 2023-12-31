#include "aoc_str.h"

i64 str_len(char* s)
{
    // NOTE(lucas): To get length of a string, get another pointer,
    // then advance until the null terminator and get the difference
    // between the copy and original pointer location
    // ASSERT(s);
    if (!s)
        return 0;
    char* copy = s;
    for (copy; *copy; ++copy)
        ;
    i64 result = copy - s;
    return result;
}

// NOTE(lucas): Check if str has substring sub,
// returning a pointer to the first occurrence of sub
char *str_str(char *str, char *substring)
{
    const char* a = str;
    const char* b = substring;
    for (;;)
    {
        if (!*b)
            return (char *)str;
        if (!*a)
            return 0;
        if (*a++ != *b++)
        {
            a = ++str;
            b = substring;
        }
    }
}

// NOTE(lucas): Check if str has substring sub,
// returning a pointer to the last occurrence of sub
char* str_str_rev(char* str, char* sub)
{
    char* result = 0;
    i64 len = str_len(str);
    i64 sub_len = str_len(sub);

    i64 last_possible = str_len(str) - str_len(sub);
    char* p = str + last_possible;
    while (p >= str)
    {
        result = str_str(p, sub);
        if (result)
            break;
        
        --p;
    }

    return result;
}

b32 str_eq(char* a, char* b)
{
    ASSERT(a);
    ASSERT(b);
    if (!a || !b)
        return false;

    i64 a_len = str_len(a);
    i64 b_len = str_len(b);
    if (a_len != b_len)
        return false;

    b32 result = true;
    for (i64 i = 0; i < a_len; ++i)
    {
        if (a[i] != b[i])
        {
            result = false;
            break;
        }
    }

    return result;
}

char* str_copy(char* src, Arena* arena)
{
    usize len = str_len(src);
    char* result = push_array(arena, len+1, char);
    for (i64 i = 0; i < len; ++i)
        result[i] = src[i];

    result[len] = '\0';
    return result;
}

b32 char_is_digit(char c)
{
    b32 result = ((c >= '0') && (c <= '9'));
    return result;
}

b32 char_is_alpha(char c)
{
    b32 result = ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
    return result;
}
