#include "aoc_file.h"
#include "aoc_types.h"

#include <stdio.h>

// TODO(lucas): This currently tells the wrong size for files with CRLF line endings
char* read_file_into_memory(char* file_name, Arena* arena)
{
    char* result = 0;

    FILE* file = fopen(file_name, "r");
    if (file)
    {
        fseek(file, 0, SEEK_END);
        usize file_size = ftell(file);
        fseek(file, 0, SEEK_SET);

        result = push_array(arena, file_size+1, char);
        fread(result, file_size, 1, file);
        result[file_size] = 0; // Null-terminate file contents

        fclose(file);
    }

    return result;
}

// NOTE(lucas): Read until EOL or EOF, return null-terminated string
char* get_line(char* at, Arena* arena)
{
    char* start = at;
    while (*at && *at != '\n' && *at != '\r')
        ++at;
    
    i64 len = at - start;

    char* result = push_array(arena, len+1, char);
    for (i64 i = 0; i < len; ++i)
        result[i] = start[i];
    
    result[len] = '\0';
    return result;
}
