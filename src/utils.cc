#include <filesystem>
#include <iostream>
#include <string>

#ifndef NOMINMAX
    #define NOMINMAX
#endif
// #define WIN32_LEAN_AND_MEAN

#include <windows.h>

#include "types.hh"

global int64_t performance_counter_frequency;

void
set_cwd_to_exe_path(const char *exe_path_cstr)
{
    std::string exe_path = exe_path_cstr;
    for (auto &c : exe_path) {
        if (c == '\\') {
            c = '/';
        }
    }

    s32 last_slash = 0;
    for (s32 i = exe_path.length() - 1; i > 0; --i) {
        if (exe_path[i] == '/') {
            last_slash = i;
            break;
        }
    }

    std::string exe_dir = exe_path.substr(0, last_slash);
    std::filesystem::current_path(exe_dir);
}

void
init_time()
{
    LARGE_INTEGER performance_query_result;
    QueryPerformanceFrequency(&performance_query_result);
    performance_counter_frequency = performance_query_result.QuadPart;
}

int64_t
get_time()
{
    LARGE_INTEGER time;
    QueryPerformanceCounter(&time);
    return time.QuadPart;
}

double
get_seconds_elapsed(int64_t start, int64_t end)
{
    return (double)(end - start) / (double)performance_counter_frequency;
}

double
get_ms_elapsed(int64_t start, int64_t end)
{
    return ((double)(end - start) / (double)performance_counter_frequency) * 1000.0;
}

int
get_line_len(char *line)
{
    s32 result = 0;
    while (line) {
        if (*line == '\n') {
            break;
        }
        ++result;
        ++line;
    }

    return result;
}
