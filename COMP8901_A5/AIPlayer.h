#pragma once

#include <map>
#include <string>
#include "ConnectFour.h"
#include "Vec2D.h"
#include "Token.h"
#include "C4Node.h"
#include <bitset>
#include <algorithm>

class AIPlayer {
public:
    AIPlayer(const Token (*board)[7], const Token &colour);

    Vec2D FindBestPlacement();
private:
    unsigned long long getPos(int x, int y);
    Vec2D idToVec(unsigned long long pos);
    int getScore(const C4Node &n, const int curM);
    void expandNode(C4Node &n, const int depth);

    void printNode(const C4Node &node, const int depth);

    const Token (*_board)[7];
    const Token _colour;
    unsigned long long _winStates[69];
    std::map<std::string, C4Node> _nodes;
    const static int MAX_DEPTH = 8;
};

