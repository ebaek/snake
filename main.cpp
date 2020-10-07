#include <cstdlib>
#include <ncurses.h>

bool gameOver;
const int width = 20, height = 20;
int x, y, FruitX, FruitY, score;

enum eDirection {
    STOP = 0,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

eDirection dir;

int TailX[100], TailY[100];
int nTail = 0;
void Setup() {

    initscr();
    clear();
    noecho();
    cbreak();
    curs_set(0);

    gameOver = false;

    dir = STOP;

    x = width / 2;
    y = height / 2;

    FruitX = (rand() % width)+1;
    FruitY = (rand() % height)+1;

    score = 0;
}

void Draw() {
    clear();

    // top wall
    for(int i = 0; i < width+2; i++)
        mvprintw(0,i,"+");

    for(int row = 0; row <= height+1; row++) {
        for(int col = 0; col <= width+1; col++) {
            // wall
            if (row == 0 || row == height + 1)
                mvprintw(row, col, "+");
            else if (col == 0 || col == height + 1)
                mvprintw(row, col, "+");
            else if (row == y && col == x)
                mvprintw(row, col, "O");
            else if(row == FruitY && col == FruitX)
                mvprintw(row, col, "Q");
            else {

              for(int k = 0; k < nTail; k++) {
                if (TailX[k] == col && TailY[k] == row) {
                  mvprintw(row, col, "o");
                }
              }

            }      
        }
    }

    mvprintw(height+3, 0, "Score %d", score);
    refresh();
}

void Input() {
  keypad(stdscr, true);

  halfdelay(1);

  switch(getch()) {

    case KEY_LEFT:
      dir = LEFT;
      break;

    case KEY_RIGHT:
      dir = RIGHT;
      break;

    case KEY_UP:
      dir = UP;
      break;

    case KEY_DOWN:
      dir = DOWN;
      break;

    case 'Q':
      gameOver = true;
      break;

  }

}

void Logic() {
  int prevX = TailX[0];
  int prevY = TailY[0];

  int currX, currY;

  TailX[0] = x;
  TailY[0] = y;


  for (int i = 1; i < nTail; i++) {
    currX = TailX[i];
    currY = TailY[i];

    TailX[i] = prevX;
    TailY[i] = prevY;

    prevX = currX;
    prevY = currY;
  }

  switch(dir) {
    case LEFT:
      x--;
      break;

    case RIGHT:
      x++;
      break;

    case UP:
      y--;
      break;

    case DOWN:
      y++;
      break;

    default:
      break;

  }

  if(x > width || x < 1 || y < 1 || y > height) {
    gameOver = true;
  }

  if(x == FruitX && y == FruitY) {
    score++;
    FruitX = (rand() % width)+1;
    FruitY = (rand() % height)+1;

    nTail++;
  }

  for (int i = 0; i < nTail; i++)
    if (TailX[i] == x && TailY[i] == y) {
      gameOver = true;
    }

}

int main() {
  Setup();
  Draw();

  while(!gameOver) {
    Draw();
    Input();
    Logic();
  }

  getch();
  endwin();

  return 0;
}