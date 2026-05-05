#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <curses.h>

//W x H of the bash terminal in order to play the game
#define WIDTH 80
#define HEIGHT 24

//ground variable
#define GROUND_Y 20
//length of the level
#define WORLD_SIZE 250

int isBlock(int x, int y) {
    if (y == 13 && x >= 35 && x <= 45) {
        return 1;
    }

    if (y == 13 && x >= 75 && x <= 90) {
        return 1;
    }

    if (y == 16 && x >= 130 && x <= 145) {
        return 1;
    }

    return 0;
}

int isPit(int x) {
    if (x >= 55 && x <= 62) {
        return 1;
    }

    if (x >= 110 && x <= 118) {
        return 1;
    }

    if (x >= 170 && x <= 180) {
        return 1;
    }

    return 0;
}

int isObstacle(int x, int y) {
    if (y == GROUND_Y - 1 && x == 28) {
        return 1;
    }

    if (y == GROUND_Y - 1 && x == 95) {
        return 1;
    }

    if (y == GROUND_Y - 1 && x == 155) {
        return 1;
    }

    return 0;
}

int main() {
    int speed;
    int marioX = 2;
    int marioY = GROUND_Y - 1;

    int oldX;
    int oldY;

    int velocityY = 0;
    int jumping = 0;

    int cameraX = 0;
    int castleX = 220;

    int starX;
    int starY = GROUND_Y - 1;
    int starCollected = 0;
    int hasStar = 0;
    int starTimer = 0;

    int gameOver = 0;
    int win = 0;

    int ch;

    //user chooses the constant speed
    printf("Enter Mario speed from 1 to 5: ");
    scanf("%d", &speed);

    if (speed < 1) {
        speed = 1;
    }

    if (speed > 5) {
        speed = 5;
    }

    //randomize the star powerup spot
    srand(time(NULL));
    do {
    starX = 40 + rand() % 130;
    } while (isPit(starX));

    //curses library to call the game
    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);

    while (!gameOver) { //while loop to show in game functionality 
        clear();

        oldX = marioX;
        oldY = marioY;

        /*
            INPUT
        */
        ch = getch();

        if (ch == 'q') {
            gameOver = 1;
        }
        //when player moves to the left
        if (ch == KEY_LEFT) {
            for (int i = 0; i < speed; i++) {
                marioX--;

                if (marioX < 0) {
                marioX = 0;
                }

                if (isObstacle(marioX, marioY) && !hasStar) {
                    gameOver = 1;
                    win = 0;
                    }

                if (!starCollected && marioX >= starX - 1 && marioX <= starX + 1 && marioY == starY) {
                    starCollected = 1;
                    hasStar = 1;
                    starTimer = 80;
                }
            }
        }
        
        //when player moves to the right
        if (ch == KEY_RIGHT) {
            for (int i = 0; i < speed; i++) {
                marioX++;

                if (marioX >= WORLD_SIZE) {
                    marioX = WORLD_SIZE - 1;
                }

                if (isObstacle(marioX, marioY) && !hasStar) {
                    gameOver = 1;
                    win = 0;
                }

                if (!starCollected && marioX >= starX - 1 && marioX <= starX + 1 && marioY == starY) {
                    starCollected = 1;
                    hasStar = 1;
                    starTimer = 80;
                }
            }
        }

        if ((ch == ' ' || ch == KEY_UP) && jumping == 0) {
            velocityY = -4;
            jumping = 1;
        }


        if (marioX < 0) {
            marioX = 0;
        }

        if (marioX >= WORLD_SIZE) {
            marioX = WORLD_SIZE - 1;
        }


        //Gravity

        marioY += velocityY;
        velocityY++;

        if (velocityY < 0 && isBlock(marioX, marioY)) {
            marioY = marioY + 1;
            velocityY = 0;
        }


            //Ground Collision
        if (!isPit(marioX) && marioY >= GROUND_Y - 1) {
            marioY = GROUND_Y - 1;
            velocityY = 0;
            jumping = 0;
        }


        //Platform Collision
        if (velocityY >= 0 && isBlock(marioX, marioY + 1)) {
            velocityY = 0;
            jumping = 0;
        }


        //Obstacle collision
        if (isObstacle(marioX, marioY) && !hasStar) {
            gameOver = 1;
            win = 0;
        }


        //Mario falls into pit

        if (isPit(marioX) && marioY > GROUND_Y) {
            gameOver = 1;
            win = 0;
        }


        //Mario collects star
        if (!starCollected && marioX >= starX - 1 && marioX <= starX + 1 && marioY == starY) {
            starCollected = 1;
            hasStar = 1;
            starTimer = 80;
        }
        // Mario star timer limit decrement
        if (hasStar) {
            starTimer--;

            if (starTimer <= 0) {
                hasStar = 0;
            }
        }


        //WIN CONDITION

        if (marioX >= castleX) {
            gameOver = 1;
            win = 1;
        }


        //CAMERA FOLLOWS MARIO

        cameraX = marioX - 10;

        if (cameraX < 0) {
            cameraX = 0;
        }


        //GENERATE HUD

        move(0, 0);
        printw("Mario X: %d   Speed: %d   Star: ", marioX, speed);

        if (hasStar) {
            printw("ON ");
        } else {
            printw("OFF");
        }
        
        move(1, 0);
        printw("Left/Right arrows to move. Space or Up arrow to jump. q to quit.");

        /*
            GENERATE
        */
        for (int x = cameraX; x < cameraX + WIDTH; x++) {
            int screenX = x - cameraX;

            //Generate ground unless there is a pit
            if (!isPit(x)) {
                move(GROUND_Y, screenX);
                addch('=');
            }


            //Generate platforms
            for (int y = 0; y < HEIGHT; y++) {
                if (isBlock(x, y)) {
                    move(y, screenX);
                    addch('#');
                }
            }

            //generate obstacles
            if (isObstacle(x, GROUND_Y - 1)) {
                move(GROUND_Y - 1, screenX);
                addch('X');
            }

                //Generate star if it has not been collected
            if (!starCollected && x == starX) {
                move(starY, screenX);
                addch('*');
            }


                //Generate castle
            if (x == castleX) {
                move(GROUND_Y - 1, screenX);
                addch('|');

                move(GROUND_Y - 2, screenX);
                addch('|');

                move(GROUND_Y - 3, screenX);
                addch('|');

                move(GROUND_Y - 4, screenX);
                addch('^');
            }
        }

        //generate mario
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

    clear();
    //if else block based on in-game interaction of reaching the castle or causing a game over
    if (win) {
        move(10, 30);
        printw("YOU WIN!");
        move(12, 22);
        printw("Mario reached the castle!");
    } else {
        move(10, 30);
        printw("GAME OVER");
        move(12, 18);
        printw("Mario hit an obstacle or fell into a pit.");
    }

    move(15, 22);
    printw("Press any key to exit.");
    
    nodelay(stdscr, FALSE);
    getch();
    //Game is ended
    endwin();

    return 0;
}
