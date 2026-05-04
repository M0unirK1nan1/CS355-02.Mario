#include <stdio.h>
#include <curses.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define WIDTH 80
#define HEIGHT 25

#define GROUND_Y 20
#define WORLD 250

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
    //randomized obstacles (Note from Aidan: Do the obstacles need to be randomized? I made code to generate obstacles below if you like it otherwise we can use your code)
    for (int i = 0; i <= WORLD; i++){
        int obstacle = rand() % 3;
        if (obstacle < 2){
            world[i] = 'X';
        }
        else if (obstacle < 3){
            world[i] = '*';
        }
        else{
            world[i] = ' ';
        }
  }
    int castle = WORLD - 1;



    printf("Enter Mario speed from 1 to 5: ");
    scanf("%d", &speed);

    if (speed < 1) {
        speed = 1;
    }

    if (speed > 5) {
        speed = 5;
    }

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

        //Camera following Mario
        cameraX = marioX - 10;

        if (cameraX < 0) {
            cameraX = 0;
        }

        // Obstacle collision
        if (isObstacle(marioX, marioY) && !hasStar) {
        gameover = 1;
        win = 0;
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
                velocityY = 0;
                jumping = 0;    
            }
        }

        // Falling into a pit
        if (isPit(marioX) && marioY > GROUND_Y) {
            gameover = 1;
            win = 0;
        }
        // Generate World
        for (int x = cameraX; x < cameraX + WIDTH; x++) {
            int screenX = x - cameraX;

            // Generate ground unless there is a pit
            if (!isPit(x)) {
                move(GROUND_Y, screenX);
                addch('=');
            }

            // Generate platforms
            for (int y = 0; y < HEIGHT; y++) {
                if (isBlock(x, y)) {
                    move(y, screenX);
                    addch('#');
                }
            }

            // Generate obstacles
            if (isObstacle(x, GROUND_Y - 1)) {
                move(GROUND_Y - 1, screenX);
                addch('X');
            }
            // Generate star if not collected yet
            if (!starCollected && x == starX) {
                move(starY, screenX);
                addch('*');
            }
            //Generate castle/flag goes here

        }
        

        if (WORLD == 0){
            break;
        }

  }

  // Mario Generation
  int marioScreenX = marioX - cameraX;

        if (marioScreenX >= 0 && marioScreenX < WIDTH) {
            move(marioY, marioScreenX);

            if (hasStar) {
                addch('M');
            } else {
                addch('@');
            }
        }

        refresh();

        usleep(80000);
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
