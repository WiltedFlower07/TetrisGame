#include <iostream>
#include <conio.h>
#include <windows.h>
#include <ctime>

using namespace std;

#define H 20
#define W 15


char BLOCKS[7][4][4] = {
    {
        {' ','O',' ',' '},
        {' ','O',' ',' '},
        {' ','O',' ',' '},
        {' ','O',' ',' '}
    },
    {
        {' ',' ',' ',' '},
        {' ','O','O',' '},
        {' ','O','O',' '},
        {' ',' ',' ',' '}
    },
    {
        {' ',' ',' ',' '},
        {' ','O',' ',' '},
        {'O','O','O',' '},
        {' ',' ',' ',' '}
    },
    {
        {' ',' ',' ',' '},
        {' ','O','O',' '},
        {'O','O',' ',' '},
        {' ',' ',' ',' '}
    },
    {
        {' ',' ',' ',' '},
        {'O','O',' ',' '},
        {' ','O','O',' '},
        {' ',' ',' ',' '}
    },
    {
        {' ',' ',' ',' '},
        {'O',' ',' ',' '},
        {'O','O','O',' '},
        {' ',' ',' ',' '}
    },
    {
        {' ',' ',' ',' '},
        {' ',' ','O',' '},
        {'O','O','O',' '},
        {' ',' ',' ',' '}
    }
};


void gotoxy(int x, int y) {
    COORD c = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}


class Block {
private:
    int x, y;
    int type;
    char shape[4][4];

public:
    Block() {
        reset();
    }

    void reset() {
        x = 5;
        y = 0;
        type = rand() % 7;

        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                shape[i][j] = BLOCKS[type][i][j];
    }

    void rotate() {
        char temp[4][4];

        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                temp[i][j] = shape[3 - j][i];

        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                shape[i][j] = temp[i][j];
    }

    // Getters
    int getX() const { return x; }
    int getY() const { return y; }

    void setX(int value) { x = value; }
    void setY(int value) { y = value; }

    char getCell(int i, int j) const {
        return shape[i][j];
    }

    void setShape(char temp[4][4]) {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                shape[i][j] = temp[i][j];
    }
};


class Board {
private:
    char grid[H][W];

public:
    Board() {
        init();
    }

    void init() {
        for (int i = 0; i < H; i++) {
            for (int j = 0; j < W; j++) {

                if (i == H - 1 || j == 0 || j == W - 1)
                    grid[i][j] = '#';
                else
                    grid[i][j] = ' ';
            }
        }
    }

    void draw(const Block& block, int score, int highScore) {
        gotoxy(0, 0);

        char temp[H][W];

        // Copy board
        for (int i = 0; i < H; i++)
            for (int j = 0; j < W; j++)
                temp[i][j] = grid[i][j];

        // Draw current block
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {

                if (block.getCell(i, j) != ' ') {

                    int x = block.getX() + j;
                    int y = block.getY() + i;

                    if (y >= 0 && y < H && x >= 0 && x < W)
                        temp[y][x] = block.getCell(i, j);
                }
            }
        }

        // Print board
        for (int i = 0; i < H; i++) {

            for (int j = 0; j < W; j++) {
                cout << temp[i][j];
            }

            if (i == 2)
                cout << "   Score: " << score;

            if (i == 4)
                cout << "   High Score: " << highScore;

            cout << endl;
        }
    }

    bool canMove(const Block& block, int dx, int dy) {

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {

                if (block.getCell(i, j) != ' ') {

                    int tx = block.getX() + j + dx;
                    int ty = block.getY() + i + dy;

                    if (tx < 1 || tx >= W - 1 || ty >= H - 1)
                        return false;

                    if (grid[ty][tx] != ' ')
                        return false;
                }
            }
        }

        return true;
    }

    bool canRotate(const Block& block) {

        char temp[4][4];

        // Rotate into temp
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                temp[i][j] = block.getCell(3 - j, i);

        // Check collision
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {

                if (temp[i][j] != ' ') {

                    int tx = block.getX() + j;
                    int ty = block.getY() + i;

                    if (tx < 1 || tx >= W - 1 || ty >= H - 1)
                        return false;

                    if (grid[ty][tx] != ' ')
                        return false;
                }
            }
        }

        return true;
    }

    void placeBlock(const Block& block) {

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {

                if (block.getCell(i, j) != ' ') {

                    int x = block.getX() + j;
                    int y = block.getY() + i;

                    grid[y][x] = block.getCell(i, j);
                }
            }
        }
    }

    void clearLines(int& score) {

        int linesCleared = 0;

        for (int i = H - 2; i > 0; i--) {

            bool full = true;

            for (int j = 1; j < W - 1; j++) {

                if (grid[i][j] == ' ') {
                    full = false;
                    break;
                }
            }

            if (full) {

                linesCleared++;

                for (int k = i; k > 0; k--) {

                    for (int j = 1; j < W - 1; j++) {
                        grid[k][j] = grid[k - 1][j];
                    }
                }

                for (int j = 1; j < W - 1; j++) {
                    grid[0][j] = ' ';
                }

                i++;
            }
        }

        if (linesCleared > 0) {

            int gained = 100;

            for (int i = 2; i <= linesCleared; i++) {
                gained += 100 * (1 + 0.5 * i);
            }

            score += gained;
        }
    }

    bool gameOver(const Block& block) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (block.getCell(i, j) != ' ') {
                    int x = block.getX() + j;
                    int y = block.getY() + i;
                    if (grid[y][x] != ' ')
                        return true;
                }
            }
        }
        return false;
    }
};


class Game {
private:
    Board board;
    Block block;
    bool running;
    bool paused;
    int score;
    int highScore;
    int speed;
    int normalSpeed;
    bool dashing;
    DWORD dashStart;
    int dashDuration;

public:
    Game() {
        running = true;
        paused = false;
        score = 0;
        highScore = 0;
        speed = 200;
        normalSpeed = 200;
        dashing = false;
        dashDuration = 500;
    }

    void input() {
        if (kbhit()) {
            char c = getch();

            if (c == ' ')
                paused = !paused;

            if (c == 'q')
                running = false;

            if (paused)
                return;

            if (c == 'a' && board.canMove(block, -1, 0))
                block.setX(block.getX() - 1);

            if (c == 'd' && board.canMove(block, 1, 0))
                block.setX(block.getX() + 1);

            if (c == 's' && board.canMove(block, 0, 1))
                block.setY(block.getY() + 1);

            if (c == 'r' && board.canRotate(block))
                block.rotate();

            if (c == 'x') {
                dashing = true;
                dashStart = GetTickCount();
            }
        }

        if (dashing) {
            speed = 40;

            if (GetTickCount() - dashStart >= dashDuration) {
                dashing = false;
            }
        }
        else {
            speed = normalSpeed;
        }
    }

    void update() {

        if (paused)
            return;

        if (board.canMove(block, 0, 1)) {
            block.setY(block.getY() + 1);
        }
        else {
            board.placeBlock(block);
            board.clearLines(score);

            if (score > highScore)
                highScore = score;

            int level = score / 500;
            normalSpeed = 200 - level * 20;

            if (normalSpeed < 40)
                normalSpeed = 40;

            block.reset();

            if (board.gameOver(block)) {
                gotoxy(0, H + 2);
                cout << "GAME OVER\n";
                running = false;
            }
        }
    }

    void render() {
        board.draw(block, score, highScore);
    }

    void run() {
        system("cls");
        while (running) {
            input();
            update();
            render();
            Sleep(speed);
        }
    }
};

int main() {
    srand((unsigned)time(0));
    Game game;
    game.run();
    return 0;
}