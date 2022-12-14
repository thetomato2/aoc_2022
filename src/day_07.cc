#include "types.hh"
#include "utils.hh"

#define FNV_OFFSET     14695981039346656037UL
#define FNV_PRIME      1099511628211UL
#define MAX_CHILD_DIRS 50

struct Directory
{
    const char *name;
    Directory *parent;
    u64 child_cnt;
    Directory *children[MAX_CHILD_DIRS];
    u64 size;
};

function u64
hash_dir(const char *key)
{
    u64 hash = FNV_OFFSET;
    for (const char *p = key; *p; p++) {
        hash ^= (u64)(char)(*p);
        hash *= FNV_PRIME;
    }
    return hash;
}

function Directory *
get_dir(Directory *dir, const char *name)
{
    u64 hash = hash_dir(name);
    u64 i    = (u64)(hash & (u64)(MAX_CHILD_DIRS - 1));

    // Loop till we find an empty entry.
    while (dir->children[i]) {
        if (strcmp(name, dir->children[i]->name) == 0) {
            // Found key, return value.
            return dir->children[i];
        }
        // Key wasn't in this slot, move to next (linear probing).
        ++i;
        if (i >= MAX_CHILD_DIRS) {
            // At end of entries array, wrap around.
            i = 0;
        }
    }

    return 0;
}

// Internal function to set an entry (without expanding table).
function Directory *
make_dir(Directory *dir, const char *name)
{
    // AND hash with capacity-1 to ensure it's within entries array.
    u64 hash = hash_dir(name);
    u64 i    = (u64)(hash & (u64)(MAX_CHILD_DIRS - 1));

    Directory *result = 0;

    // Loop till we find an empty entry.
    while (dir->children[i]) {
        if (strcmp(name, dir->children[i]->name) == 0) {
            return dir->children[i];
        }
        i++;
        if (i >= MAX_CHILD_DIRS) {
            // At end of entries array, wrap around.
            i = 0;
        }
    }

    // Didn't find key, allocate+copy if needed, then insert it.
    dir->children[i] = (Directory *)calloc(1, sizeof(Directory));
    ++dir->child_cnt;
    result         = dir->children[i];
    result->name   = _strdup(name);
    result->parent = dir;

    return result;
}

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

function void
print_dir_path(Directory *dir)
{
    Directory *parents[MAX_CHILD_DIRS] = {};
    s32 parent_i                       = 0;
    Directory *parent                  = dir->parent;
    while (parent) {
        parents[parent_i++] = parent;
        parent              = parent->parent;
    }
    for (s32 i = parent_i - 1; i >= 0; --i) {
        printf("%s/", parents[i]->name);
    }
    printf("%s:", dir->name);
}

function u64
walk_part1(Directory *dir, u64 *total)
{
    u64 dir_total = dir->size;
    for (s32 i = 0; i < MAX_CHILD_DIRS; ++i) {
        if (dir->children[i]) {
            dir_total += walk_part1(dir->children[i], total);
        }
    }
    if (dir_total <= 100000) {
        *total += dir_total;
    }
    // print_dir_path(dir);
    // printf(": %llu\n", dir_total);

    return dir_total;
}

function u64
walk_part2(Directory *dir, u64 *total, u64 space_needed)
{
    u64 dir_total = dir->size;
    for (s32 i = 0; i < MAX_CHILD_DIRS; ++i) {
        if (dir->children[i]) {
            dir_total += walk_part2(dir->children[i], total, space_needed);
        }
    }
    if (dir_total >= space_needed && dir_total < *total) {
        *total = dir_total;
        // print_dir_path(dir);
        // printf(": %llu\n", dir_total);
    }

    return dir_total;
}

s32
main(s32 argc, char *argv[])
{
    set_cwd_to_exe_path(argv[0]);

    FILE *file = fopen("./inputs/day_07.txt", "r");
    // FILE *file = fopen("inputs/day_07_test.txt", "r");

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

    input[file_size] = 0;

    auto root  = (Directory *)calloc(1, sizeof(Directory));
    root->name = "/";

    // make the tree
    // each directory has a hashmap of child directorys
    Directory *cur_dir = root;
    char *line         = input;
    line += get_line_len(line) + 1;  // skip "cd /"
    s32 debug_line_i = 2;
    while (line < input + file_size) {
        if (line[2] == 'c') {
            if (line[5] == '.') {
                cur_dir = cur_dir->parent;
                line += get_line_len(line) + 1;
                ++debug_line_i;
            } else {
                s32 line_len      = get_line_len(line);
                char new_dir[256] = {};
                memcpy(new_dir, &line[5], line_len - 5);
                Directory *dir = get_dir(cur_dir, new_dir);
                if (!dir) {
                    dir = make_dir(cur_dir, new_dir);
                }
                cur_dir = dir;
                line += line_len + 1;
                ++debug_line_i;
            }
        } else if (line[2] == 'l') {
            line += get_line_len(line) + 1;
            ++debug_line_i;
            while (line < input + file_size - 1) {
                if (line[0] == '$') {
                    break;
                } else if (line[0] >= '0' && line[0] <= '9') {
                    u64 size = 0;
                    s32 i    = 0;
                    while (line[i] != ' ') {
                        size *= 10;
                        size += line[i++] - '0';
                    }
                    cur_dir->size += size;
                    line += get_line_len(line) + 1;
                    ++debug_line_i;
                } else if (line[0] == 'd') {
                    s32 line_len      = get_line_len(line);
                    char new_dir[256] = {};
                    memcpy(new_dir, &line[4], line_len - 4);
                    make_dir(cur_dir, new_dir);
                    line += line_len + 1;
                    ++debug_line_i;
                }
            }
        }
    }

    u64 part1      = 0;
    u64 total_used = walk_part1(root, &part1);
    printf("part1: %llu\n", part1);

    u64 space_needed = 30000000 - (70000000 - total_used);
    u64 part2        = U64_MAX;
    walk_part2(root, &part2, space_needed);
    printf("part2: %llu\n", part2);

    return 0;
}
