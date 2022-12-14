#include "types.hh"
#include "utils.hh"

s32
main(s32 argc, char *argv[])
{
    set_cwd_to_exe_path(argv[0]);

    FILE *file = fopen("day_05.txt", "r");
    // FILE *file = fopen("day_05_test.txt", "r");
    char line[256];

    constexpr s32 crate_sz           = 64;
    char crates1[crate_sz][crate_sz] = {};
    char crates2[crate_sz][crate_sz] = {};

    if (file) {
        s32 stack_max_height = 0;
        while (fgets(line, sizeof(line), file) && line[0] != '\n') {
            ++stack_max_height;
        }
        rewind(file);
        stack_max_height -= 1;
        s32 crate_stack_i = stack_max_height - 1;
        while (fgets(line, sizeof(line), file) && line[0] != '\n') {
            if (crate_stack_i < 0) break;
            s32 i       = 1;
            s32 crate_i = 0;
            while (i < 36) {
                crates1[crate_i][crate_stack_i] = line[i];
                crates2[crate_i][crate_stack_i] = line[i];
                ++crate_i;
                i += 4;
            }
            --crate_stack_i;
        }
    } else {
        printf("No file!\n");
    }

    for (s32 i = 0; i < crate_sz; ++i) {
        for (s32 j = 0; j < crate_sz; ++j) {
            if (crates1[i][j] < 'A' || crates1[i][j] > 'Z') {
                crates1[i][j] = '\0';
                crates2[i][j] = '\0';
            }
        }
    }
    fgets(line, sizeof(line), file);

    s32 n, i0, i1;
    s32 debug_i = 0;
    while (fscanf(file, "move %d from %d to %d\n", &n, &i0, &i1) == 3) {
        // part1
        {
            s32 a = strlen(crates1[i0 - 1]) - 1;
            s32 b = strlen(crates1[i1 - 1]);
            for (s32 i = 0; i < n; ++i) {
                crates1[i1 - 1][b] = crates1[i0 - 1][a];
                crates1[i0 - 1][a] = '\0';
                --a;
                ++b;
            }
        }
        // part2
        {
            s32 a = strlen(crates2[i0 - 1]);
            s32 b = strlen(crates2[i1 - 1]);
            a -= n;
            for (s32 i = 0; i < n; ++i) {
                crates2[i1 - 1][b++] = crates2[i0 - 1][a];
                crates2[i0 - 1][a++] = '\0';
            }
        }
    }

    fclose(file);

    char part1[crate_sz] = {};
    char part2[crate_sz] = {};
    for (s32 i = 0; i < crate_sz; ++i) {
        // printf("%s\n", crates1[i]);
        part1[i] = crates1[i][strlen(crates1[i]) - 1];
        part2[i] = crates2[i][strlen(crates2[i]) - 1];
    }
    printf("Part1: %s\nPart2: %s\n", part1, part2);

    return 0;
}
