#include "types.hh"
#include "utils.hh"

#define FNV_OFFSET   14695981039346656037UL
#define FNV_PRIME    1099511628211UL
#define MAX_MOVE_CNT 4096
#define KNOT_CNT     10

// Vector 2 to store the knot positions
struct v2s
{
    s32 x, y;
};

// Vector 2 to easily use vector math
struct v2f
{
    f32 x, y;
};

// stored move positions for the hashset
struct Move
{
    b32 occupied;
    v2s move;
};

// You can use a grid to store the tail positions, but that gets very space inefficient the larger
// the grid gets which is totally fine for the provided problem set, but it would be trivial to make
// the hashset dynimacally grow as needed which is much more efficient and robust
struct HashSet
{
    Move moves[MAX_MOVE_CNT];
    u64 cnt;
};

// The FNV hash family is a nice for things that don't need good security
function u64
hash_v2i(const v2s &key)
{
    u64 hash = FNV_OFFSET;
    for (char *p = (char *)&key; p < (char *)&key + sizeof(key); p++) {
        hash ^= (u64)(char)(*p);
        hash *= FNV_PRIME;
    }
    return hash;
}

function void
check_move_set(HashSet *hs, v2s tail)
{
    u64 hash = hash_v2i(tail);
    u64 i    = (u64)(hash & (u64)(MAX_MOVE_CNT - 1));
    while (hs->moves[i].occupied) {
        if (hs->moves[i].move.x == tail.x && hs->moves[i].move.y == tail.y) {
            return;
        }
        ++i;
        if (i >= MAX_MOVE_CNT) i = 0;
    }

    hs->moves[i].occupied = 1;
    hs->moves[i].move     = tail;
    ++hs->cnt;
}

function v2s
move_knot(const v2s &head, const v2s &tail)
{
    v2f h = { (f32)head.x, (f32)head.y };
    v2f t = { (f32)tail.x, (f32)tail.y };

    v2f dis     = { h.x - t.x, h.y - t.y };
    v2f abs_dis = { fabsf(dis.x), fabsf(dis.y) };

    // vector length
    f32 len = sqrtf(abs_dis.x * abs_dis.x + abs_dis.y * abs_dis.y);

    v2s result = tail;
    // distance of diagonal, plug in a distance of 1,1 into the length formula
    if (len > 2.24f) {
        // diagonal move, only happens in part 2
        if (dis.x > 0 && dis.y > 0) {
            // up right
            result.x = head.x - 1;
            result.y = head.y - 1;
        } else if (dis.x > 0 && dis.y < 0) {
            // down right
            result.x = head.x - 1;
            result.y = head.y + 1;
        } else if (dis.x < 0 && dis.y < 0) {
            // down left
            result.x = head.x + 1;
            result.y = head.y + 1;
        } else if (dis.x < 0 && dis.y > 0) {
            // up left
            result.x = head.x + 1;
            result.y = head.y - 1;
        }
    } else if (len > 1.42f) {
        if (abs_dis.x > abs_dis.y) {
            if (dis.x > 0) {
                result.x = head.x - 1;
                result.y = head.y;
            } else {
                result.x = head.x + 1;
                result.y = head.y;
            }

        } else {
            if (dis.y > 0) {
                result.x = head.x;
                result.y = head.y - 1;

            } else {
                result.x = head.x;
                result.y = head.y + 1;
            }
        }
    }

    return result;
}

function void
print_rope(v2s *rope, HashSet *hs = 0)
{
    for (s32 y = 25; y >= -25; --y) {
        for (s32 x = -25; x < 25; ++x) {
            if (rope[0].x == x && rope[0].y == y) {
                printf("0");
            } else if (rope[1].x == x && rope[1].y == y) {
                printf("1");
            } else if (rope[2].x == x && rope[2].y == y) {
                printf("2");
            } else if (rope[3].x == x && rope[3].y == y) {
                printf("3");
            } else if (rope[4].x == x && rope[4].y == y) {
                printf("4");
            } else if (rope[5].x == x && rope[5].y == y) {
                printf("5");
            } else if (rope[6].x == x && rope[6].y == y) {
                printf("6");
            } else if (rope[7].x == x && rope[7].y == y) {
                printf("7");
            } else if (rope[8].x == x && rope[8].y == y) {
                printf("8");
            } else if (rope[9].x == x && rope[9].y == y) {
                printf("9");
            } else if (hs) {
                bool found = false;
                for (s32 i = 0; i < MAX_MOVE_CNT; ++i) {
                    if (hs->moves[i].occupied && hs->moves[i].move.x == x &&
                        hs->moves[i].move.y == y) {
                        found = true;
                        break;
                    }
                }
                if (found) {
                    printf("#");

                } else {
                    printf(".");
                }
            } else {
                printf(".");
            }
        }
        printf("\n");
    }
}

#define DEBUG_PRINT 0

s32
main(s32 argc, char *argv[])
{
    set_cwd_to_exe_path(argv[0]);

    char dir;
    s32 cnt;

    FILE *file = fopen("inputs/day_09.txt", "r");
    // FILE *file = fopen("inputs/day_09_test.txt", "r");
    if (!file) {
        printf("No file.\n");
        return 1;
    }

    v2s rope[KNOT_CNT] = {};
    HashSet hs         = {};

    // add the start position to the hashset
    check_move_set(&hs, {});

    u64 move_cnt = 0;
    while (fscanf(file, "%c %d\n", &dir, &cnt) == 2) {
        // one at a time to check the tails position after each step.
        switch (dir) {
            case 'U': {
                for (s32 i = 0; i < cnt; ++i) {
                    ++rope[0].y;

                    for (s32 i = 0; i < KNOT_CNT - 1; ++i) {
                        rope[i + 1] = move_knot(rope[i], rope[i + 1]);
                    }
#if DEBUG_PRINT
                    print_rope(rope, &hs);
                    printf("\n");
#endif
                    check_move_set(&hs, rope[KNOT_CNT - 1]);
                }
            } break;
            case 'R': {
                for (s32 i = 0; i < cnt; ++i) {
                    ++rope[0].x;
                    for (s32 i = 0; i < KNOT_CNT - 1; ++i) {
                        rope[i + 1] = move_knot(rope[i], rope[i + 1]);
                    }
#if DEBUG_PRINT
                    print_rope(rope, &hs);
                    printf("\n");
#endif
                    check_move_set(&hs, rope[KNOT_CNT - 1]);
                }
            } break;
            case 'D': {
                for (s32 i = 0; i < cnt; ++i) {
                    --rope[0].y;
                    for (s32 i = 0; i < KNOT_CNT - 1; ++i) {
                        rope[i + 1] = move_knot(rope[i], rope[i + 1]);
                    }
#if DEBUG_PRINT
                    print_rope(rope, &hs);
                    printf("\n");
#endif
                    check_move_set(&hs, rope[KNOT_CNT - 1]);
                }
            } break;
            case 'L': {
                for (s32 i = 0; i < cnt; ++i) {
                    --rope[0].x;
                    for (s32 i = 0; i < KNOT_CNT - 1; ++i) {
                        rope[i + 1] = move_knot(rope[i], rope[i + 1]);
                    }
#if DEBUG_PRINT
                    print_rope(rope, &hs);
                    printf("\n");
#endif
                    check_move_set(&hs, rope[KNOT_CNT - 1]);
                }
            } break;
        }
#if DEBUG_PRINT
        printf("%c %d\n", dir, cnt);
#endif
    }
    fclose(file);

    printf("Part2: %lld\n", hs.cnt);
    s32 part2_grid = 0;

    return 0;
}
