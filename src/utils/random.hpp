#pragma once

#include <cstdint>
#include <random>

class Random
{
public:
    Random();

    Random(int seed);

    int getRandInt(int min = INT32_MIN, int max = INT32_MAX);
private:
    int seed;
    std::mt19937 gen;
};