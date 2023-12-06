#pragma once

#include "aoc_memory.h"
#include "aoc_types.h"

// NOTE(lucas): Read an entire file into memory and add a null terminator at the end
char* read_file_into_memory(char* file_name, Arena* arena);
