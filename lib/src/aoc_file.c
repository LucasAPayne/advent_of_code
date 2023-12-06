#include "aoc_file.h"

#include <stdio.h>

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