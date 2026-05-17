#include <iostream>
#include <conio.h>
#include <windows.h>
using namespace std;
#define H 20
#define W 15
char board[H][W] = {} ;
char blocks[][4][4] = {
        {{' ','O',' ',' '},
         {' ','O',' ',' '},
         {' ','O',' ',' '},
         {' ','O',' ',' '}},
        {{' ',' ',' ',' '},
         {' ','O','O',' '},
         {' ','O','O',' '},
         {' ',' ',' ',' '}},
        {{' ',' ',' ',' '},
         {' ','O',' ',' '},
         {'O','O','O',' '},
         {' ',' ',' ',' '}},
        {{' ',' ',' ',' '},
         {' ','O','O',' '},
         {'O','O',' ',' '},
         {' ',' ',' ',' '}},
        {{' ',' ',' ',' '},
         {'O','O',' ',' '},
         {' ','O','O',' '},
         {' ',' ',' ',' '}},
        {{' ',' ',' ',' '},
         {'O',' ',' ',' '},
         {'O','O','O',' '},
         {' ',' ',' ',' '}},
        {{' ',' ',' ',' '},
         {' ',' ','O',' '},
         {'O','O','O',' '},
         {' ',' ',' ',' '}}
};
int highScore = 0;
int currentScore = 0;
int x=4,y=0,b=1;
char currentBlock[4][4];
void gotoxy(int x, int y) {
    COORD c = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}
void loadBlock() {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            currentBlock[i][j] = blocks[b][i][j];
}
void boardDelBlock(){
    for (int i = 0 ; i < 4 ; i++)
        for (int j = 0 ; j < 4 ; j++)
            if (currentBlock[i][j] != ' ' && y+i < H)
                board[y+i][x+j] = ' ';
}
void block2Board(){
    for (int i = 0 ; i < 4 ; i++)
        for (int j = 0 ; j < 4 ; j++)
            if (currentBlock[i][j] != ' ' )
                board[y+i][x+j] = currentBlock[i][j];
}
void initBoard(){
    for (int i = 0 ; i < H ; i++)
        for (int j = 0 ; j < W ; j++)
            if ((i==H-1) || (j==0) || (j == W-1)) board[i][j] = '#';
            else board[i][j] = ' ';
}
void draw() {
    gotoxy(0,0);

    for (int i = 0; i < H; i++) {

        for (int j = 0; j < W; j++) {
            cout << board[i][j];
        }

        if (i == 2)
            cout << "   Current score: " << currentScore;

        if (i == 4)
            cout << "   Highest score: " << highScore;

        cout << endl;
    }
}
bool canMove(int dx, int dy){
    for (int i = 0 ; i < 4 ; i++)
        for (int j = 0 ; j < 4 ; j++)
            if (currentBlock[i][j] != ' '){
                int tx = x + j + dx;
                int ty = y + i + dy;
                if ( tx<1 || tx >= W-1 || ty >= H-1) return false;
                if ( board[ty][tx] != ' ') return false;
            }
    return true;
}

bool canRotate() {    // check if the block can be rotated without collision or going out of bounds
    char temp[4][4];

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            temp[i][j] = currentBlock[3-j][i];

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {

            if (temp[i][j] != ' ') {

                int tx = x + j;
                int ty = y + i;

                if (tx < 1 || tx >= W-1 || ty >= H-1)
                    return false;

                if (board[ty][tx] != ' ')
                    return false;
            }
        }
    }
    return true;
}

void rotateBlock() {  //actual rotation of the block, should only be called if canRotate() returns true
    char temp[4][4];

    // rotate here
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            temp[i][j] = currentBlock[3-j][i];

    // copy back here
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            currentBlock[i][j] = temp[i][j];
}

void removeLine(int& currentScore){
    int linesCleared = 0;
    for (int i = H - 2; i > 0; i--) {
        bool full = true;
        for (int j = 1; j < W - 1; j++) {
            if (board[i][j] == ' ') {
                full = false;
                break;
            }
        }
        if (full) {
            linesCleared++;
            for (int k = i; k > 0; k--) {
                for (int j = 1; j < W - 1; j++) {
                    board[k][j] = board[k - 1][j];
                }
            }
            for (int j = 1; j < W - 1; j++) {
                board[0][j] = ' ';
            }

            i++;
        }
    }
    if (linesCleared > 0) {
        int score = 100;
        for (int i = 2; i <= linesCleared; i++) {
            score += 100 * ( 1 + 0.5*i);
        }
        currentScore += score;
    }
}

int main()
{
    bool gamePaused=false;
    int speed=200;
    int normalspeed=200;
    bool isDashing=false;
    DWORD dashStart=0;
    int dashDuration=500;

    srand(time(0));
    b=rand()%7;
    loadBlock();

    system("cls");
    initBoard();

    while(1){
        boardDelBlock();

        if(kbhit()){
            char c=getch();

            if(c=='a' && canMove(-1,0)) x--;
            if(c=='d' && canMove(1,0)) x++;

            if(c=='x'){
                isDashing=true;
                dashStart=GetTickCount();
            }

            if(c=='r' && canRotate()) rotateBlock();
            if(c=='q') break;
        }

        if(isDashing){
            speed=40;
            if(GetTickCount()-dashStart>=dashDuration){
                isDashing=false;
            }
        }
        else speed=normalspeed;

        if(!gamePaused){
            if(canMove(0,1)){
                y++;
            }
            else{
                block2Board();
                removeLine(currentScore);

                int level=currentScore/500;
                normalspeed=200-level*20;
                if(normalspeed<40) normalspeed=40;

                if(currentScore>highScore)
                    highScore=currentScore;

                x=5;
                y=0;
                b=rand()%7;
                loadBlock();
            }
        }

        block2Board();
        draw();
        Sleep(speed);
    }

    return 0;
}
