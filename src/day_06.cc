#include "types.hh"
#include "utils.hh"

function bool
check_unique(s32 *arr, s32 sz)
{
    bool result = true;
    for (s32 i = 0; i < sz; ++i) {
        if (arr[i] != 0 && arr[i] != 1) {
            result = false;
            break;
        }
    }
    return result;
}

s32
main(s32 argc, char *argv[])
{
    set_cwd_to_exe_path(argv[0]);

    FILE *file = fopen("day_06.txt", "r");
    // FILE *file = fopen("day_06_test.txt", "r");

    if (!file) {
        printf("No file!\n");
        INVALID_CODE_PATH;
    }

    fseek(file, 0, SEEK_END);
    s32 file_size = ftell(file);
    rewind(file);

    char *input = (char *)malloc(file_size + 1);
    fread(input, file_size, 1, file);
    fclose(file);

    char *ptr         = input;
    bool found_unique = false;
    while (!found_unique && ptr < input + file_size) {
        found_unique = ptr[0] != ptr[1] && ptr[0] != ptr[2] && ptr[0] != ptr[3] &&
                       ptr[1] != ptr[2] && ptr[1] != ptr[3] && ptr[2] != ptr[3];
        ++ptr;
    }

    s32 part1 = ptr - input + 3;
    printf("part1: %d\n", part1);

    s32 c_cnt[26] = {};
    ptr           = input;
    for (s32 i = 0; i < 14; ++i) {
        ++c_cnt[ptr[i] - 'a'];
    }

    s32 file_end = (input + file_size) - input;
    for (s32 i = 14; i < file_end; ++i) {
        if (check_unique(c_cnt, 26)) {
            s32 part2 = i;
            printf("part2: %d\n", part2);
            break;
        }
        ++c_cnt[ptr[i] - 'a'];
        --c_cnt[ptr[i - 14] - 'a'];
    }

    return 0;
}
