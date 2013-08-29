/**
 * effects.c
 * Ahmed Ismail
 *
 * Branching off old 4x4x4 code...
 */

#include "mbed.h"
#include "stdlib.h"
 
void rotZ(int*);
void effect_rain(int, double);
void effect_box(int);
void effect_rand(int);

// seeding rand
srand(time(NULL));
 
const double alph = 0.3827; // sin(22.5*)
const double bet = 0.9239; // cos(22.5*)
 enum
   {
      XDIM, YDIM, ZDIM
   } DIM;
  
// Random voxels light up at the top layer and falls to the bottom layer.
void effect_rain (int iterations, double speed){
    int i = 0;
    char topLayer = 7;
    // char randx = rand()%8, randy = rand()%8
    // char tempx = randx, tempy = randy;
    while (i < iterations){
        // tempx = rand()%8; tempy = rand()%8;
        // while(randx != tempx)
        //     randx = rand()%8;
        
        clearCube();
        /* Choose random led's on top layer to light
        */
        for(int i = 0; i < 8; i++)
            for(int j = 0; j < 8; j++)
                if(rand()%2)
                    plotPoint(i, rand()%8, topLayer);

        lightCube();
        wait(speed);
        clearCube();

    }
}
 
void rotZ(int* cube){
   
}
   
void effect_rand(int it){
    
}

void effect_box(int it){
  
}
            