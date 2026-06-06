#include "random.hpp"
#include <random>

Random::Random() {}

Random::Random(int seed)
{
    this->seed = seed;
    gen = std::mt19937(seed);
}

int Random::getRandInt(int min, int max)
{
    std::uniform_int_distribution<int> dist(min, max);
    return dist(gen);
}