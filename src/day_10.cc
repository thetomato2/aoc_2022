#include "types.hh"
#include "utils.hh"

#define PART1 0
#define PART2 1

struct CPU
{
    u64 cur_cycle;
    u64 cycle_interval;
    u64 reg;
};

#if PART1
function CPU
do_cycle_part1(CPU cpu, u64 *part1)
{
    ++cpu.cur_cycle;
    ++cpu.cycle_interval;

    if (cpu.cycle_interval == 40) {
        cpu.cycle_interval = 0;
        *part1 += cpu.cur_cycle * cpu.reg;
    }

    return cpu;
}
#endif

#if PART2
struct Screen
{
    char *pixels;
    s32 width, height;
};

function CPU
do_cycle_part2(CPU cpu, Screen screen)
{
    s32 cpu_x = cpu.cur_cycle % screen.width;

    if (cpu_x == cpu.reg || cpu_x == cpu.reg + 1 || cpu_x == cpu.reg - 1) {
        screen.pixels[cpu.cur_cycle] = '#';
    }else {
        screen.pixels[cpu.cur_cycle] = '.';

    }

    ++cpu.cur_cycle;
    return cpu;
}
#endif

s32
main(s32 argc, char *argv[])
{
    set_cwd_to_exe_path(argv[0]);
    FILE *file = fopen("inputs/day_10.txt", "r");
    // FILE *file = fopen("inputs/day_10_test.txt", "r");
    if (!file) {
        printf("No file.\n");
        return 1;
    }

#if PART1
    {
        u64 part1 = 0;
        CPU cpu   = { 0, 20, 1 };

        char op[8];
        s32 val;
        while (fscanf(file, "%s", op) == 1) {
            if (op[0] == 'a') {
                fscanf(file, "%d\n", &val);
                cpu = do_cycle_part1(cpu, &part1);
                cpu = do_cycle_part1(cpu, &part1);
                cpu.reg += val;

            } else {
                cpu = do_cycle_part1(cpu, &part1);
                val = 0;
            }
        }
        printf("part1: %lld\n", part1);
    }
#endif

#if PART2
    #if PART1
    rewind(file);
    #endif

    {
        CPU cpu = { 0, 20, 1 };

        Screen screen;
        screen.width  = 40;
        screen.height = 6;
        screen.pixels = (char *)malloc(screen.width * screen.height);
        memset(screen.pixels, ' ' , screen.width * screen.height);

        char op[8];
        s32 val;
        while (fscanf(file, "%s", op) == 1) {
            if (op[0] == 'a') {
                fscanf(file, "%d\n", &val);
                cpu = do_cycle_part2(cpu, screen);
                cpu = do_cycle_part2(cpu, screen);
                cpu.reg += val;

            } else {
                cpu = do_cycle_part2(cpu, screen);
                val = 0;
            }
        }

        for (s32 y = 0; y < screen.height; ++y) {
            for (s32 x = 0; x < screen.width; ++x) {
                printf("%c", screen.pixels[y * screen.width + x]);
            }
            printf("\n");
        }
    }

#endif
    fclose(file);

    return 0;
}
