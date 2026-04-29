#include <stdio.h>
#include <curses.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define WORLD 1000

int main(){



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
        
  



        if (WORLD == 0){
            break;
        }

  }




}


