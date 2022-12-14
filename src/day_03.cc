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

    FILE *file = fopen("day_03.txt", "rb");
    fseek(file, 0, SEEK_END);
    s32 file_size = ftell(file);
    rewind(file);

    char *input = (char *)malloc(file_size + 1);
    fread(input, file_size, 1, file);
    fclose(file);

    input[file_size] = 0;

    s32 map[256] = {};
    s32 part1    = 0;

    // part 1
    for (char *it = input; it < input + file_size;) {
        s32 line_len      = get_line_len(it);
        s32 half_line_len = line_len / 2;
        char *it2         = it;
        while (it2 < it + half_line_len) {
            ++map[*it2];
            ++it2;
        }
        it2 = it + half_line_len;
        while (it2 < it + line_len) {
            if (*it2 != '\n' && map[*it2] > 0) {
                if (*it2 < 'a') {
                    part1 += *it2 - 38;
                    break;
                } else {
                    part1 += *it2 - 96;
                    break;
                }
            }
            ++it2;
        }
        memset(&map[0], 0, sizeof(s32) * 256);

        it = it + line_len + 1;
    }

    printf("Part1: %d\n", part1);

    // part 2
    // NOTE: if you are using languages with hash-maps/sets that would be ideal
    // but its just simpler in C to use arrays indexed off ascii values
    s32 map2[256] = {};
    s32 part2     = 0;
    for (char *it = input; it < input + file_size;) {
        s32 line_len = 0;
        // line/map 1
        {
            line_len  = get_line_len(it);
            char *it2 = it;
            while (it2 < it + line_len) {
                ++map[*it2++];
            }

            it = it + line_len + 1;
        }
        // line/map 2
        {
            line_len  = get_line_len(it);
            char *it2 = it;
            while (it2 < it + line_len) {
                ++map2[*it2++];
            }

            it = it + line_len + 1;
        }

        // cross reference against map 1 and 2
        {
            bool found_type = false;
            char *it2       = it;
            line_len        = get_line_len(it);
            while (it2 < it + line_len) {
                if (map[*it2] && map2[*it2]) {
                    if (*it2 < 'a') {
                        found_type = true;
                        part2 += *it2 - 38;
                        break;
                    } else {
                        found_type = true;
                        part2 += *it2 - 96;
                        break;
                    }
                }
                ++it2;
            }
        }
        memset(&map[0], 0, sizeof(s32) * 256);
        memset(&map2[0], 0, sizeof(s32) * 256);
        it = it + line_len + 1;
    }

    printf("Part2: %d\n", part2);

    return 0;
}
