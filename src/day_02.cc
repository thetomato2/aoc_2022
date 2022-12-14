#include "types.hh"
#include "utils.hh"

s32
main(s32 argc, char *argv[])
{
    set_cwd_to_exe_path(argv[0]);

    FILE *file = fopen("day_02_A.txt", "rb");
    fseek(file, 0, SEEK_END);
    s32 file_size = ftell(file);
    rewind(file);

    char *input = (char *)malloc(file_size + 1);
    fread(input, file_size, 1, file);
    fclose(file);

    input[file_size] = 0;

    char *read_ptr = input;
    while (*read_ptr != '\n' && read_ptr < input + file_size) ++read_ptr;
    s32 line_len = read_ptr - input + 1;  // with \n
    read_ptr     = input;

    s64 part1 = 0;
    s64 part2 = 0;
    s32 o, m;
    while (read_ptr < input + file_size) {
        o = read_ptr[0] - 'A';
        m = read_ptr[2] - 'X';
        // simple modulus math, but in C/C++ you need to mod, plus mode,  then mod again to get the
        // modulus to wrap to positive instead of negative. 
        part1 += ((((m - o + 1) % 3) + 3) % 3) * 3 + m + 1;
        part2 += (o + (m + 2)) % 3 + 3 * m + 1;

        read_ptr += line_len;
    }

    printf("Part1: %lld\n", part1);
    printf("Part2: %lld\n", part2);

    return 0;
}
