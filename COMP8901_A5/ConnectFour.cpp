#include "ConnectFour.h"
#include <windows.h>

using namespace std;

ConnectFour::ConnectFour() {
    int cols = sizeof(_board[0]) / sizeof(_board[0][0]),
        rows = sizeof(_board) / sizeof(_board[0]);

    _lastPlacement = { -1, -1 };
    
    for (int y = 0; y < rows; ++y)
        for (int x = 0; x < cols; ++x)
            _board[y][x] = NONE;

    _tokenCount = sizeof(_board) / sizeof(Token);
}

ConnectFour::~ConnectFour() {
}

void ConnectFour::StartGame(const Token &playerColour) {
    _playerToken = playerColour;
    Token enemy = playerColour == RED ? BLACK : RED;
    AIPlayer ai(_board, enemy);
    GameState state = _playerToken == RED ? PLAYER_TURN : AI_TURN;
    bool win = false;

    while (!win) {
        switch (state) {
        case PLAYER_TURN:
            PrintBoard();

            while (!attemptPlaceToken(_playerToken)) {
                cout << "Invalid Move" << endl;
            }

            if (hasWon()) {
                win = true;
                break;
            }

            state = AI_TURN;
            break;
        case AI_TURN:
            auto start = chrono::high_resolution_clock::now();

            Vec2D best = ai.FindBestPlacement();

            auto finish = chrono::high_resolution_clock::now();
            cout << "AI took " << chrono::duration_cast<chrono::milliseconds>(finish - start).count() << "ms" << endl;

            placeToken(best.x, best.y, enemy);

            if (hasWon()) {
                win = true;
                break;
            }

            state = PLAYER_TURN;
            break;
        }

        if (_tokenCount == 0)
            break;
    }

    cout << endl << endl;

    if (win) {
        if (state == PLAYER_TURN)
            cout << "YOU WON!" << endl;
        else
            cout << "YOU LOST" << endl;
    } else {
        cout << "DRAW" << endl;
    }

    PrintBoard();
}

void ConnectFour::PrintBoard() {
    cout << endl;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int cols = sizeof(_board[0]) / sizeof(_board[0][0]),
        rows = sizeof(_board) / sizeof(_board[0]);

    SetConsoleTextAttribute(hConsole, BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
    for (int y = rows - 1; y > -1; --y) {
        for (int x = 0; x < cols; ++x) {
            cout << "|";
            switch (_board[y][x]) {
            case RED:
                SetConsoleTextAttribute(hConsole, BACKGROUND_RED | BACKGROUND_INTENSITY);
                cout << "   ";
                SetConsoleTextAttribute(hConsole, BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
                break;
            case BLACK:
                SetConsoleTextAttribute(hConsole, 0);
                cout << "   ";
                SetConsoleTextAttribute(hConsole, BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
                break;
            default:
                cout << "   ";
                break;
            }
        }
        cout << "|" << endl;
        for (int i = 0; i < cols * 4 + 1; i++)
            cout << "-";
        cout << endl;
    }
    for (int x = 0; x < cols; ++x)
        cout << "  " << (x + 1) << " ";
    cout << " " << endl;
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    cout << endl;
}

bool ConnectFour::attemptPlaceToken(const Token &colour) {
    cout << "Choose which lane to place your token (1-7)" << endl;
    int lane;
    cin >> lane;

    if (lane < 1 || lane > 7)
        return false;

    int rows = sizeof(_board) / sizeof(_board[0]);

    int row = 0;
    for (row = 0; row < rows; ++row) {
        if (_board[row][lane - 1] == NONE) {
            placeToken(row, lane - 1, colour);
            break;
        }
    }

    return row < rows;
}

void ConnectFour::placeToken(const int row, const int col, const Token &colour) {
    if (_board[row][col] == NONE) {
        _board[row][col] = colour;
        --_tokenCount;

        _lastPlacement = {row, col};
    }
}

bool ConnectFour::hasWon() {
    if (!(_lastPlacement.x < 0 || _lastPlacement.y < 0)) {
        return (longestChain(_lastPlacement, { 0,1 }) >= 4 ||
                longestChain(_lastPlacement, { 1,0 }) >= 4 ||
                longestChain(_lastPlacement, { 1,1 }) >= 4 ||
                longestChain(_lastPlacement, { 1,-1 }) >= 4);
    }
    return false;
}

int ConnectFour::longestChain(const Vec2D &pos, const Vec2D &dir) {
    if (_board[pos.x][pos.y] == NONE)
        return 0;

    int len = 1;
    Token colour = _board[pos.x][pos.y];
    Vec2D opposite = dir * -1;

    Vec2D next = pos + dir;
    while (inBounds(next) && _board[next.x][next.y] == colour) {
        next += dir;
        len++;
    }

    next = pos + opposite;
    while (inBounds(next) && _board[next.x][next.y] == colour) {
        next += opposite;
        len++;
    }

    return len;
}

bool ConnectFour::inBounds(const Vec2D &pos) {
    return(pos.x > -1 && pos.x < BOARD_HEIGHT &&
        pos.y > -1 && pos.y < BOARD_WIDTH);
}

int main(int argc, const char *argv[]) {
    ConnectFour game;

    bool again = true;
    char c;

    while (again) {
        cout << "Choose your colour (R/B)" << endl;
        Token player = NONE;
        while (player == NONE) {
            cin >> c;

            if (c == 'R' || c == 'r')
                player = RED;
            else if (c == 'B' || c == 'b')
                player = BLACK;
            else
                cout << "Invalid colour. R or B?" << endl;
        }

        game.StartGame(player);

        cout << "Play Again? (Y/N)" << endl;
        cin >> c;

        if (c == 'N' || c == 'n')
            again = false;
        else
            game = ConnectFour();
    }

    return EXIT_SUCCESS;
}
