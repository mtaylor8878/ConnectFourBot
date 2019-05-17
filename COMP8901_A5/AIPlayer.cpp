#include "AIPlayer.h"
#include <vector>

using namespace std;

AIPlayer::AIPlayer(const Token (*board)[7], const Token &colour) :
    _board(board), _colour(colour) {

    int winCount = 0;
    for (int y = 0; y < ConnectFour::BOARD_HEIGHT; y++) {
        for (int x = 0; x < ConnectFour::BOARD_WIDTH - 3; x++) {
            _winStates[winCount++] = getPos(x, y) | getPos(x + 1, y) | getPos(x + 2, y) | getPos(x + 3, y);
        }
    }
    for (int x = 0; x < ConnectFour::BOARD_WIDTH; x++) {
        for (int y = 0; y < ConnectFour::BOARD_HEIGHT - 3; y++) {
            _winStates[winCount++] = getPos(x, y) | getPos(x, y + 1) | getPos(x, y + 2) | getPos(x, y + 3);
        }
    }
    for (int y = 0; y < ConnectFour::BOARD_HEIGHT - 3; y++) {
        for (int x = 0; x < ConnectFour::BOARD_WIDTH - 3; x++) {
            _winStates[winCount++] = getPos(x, y) | getPos(x + 1, y + 1) | getPos(x + 2, y + 2) | getPos(x + 3, y + 3);
        }
    }
    for (int y = 0; y < ConnectFour::BOARD_HEIGHT - 3; y++) {
        for (int x = ConnectFour::BOARD_WIDTH; x > 3; x--) {
            _winStates[winCount++] = getPos(x, y) | getPos(x - 1, y + 1) | getPos(x - 2, y + 2) | getPos(x - 3, y + 3);
        }
    }
}

Vec2D AIPlayer::FindBestPlacement() {
    unsigned long long pState = 0,
        eState = 0;
    for (int y = ConnectFour::BOARD_HEIGHT - 1; y > -1 ; --y) {
        for (int x = ConnectFour::BOARD_WIDTH - 1; x  > -1; --x) {
            pState <<= 1;
            eState <<= 1;
            if (_board[y][x] == _colour) {
                pState |= 1;
            } else if (_board[y][x] != NONE) {
                eState |= 1;
            }
        }
    }

    string id = bitset<42>(pState).to_string() + bitset<42>(eState).to_string();
    if (_nodes.find(id) == _nodes.end()) {
        C4Node curNode;
        curNode.playerPos = pState;
        curNode.enemyPos = eState;
        curNode.max = true;
        for (int i = 0; i < 69; i++) {
            if ((_winStates[i] & eState) == 0)
                curNode.pWins.push_back(i);
            if ((_winStates[i] & pState) == 0)
                curNode.eWins.push_back(i);
        }
        _nodes[id] = curNode;
    }
    
    expandNode(_nodes[id], 1);

//  printNode(_nodes[id], 0);

    pair<int, unsigned long long> max = {INT_MIN, 0};
    for (auto child : _nodes[id].children) {
        int score = getScore(_nodes[child], max.first);
        Vec2D coord = idToVec(_nodes[id].playerPos ^ _nodes[child].playerPos);
        cout << "(" << coord.y << "," << coord.x << ") = " << score << endl;
        if (score == INT_MAX) {
            max = { score, _nodes[id].playerPos ^ _nodes[child].playerPos };
            break;
        }
        if (score > max.first)
            max = {score, _nodes[id].playerPos ^ _nodes[child].playerPos};
    }

    return idToVec(max.second);
}

Vec2D AIPlayer::idToVec(unsigned long long pos) {
    for (int y = 0; y < ConnectFour::BOARD_HEIGHT; y++) {
        for (int x = 0; x < ConnectFour::BOARD_WIDTH; x++) {
            if (pos % 2 == 1)
                return Vec2D(y, x);
            pos >>= 1;
        }
    }

    return Vec2D();
}

void AIPlayer::expandNode(C4Node &n, const int depth) {
    if (n.depthPopulate != depth) {
        vector<unsigned long long> available;
        for (int x = 0; x < ConnectFour::BOARD_WIDTH; x++) {
            for (int y = 0; y < ConnectFour::BOARD_HEIGHT; y++) {
                unsigned long long cur = getPos(x, y);
                if ((n.playerPos & cur) == 0 && (n.enemyPos & cur) == 0) {
                    available.push_back(cur);
                    break;
                }
            }
        }

        for (unsigned long long pos : available) {
            unsigned long long newP, newE;
            if (n.max) {
                newP = n.playerPos | pos;
                newE = n.enemyPos;
            } else {
                newP = n.playerPos;
                newE = n.enemyPos | pos;
            }

            bitset<84> pBit(newP);
            pBit <<= 42;
            pBit |= newE;
            string id = pBit.to_string();
            if (find(n.children.begin(), n.children.end(), id) == n.children.end())
                n.children.push_back(id);

            if (_nodes.find(id) == _nodes.end()) {
                C4Node curNode;
                curNode.playerPos = newP;
                curNode.enemyPos = newE;
                curNode.max = !n.max;
                curNode.eWins = n.eWins;
                curNode.pWins = n.pWins;

                if (n.max) {
                    for (int index : n.eWins)
                        if ((_winStates[index] & newP) > 0)
                            curNode.eWins.erase(find(curNode.eWins.begin(), curNode.eWins.end(), index));
                } else {
                    for (int index : n.pWins)
                        if ((_winStates[index] & newE) > 0)
                            curNode.pWins.erase(find(curNode.pWins.begin(), curNode.pWins.end(), index));
                }

                _nodes[id] = curNode;

            }

            if (depth < MAX_DEPTH)
                expandNode(_nodes[id], depth + 1);

        }
        n.depthPopulate = depth;
    }
}

int AIPlayer::getScore(const C4Node &n, const int curM) {
    if (n.children.size() == 0) {
        for (int stateId : n.eWins) {
            if ((n.enemyPos & _winStates[stateId]) == _winStates[stateId])
                return INT_MIN;
        }

        for (int stateId : n.pWins) {
            if ((n.playerPos & _winStates[stateId]) == _winStates[stateId])
                return INT_MAX;
        }

        return n.pWins.size() - n.eWins.size();
    } else {
        if (n.max) {
            int max = INT_MIN;
            for (string id : n.children) {
                int score = getScore(_nodes[id], max);
                if (score == INT_MAX)
                    return score;
                if (score > curM)
                    return score;
                if (score > max)
                    max = score;
            }
            return max;
        } else {
            int min = INT_MAX;
            for (string id : n.children) {
                int score = getScore(_nodes[id], min);
                if (score == INT_MIN)
                    return score;
                if (score < curM)
                    return score;
                if (score < min)
                    min = score;
            }
            return min;
        }
    }
}

unsigned long long AIPlayer::getPos(int x, int y) {
    return unsigned long long(1) << (x + y * ConnectFour::BOARD_WIDTH);
}

void AIPlayer::printNode(const C4Node &node, const int depth) {
    for (int i = 0; i < depth; i++)
        cout << "    ";

    cout << "P:";
    unsigned long long pos = node.enemyPos;
    while (pos > 0) {
        Vec2D coord = idToVec(pos);
        cout << "(" << coord.y << "," << coord.x << ")";
        pos &= ~getPos(coord.y, coord.x);
    }

    cout << ",AI:";
    pos = node.playerPos;
    while (pos > 0) {
        Vec2D coord = idToVec(pos);
        cout << "(" << coord.y << "," << coord.x << ")";
        pos &= ~getPos(coord.y, coord.x);
    }

    cout << endl;

    for (auto id : node.children) {
        printNode(_nodes[id], depth + 1);
    }
}