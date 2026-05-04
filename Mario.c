#include <stdio.h>
#include <curses.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define WIDTH 80
#define HEIGHT 25

#define GROUND_Y 20
#define WORLD 1000

static int  hasGround[WORLD];

int isBlock(int x, int y) {
    if (y == 0 && x >= 35 && x <= 50) {
        return 1;
    }

    if (y == 15 && x >= 75 && x <= 90) {
        return 1;
    }

    if (y == 20 && x >= 130 && x <= 145) {
        return 1;
    }
    
    return 0;

}
//unfinished isPit function (Currently placeholder values)
int isPit(int x) {
    if (x >= 55 && x <= 65) {
        return 1;
    }

    if (x >= 110 && x <= 120) {
        return 1;
    }

    if (x >= 170 && x <= 180) {
        return 1;
    }
    
    return 0;
    
}
//unfinished isPit function (Currently placeholder values)
int isObstacle(int x, int y) {
    if (y == GROUND_Y - 1 && x == 20) {
        return 1;
    }

    if (y == GROUND_Y - 1 && x == 100) {
        return 1;
    }

    if (y == GROUND_Y - 1 && x == 150) {
        return 1;
    }

    return 0;
}

//display world in the terminal
void Animation(int marioX, int marioY, int cameraX, int hasStar, int starTimer, int speed) {
    clear();
    // world tiles 
    for (int i = 0; i < WIDTH; i++) {
        int dis = cameraX + i;
        if (dis < 0 || dis >= WORLD){ 
            continue;
        }
 
        // ground
        if (hasGround[dis]) {
            mvaddch(GROUND_Y, i, '=');
            for (int j = GROUND_Y + 1; j < HEIGHT - 1; j++)
                mvaddch(j, i, '#');
        }
 
        // objects sitting on the ground 
        switch (world[dis]) {
            case TILE_OBSTACLE:
                mvaddch(GROUND_Y - 1, i, 'X');
                break;
            case TILE_STAR:
                mvaddch(GROUND_Y - 1, i, '*');
                break;
            case TILE_CASTLE:
                // display the castle
                mvaddch(GROUND_Y - 4, i, '^');
                mvaddch(GROUND_Y - 3, i, '|');
                mvaddch(GROUND_Y - 2, i, '|');
                mvaddch(GROUND_Y - 1, i, '|');
                break;
        }
    }
 
    // Player 
    int x = marioX - camX;
    if (x >= 0 && x < WIDTH && marioY >= 0 && marioY < HEIGHT) {
        if (hasStar) attron(A_BOLD);
        // '$' when star-powered, '@' normally 
        mvaddch(marioY, x, hasStar ? '$' : '@');
        if (hasStar) attroff(A_BOLD);
    }
    //player controls
    mvprintw(0, 0, "Speed:%d  [SPACE/UP]=Jump  [Q]=Quit", speed);
 
    // progress bar 
    int bar     = 30;
    int progress = (marioX * bar) / (WORLD - 1);
    if (progress > bar) progress = bar;

    mvprintw(0, 45, "Progress:[");

    for (int p = 0; p < bar; p++)
        addch(p < progress ? '#' : '-');
    addch(']');
 
    // star power indicator 
    if (hasStar)
        mvprintw(1, 0, "*** STAR POWER ACTIVE: %3d frames ***", starTimer);
 
    refresh();
}


int main(){

    int speed;
    int marioX = 0;
    int marioY = GROUND_Y - 1;

    int velocityY = 0;
    int jumping = 0;

    int cameraX = 0;
    int castleX = 0;
    int marioX = 0;
    int marioY = 0;
    int starX = 0;
    int starY = GROUND_Y - 1;

    int hasStar = 0;
    int starTimer = 0;

    int win = 0;
    int gameover = 0;

  char world[WORLD];

  //variables to implement the curses library
  initscr();
  curs_set(FALSE);
  nodelay(stdscr, TRUE);
  keypad(stdscr, TRUE);
  srand(time(NULL));
  //randomized obstacles
  for (int i = 0; i <= WORLD; i++){
        int obstacle = rand() % 3;
        if (obstacle < 2){
            world[i] = 'X'
        }
        else if (obstacle < 3){
            world[i] = '*';
        }
        else{
            world[i] = '';
        }
  }
  int castle = WORLD - 1;

  while (!gameover && !win){ //while loop to dictate in-game physics of the player
        clear();

        int character = getch();

        if (character == ' ' && !jumping){ //When mario is not jumping
            velocityY = &speed; 
            jumping = 0;
        }

        velocityY +=1;
        marioY += velocityY;
        
        if (marioY >= GROUND_Y){ //when Mario is Idle
            marioY = GROUND_Y;
            velocityY = 0;
            jumping = 0;
        }

        if ((!hasStar &&  marioX == starX) && (!hasStar && marioY == starY)){}{ //once Mario makes contact with the star
            hasStar = 1;
            starTimer = 8;
        }

        if (hasStar){ //Star timer 
            starTimer--;
            if (starTimer <= 0){
                hasStar = 0;
            }
        }

    // Ground Collision maybe?
        if (!isPit(marioX) && marioY >= GROUND_Y - 1){
            marioY = GROUND_Y - 1;
            velocityY = 0;
            jumping = 0;
        }

        // Platform Collision prototype
        if (velocityY >= 0) {
            if (isBlock(marioX, marioY + 1)) {
                velocityY = 0
                jumping = 0
            }
        }
  



        if (WORLD == 0){
            break;
        }

        Animation(marioX, marioY, cameraX, hasStar, starTimer, speed);

  }

// WIN and GAMEOVER screens prototype
    clear();

    if (win) {
        move(10,30);
        printw("You Win!");
        move(12,22);
        printw("Mario has reached the castle!");
    } else {
        move(10,30);
        printw("GAME OVER");
        move(12,22);
        printw("Mario hit an obstacle or fell into a pit.");
    }

    move(15,22);
    printw("Press any key to exit game.");

    nodelay(stdscr, FALSE);
    getch();
    
    endwin();

    return 0;
}


}


