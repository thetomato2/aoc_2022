#include "types.hh"
#include "utils.hh"

function s32
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

s32
main(s32 argc, char *argv[])
{
    set_cwd_to_exe_path(argv[0]);

    s32 a[4], part1 = 0, part2 = 0;
    FILE *file = fopen("day_04.txt", "r");
    while (fscanf(file, "%d-%d,%d-%d", &a[0], &a[1], &a[2], &a[3]) == 4) {
        part1 += (a[0] >= a[2] && a[1] <= a[3]) || (a[0] <= a[2] && a[1] >= a[3]);
        part2 += a[0] <= a[3] && a[1] >= a[2];
    }
    fclose(file);

    // part
    printf("Part1: %d\nPart2: %d\n", part1, part2);

    return 0;
}
