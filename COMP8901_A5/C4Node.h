#pragma once

#include <string>
#include <vector>
#include <set>

struct C4Node {
    unsigned long long playerPos;
    unsigned long long enemyPos;

    bool max;

    std::vector<int> pWins;
    std::vector<int> eWins;

    int depthPopulate = 0;
    int depthScore = 0;

    std::vector<std::string> children;
};

