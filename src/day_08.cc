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
    init_time();

    s64 program_start_time = get_time();

    FILE *file = fopen("./inputs/day_08.txt", "r");
    // FILE *file = fopen("inputs/day_08_test.txt", "r");

    if (!file) {
        printf("No file!\n");
        return 1;
    }

    fseek(file, 0, SEEK_END);
    s32 file_size = ftell(file);
    rewind(file);

    char *input = (char *)malloc(file_size + 1);
    fread(input, file_size, 1, file);
    fclose(file);

    char *line = input;
    s32 cols   = get_line_len(line);
    s32 rows   = file_size / (cols + 1);  // +1 for the \n
    s32 *mat   = (s32 *)malloc(cols * rows * sizeof(s32));
    s32 *mat2  = (s32 *)calloc(cols * rows, sizeof(s32));
    s32 y      = 0;

    while (line < input + file_size) {
        if (y == rows) break;
        for (s32 i = 0; i < cols; ++i) {
            mat[y * cols + i] = line[i] - '0';
        }
        ++y;

        line += get_line_len(line) + 1;
    }

    s64 part1_start_time = get_time();

    for (s32 y = 1; y < cols - 1; ++y) {
        s32 m = -1;
        for (s32 x = 0; x < rows; ++x) {
            s32 tree = mat[y * cols + x];
            if (tree > m) {
                mat2[y * cols + x] = 1;
                m                  = tree;
            }
            if (tree == 9) break;
        }
    }

    for (s32 y = 1; y < cols - 1; ++y) {
        s32 m = -1;
        for (s32 x = rows - 1; x >= 0; --x) {
            s32 tree = mat[y * cols + x];
            if (tree > m) {
                mat2[y * cols + x] = 1;
                m                  = tree;
            }
            if (tree == 9) break;
        }
    }

    for (s32 x = 1; x < cols - 1; ++x) {
        s32 m = -1;
        for (s32 y = 0; y < rows; ++y) {
            s32 tree = mat[y * cols + x];
            if (tree > m) {
                mat2[y * cols + x] = 1;
                m                  = tree;
            }
            if (tree == 9) break;
        }
    }

    for (s32 x = 1; x < cols - 1; ++x) {
        s32 m = -1;
        for (s32 y = rows - 1; y >= 0; --y) {
            s32 tree = mat[y * cols + x];
            if (tree > m) {
                mat2[y * cols + x] = 1;
                m                  = tree;
            }
            if (tree == 9) break;
        }
    }

    // set the corners
    mat2[0]                            = 1;
    mat2[rows - 1]                     = 1;
    mat2[(cols - 1) * cols]            = 1;
    mat2[(cols - 1) * cols + rows - 1] = 1;

    // // debug print
    // for (s32 y = 0; y < cols; ++y) {
    //     for (s32 x = 0; x < rows; ++x) {
    //         s32 tree = mat2[y * cols + x];
    //         printf("%d", tree);
    //     }
    //     printf("\n");
    // }

    s32 part1 = 0;
    for (s32 i = 0; i < cols * rows; ++i) {
        if (mat2[i]) ++part1;
    }
    s64 part1_end_time = get_time();

    // part2
    // I can't think of way that isn't brute force
    // as in just raycasting from each tree.
    s32 part2            = 0;
    s64 part2_start_time = get_time();
    for (s32 y = 0; y < cols; ++y) {
        for (s32 x = 0; x < rows; ++x) {
            s32 tree = mat[y * cols + x];

            s32 scenic_val0 = 0;
            for (s32 tx = x + 1; tx < rows; ++tx) {
                if (mat[y * cols + tx] < tree) {
                    ++scenic_val0;
                } else {
                    ++scenic_val0;
                    break;
                }
            }

            s32 scenic_val1 = 0;
            for (s32 tx = x - 1; tx >= 0; --tx) {
                if (mat[y * cols + tx] < tree) {
                    ++scenic_val1;
                } else {
                    ++scenic_val1;
                    break;
                }
            }

            s32 scenic_val2 = 0;
            for (s32 ty = y - 1; ty >= 0; --ty) {
                if (mat[ty * cols + x] < tree) {
                    ++scenic_val2;
                } else {
                    ++scenic_val2;
                    break;
                }
            }

            s32 scenic_val3 = 0;
            for (s32 ty = y + 1; ty < cols; ++ty) {
                if (mat[ty * cols + x] < tree) {
                    ++scenic_val3;
                } else {
                    ++scenic_val3;
                    break;
                }
            }
            s32 scenic_score = scenic_val0 * scenic_val1 * scenic_val2 * scenic_val3;
            if (scenic_score > part2) part2 = scenic_score;
        }
    }
    s64 part2_end_time = get_time();

    s64 program_end_time = get_time();

    f64 total_time = get_ms_elapsed(program_start_time, program_end_time);
    f64 part1_time = get_ms_elapsed(part1_start_time, part1_end_time);
    f64 part2_time = get_ms_elapsed(part2_start_time, part2_end_time);

    printf("part1: %d - %.3fms\npart2: %d - %.3fms\ntotal - %.3fms\n", part1, part1_time, part2,
           part2_time, total_time);

    return 0;
}
