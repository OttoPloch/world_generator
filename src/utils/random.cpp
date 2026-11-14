#include "random.hpp"
#include <cassert>
#include <iostream>

Random::Random() {}

Random::Random(std::unordered_map<std::string, int> p_seeds)
{
    for (auto i_entry : p_seeds)
    {
        m_randoms[i_entry.first] = {i_entry.second, std::mt19937(i_entry.second)};
    }
}

int Random::getRandInt(std::string p_entry, int p_min, int p_max)
{
    if (m_randoms.find(p_entry) == m_randoms.end())
    {        
        std::cerr << "ERROR: cannot get random int for entry '" << p_entry << "'. That entry does not exist.\n";
        
        for (auto i : m_randoms)
        {
            std::cout << "))) " << i.first << ", " << i.second.first << '\n';
        }

        return 0;
    }

    std::uniform_int_distribution<int> dist(p_min, p_max);
    return dist(m_randoms[p_entry].second);
}