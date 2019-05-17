#pragma once

#include <iostream>
#include "Vec2D.h"
#include "AIPlayer.h"
#include "Token.h"
#include <chrono>

class ConnectFour {
public:
    enum GameState {
        PLAYER_TURN, AI_TURN, 
    };

    ConnectFour();
    ~ConnectFour();

    void StartGame(const Token &playerColour);
    void PrintBoard();

    const static int BOARD_WIDTH = 7;
    const static int BOARD_HEIGHT = 6;
private:
    bool attemptPlaceToken(const Token &colour);
    void placeToken(const int row, const int col, const Token &colour);
    bool hasWon();
    int longestChain(const Vec2D &pos, const Vec2D &dir);
    bool inBounds(const Vec2D &pos);

    Token   _board[BOARD_HEIGHT][BOARD_WIDTH];
    Token   _playerToken;
    int     _tokenCount;
    Vec2D   _lastPlacement;
};

