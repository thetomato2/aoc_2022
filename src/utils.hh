#include <stdint.h>

template<typename T>
constexpr const T &
min(const T &a, const T &b)
{
    return a < b ? a : b;
}

template<typename T>
constexpr const T &
max(const T &a, const T &b)
{
    return a > b ? a : b;
}

void set_cwd_to_exe_path(const char *exe_path_cstr);

int get_line_len(char *line);

void init_time();
int64_t get_time();
double get_seconds_elapsed(int64_t start, int64_t end);
double get_ms_elapsed(int64_t start, int64_t end);
