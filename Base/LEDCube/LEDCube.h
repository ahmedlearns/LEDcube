/*
*   LEDcubeers
*      MCP23017
*
*
*/

#include "mbed.h"

#ifndef  LEDcube_H
#define  LEDcube_H

#define DEBUG 0
#define SPEED 1.0

// Definitions For MCP23017
#define IODIRA   0x00
#define IODIRB   0x01
#define IOCONA   0x0A
#define GPIOA    0x12
#define GPIOB    0x13

#define myDELAY 0.0000003 // 300 ns
#define CUBE_DELAY 0.0005

#define CUBE_X  0x00
#define CUBE_Y  0x01
#define CUBE_Z  0x02


class LEDcube {
public:
    unsigned char cubeData[8][8]; // [layer][x]

    LEDcube(I2C &i2c, PinName a, PinName b, PinName c, char deviceAddress, char numChip);
    
    //cubeData manipulation
    void clearCube();
    void plotPoint(signed char x, signed char y, signed char z);
    void clearPoint(signed char x, signed char y, signed char z);
    
    //Fancy Manipulation
    void drawDiamond(char x, char y, char z, char x2, char y2, char z2);
    void explodeDiamond(char x, char y, char z);
    
    void shiftArray();
    
    
    //Lighting LED functions
    void lightLED(int x, int y); 
    void lightPort(int x, char byte);
    void lightCube(double myWait);

private:
    // MCP23017 Variables
    I2C &_i2c;
    char _baseWrite;
    char _baseRead;
    char _numChips;
    
    void _init();
    void _write(char chip, char address, char byte);
    void _write(char chip, char address, char portA, char portB);
    
    // Decoder Variables
    DigitalOut _decode0;
    DigitalOut _decode1;
    DigitalOut _decode2;

};

unsigned char reverse_byte(unsigned char x);

#endif  //LEDcube
