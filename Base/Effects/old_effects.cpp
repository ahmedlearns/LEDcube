/**
 * effects.c
 * Ahmed Ismail
 *
 * Branching off old 4x4x4 code...
 */

#include "mbed.h"
#include "stdlib.h"
 
DigitalOut myled(LED1);
 
// 8 bit bus to control two FF's
DigitalOut d0(p10);
DigitalOut d1(p11);
DigitalOut d2(p12);
DigitalOut d3(p13);
DigitalOut d4(p14);
DigitalOut d5(p15);
DigitalOut d6(p16);
DigitalOut d7(p17);
 
// Two pins for the CP pins on the FF's
DigitalOut cp0(p22);
DigitalOut cp1(p21); 
 
// Two pins for the input to to the 3 to 8 decoder
DigitalOut a0(p24);
DigitalOut a1(p23);
 
// 4 x 4 x 4 Cube
const int SIDE = 4;
 
void drawLayer(int*, int);
void selPin(int, int);
void selLED(int*, int);
void clear(int);
void RotZ(int*);
void DrawCube(int* cube);
void effect_rain(int, double);
 
const double alph = 0.3827; // sin(22.5*)
const double bet = 0.9239; // cos(22.5*)
 enum
   {
      XDIM, YDIM, ZDIM
   } DIM;
  
int EMPTYARRAY[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
 
int main() {
 void effect_rand(int);
 void effect_box(int);
 void clearAll();
   // int layer[16] = {1,0,0,1,1,0,0,0,1,1,0,1,1,1,0,1};
   int layer[16] = {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};
   int cube[64] = {1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1};
//    layer[0] = 1;
//    layer[3] = 1;
//    layer[3] = 1;
//    layer[3] = 1;
//    layer[4] = 1;
//    layer[8] = 1;
//    layer[9] = 1;
//    layer[11] = 1;
//    layer[12] = 1;
//    layer[13] = 1;
//    layer[15] = 1;
    int x = 0;
    int y = 0;
    srand(546);
    int t = 0;
while(1){
            // DrawCube(cube);
            // if(t>1000){
            // for(int j = 0; j<64; ++j){
            // cube[j] = rand() % 2;}
            // t=0;
             clearAll();wait(1);
            effect_rand(8000);
            clearAll();wait(1);
            effect_box(8000);
            clearAll();wait(1);
            effect_rain(15, 0.16);
    
        }
// ++t;
  
}
// Draw a layer using a 16 int array (SIDE*SIDE)
void drawLayer(int* layer, int selLayer){
    switch(selLayer){
        case 0:
            a0 = 0;
            a1 = 0;
            break;
        case 1:
            a0 = 1;
            a1 = 0;
            break;
        case 2:
            a0 = 0;
            a1 = 1;
            break;
        case 3:
            a0 = 1;
            a1 = 1;
            break;
    }
    
    for(int i = 0; i < (SIDE*SIDE); i+=8){
        int ffSel = i >> 3; // divide by 8 to determine which FF (will give 0 or 1)
        selLED(layer+ffSel*8, ffSel);
    }
}
// selLED will turn on the selected LEDS on a layer (8 at a time)
// cp = 0 for first FF
// cp = 1 for second FF
void selLED(int* ffOut, int cp){
    // change data, bring latch low then high to latch input to output
    for(int i = 0; i < 8; i++){
        selPin(i, ffOut[i]);
    }
    if(cp == 0){
        cp0 = 0;    // pull latch low
        // wait() ??
        cp0 = 1;
        cp1 = 0;
    }
    if(cp == 1){
        cp1 = 0;    // pull latch low
        // wait() ??
        cp0 = 0;
        cp1 = 1;
    }
}
 
// Turn the selected pin on the FF to the input value
void selPin(int i, int val){
    switch(i){
        case 0:
            d0 = val;
            break;
        case 1:
            d1 = val;
            break;
        case 2:
            d2 = val;
            break;
        case 3:
            d3 = val;
            break;
        case 4:
            d4 = val;
            break;
        case 5:
            d5 = val;
            break;
        case 6:
            d6 = val;
            break;     
        case 7:
            d7 = val;
            break;           
    }
}  
 
// Random voxels light up at the top layer and falls to the bottom layer.
void effect_rain (int iterations, double speed){
    int numi = 70;
    int count = 0; 
    
    int layer[16];
    int layer2[16];
    count = 0;
 
  /*  count = 0;
        while ( !(count > 0 && count <=3)){
        count = 0;
        for(int i = 0; i < 16; i++){
            layer2[i] = rand() % 2;
            if(layer2[i]) count++;
        }
    }
    */
    clear(3); clear(2); clear(1); clear(0);
    
    int i = 0;
    while(i < iterations){
    count = 0;
    // Get new L1
        while ( !(count > 0 && count <=3)){
        count = 0;
        for(int i = 0; i < 16; i++){
            layer[i] = rand() % 2;
            if(layer[i]) count++;
        }
    } 
        
        clear(0);clear(2);
        for(int test = 0 ; test < numi ; test++){
        clear(1);
        drawLayer(layer, 3);
        wait(0.001);
        clear(3);
        drawLayer(layer2,1);
        wait(0.001);
        }
        
        clear(3);clear(1);
        for(int test = 0 ; test < numi ; test++){
        drawLayer(layer, 2);
        wait(0.001);
        clear(2);
        drawLayer(layer2,0);
        wait(0.001);
        clear(0);
        }
        // Swap layer
        for(int i = 0 ; i<16  ;i++){
        layer2[i]=layer[i];
        }
        // Get new L1
        count = 0;
    while ( !(count > 0 && count <=3)){
        count = 0;
        for(int i = 0; i < 16; i++){
            layer[i] = rand() % 2;
            if(layer[i]) count++;
        }
    } 
      
        clear(2);clear(0);
        for(int test = 0 ; test < numi; test++){
        drawLayer(layer2, 1);
        wait(0.001);
        clear(1);
        drawLayer(layer,3);
        wait(0.001);
        clear(3);
        }
        //wait(speed);
        
        clear(1);clear(3);
        for(int test = 0 ; test < numi ; test++){
        drawLayer(layer2, 0);
        wait(0.001);
        clear(0);
        drawLayer(layer,2);
        wait(0.001);
        clear(2);
        }
                // Swap layer
        for(int i = 0 ; i<16  ;i++){
        layer2[i]=layer[i];
        }
        i++;
    }
    
}
 
void RotZ(int* cube){
   int x,y,z = 0; // init coordinates
   int temp[SIDE*SIDE*SIDE];
   int* ptemp = &temp[0];
   int newx,newy,newz;
   
   for(int i = 0; i < SIDE*SIDE*SIDE ; i++){
       z = i/(SIDE*SIDE);
       int newi = i - z*SIDE*SIDE;
       y = newi / SIDE;
       x = newi - y*SIDE;
       
       newx = bet*x - alph*y;
       newy = alph*x + bet*y;
       newz = z;
       ptemp[SIDE*SIDE*newz + SIDE*newy + newx] = 1;
   }
   
     for(int i = 0; i < SIDE*SIDE*SIDE ; i++){
        cube[i] = ptemp[i];
     } 
 
}
   
  void DrawCube(int* cube){
  double t = 0.45;
    for(int i = 0; i<SIDE*SIDE*SIDE ; i+= SIDE*SIDE){
       drawLayer(EMPTYARRAY, i/(SIDE*SIDE));
        drawLayer((cube+i),i/(SIDE*SIDE));
        wait(0.0001);
    }
 
  } 
  
  void clear(int layer){
    drawLayer(EMPTYARRAY, layer);
    }
    
  void effect_rand(int it){
    int i = 0;
    int cube[64];
    int x = 0;
    while (i<it){
    
    if(x > 500){
    for(int k = 0; k < 64 ; k++){
    cube[k] = rand() % 2;
    }
    x = 0;
    }
    DrawCube(cube);
    x++;
    
    
    i++;
    }
    
    }
  void effect_box(int it){
  
  int cube[64] = {1,1,1,1,1,0,0,1,1,0,0,1,1,1,1,1, 1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,1, 1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,1, 1,1,1,1,1,0,0,1,1,0,0,1,1,1,1,1};
  int i = 0;
  while(i < it){i++;
  DrawCube(cube);
  }
  
  }
  
  void clearAll(){clear(0);clear(1);clear(2);clear(3);}
            