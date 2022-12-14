#include <fstream>
#include <filesystem>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include "types.hh"

s32
main(s32 argc, char *argv[])
{
    std::filesystem::current_path("c:/dev/aoc_2022/build/bin/");
    // std::cout << "cwd: " << std::filesystem::current_path() << std::endl;

    s64 max_calories = 0;
    std::vector<std::string> lines;

    std::ifstream file { "day_01.txt" };

    if (file) {
        while (file) {
            std::string line;
            std::getline(file, line);
            lines.push_back(line);
        }
        s64 sum = 0;
        for (auto l : lines) {
            if (l.empty()) {
                max_calories = std::max(max_calories, sum);
                sum          = 0;
            } else {
                sum += std::stoi(l);
            }
        }
    } else {
        std::cout << "No file.\n";
    }

    std::cout << "Part_01: " << max_calories << std::endl;

    if (!lines.empty()) {
        s64 cur_sum = 0;
        std::vector<s64> sums;
        for (auto l : lines) {
            if (l.empty()) {
                sums.push_back(cur_sum);
                cur_sum = 0;
            } else {
                cur_sum += std::stoi(l);
            }
        }

        s64 m1 = 0;
        s64 m2 = 0;
        s64 m3 = 0;
        for (auto i : sums) {
            if (i > m1) {
                m1 = i;
            } else if (i > m2) {
                m2 = i;
            } else if (i > m3) {
                m3 = i;
            }
        }
        max_calories = m1 + m2 + m3;

    } else {
        std::cout << "No file.\n";
    }

    std::cout << "Part_02: " << max_calories << std::endl;

    return 0;
}
