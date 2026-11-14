#pragma once

#include <cstdint>
#include <random>
#include <unordered_map>

class Random
{
public:
    Random();

    Random(std::unordered_map<std::string, int> p_seeds);

    int getRandInt(std::string p_entry, int p_min = INT32_MIN, int p_max = INT32_MAX);
private:
    std::unordered_map<std::string, std::pair<int, std::mt19937>> m_randoms;
};