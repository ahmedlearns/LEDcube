#include "mbed.h"
#include "LinkedList_ll/linkedlist_ll.h"
#include "LEDCube/LEDCube.h"

Serial pc(USBTX, USBRX); // tx, rx
I2C my2c(p28, p27);
LEDcube myCube(my2c, p23, p22, p21, 0x40, 4);

void initializeSnake(LLRoot * master);
void resetSnake(LLRoot * master);
void moveSnake(void);
void generateFruit(LLRoot * master);
void printSnakeLayer(const LLRoot * master, char layer);
void printSnakeCube(const LLRoot * master);
void getSnakeInput(void);
void setSnakeFlag(void);

LLRoot Snake;
Ticker tick;
Timer  sysClock;

InterruptIn b_xpos(p5);
InterruptIn b_xneg(p6);
InterruptIn b_ypos(p7);
InterruptIn b_yneg(p8);
InterruptIn b_zpos(p9);
InterruptIn b_zneg(p10);

char key, reset;
char errcode = 0;
char snakeFlag = 0;
char moveFlag = 0;

int main() {
    
    sysClock.reset();
    sysClock.start();
    my2c.frequency(400000);

    b_xpos.fall(&getSnakeInput);
    b_xpos.mode(PullUp);
    b_xneg.fall(&getSnakeInput);
    b_xneg.mode(PullUp);
    b_ypos.fall(&getSnakeInput);
    b_ypos.mode(PullUp);
    b_yneg.fall(&getSnakeInput);
    b_yneg.mode(PullUp);
    b_zpos.fall(&getSnakeInput);
    b_zpos.mode(PullUp);
    b_zneg.fall(&getSnakeInput);
    b_zneg.mode(PullUp);
    wait(0.2);
        
    initializeSnake(&Snake); // sets ticker to SPEED
    
    if(LLDEBUG) printListLL(&Snake);
    while(1) {

        myCube.lightCube(CUBE_DELAY);
        if(snakeFlag) {
            snakeFlag = 0;
            moveSnake();
        }
    }
    
    //freeListLL(&Snake);
    //printf("$ Program complete ... how did we get here?\r\n");

}

void setSnakeFlag(void) {
    snakeFlag = 1;
}

void initializeSnake(LLRoot * master) {
    /* Start Snake at 3 in length
     * x, y, z:layer
     */ 
      
    //if(master != NULL)      // Uh oh, non-empty snake?
    //    freeListLL(master); // No matter
    
    initializeLL(master);
    __disable_irq();
    addToHeadLL(master, 0, 0, 0);
    __enable_irq();
    // BEAMMEUPSCOTTY
    myCube.plotPoint(0, 0, 0);
    master->direction = XPOS;
    //master->length = 1;
   
    generateFruit(master);
    
    // Note: What if fruit is generated on snake?
    // The user will not be able to see it until all snake nodes have gone
    // over it
    
    tick.attach(&setSnakeFlag, SPEED);
}

void generateFruit(LLRoot * master) {
    if(master == NULL) return;
    
    /* Generation data for fruit between 0-7 
     * http://c-faq.com/lib/randrange.html
     */
     
     
    sysClock.stop();
    
    srand (sysClock.read_us());     // Seed random generator
    //sysClock.reset();
    
    master->fruit_x = (char) (rand() / (RAND_MAX / CUBEDIM + 1));
    master->fruit_y = (char) (rand() / (RAND_MAX / CUBEDIM + 1));
    master->fruit_z = (char) (rand() / (RAND_MAX / CUBEDIM + 1));
    myCube.plotPoint(master->fruit_x, master -> fruit_y, master->fruit_z);
    sysClock.start();
}

void resetSnake(LLRoot * master) {
    if(master == NULL) return;
    
    if(LLDEBUG) printf("###### GAME RESET; Setting snake back to 0,0,0 ######\r\n\r\n");
    
    tick.detach();
    freeListLL(master);
    
    myCube.clearCube();
    initializeSnake(master);
    
    
    //sysClock.reset();
    if(LLDEBUG) printListLL(master);

}

void getSnakeInput(void) {
    //LLRoot * master = &Snake;
    char current_dir = Snake.direction;
    
    /* Determines which button was pressed (out of 6), and prevents
     * the snake from turning in the opposite direction
     * Ex: when going x dir, cannot turn -x
     */
    if(moveFlag) {
        if(b_xpos == 0 && current_dir != XNEG) Snake.direction = XPOS;
        else if(b_xneg == 0 && current_dir != XPOS) Snake.direction = XNEG;
        else if(b_ypos == 0 && current_dir != YNEG) Snake.direction = YPOS;
        else if(b_yneg == 0 && current_dir != YPOS) Snake.direction = YNEG;
        else if(b_zpos == 0 && current_dir != ZNEG) Snake.direction = ZPOS;
        else if(b_zneg == 0 && current_dir != ZPOS) Snake.direction = ZNEG;
        moveFlag = 0;    
    }
    
    // else unknown data condition
}

/* May require the use of a ticker.attach in order to update the snake every
 * x seconds. <http://mbed.org/handbook/Ticker>
 */
void moveSnake(void) {
/*
 * NOTE: This is part of an ISR called by the Ticker
 * So no printf, limited use of malloc.
 * If issues occur, split up function
 */
 /* TODO: test mbed ISR, determine how much memory allocation,
  * max time for ISR, etc
  * REMOVE PRINTF ONCE MIGRATION OCCURS
  */
    LLRoot * master = &Snake;
    LLNode * tmp;
    char sHead_x, sHead_y, sHead_z;
    char apple_x, apple_y, apple_z;
    signed char XDIR, YDIR, ZDIR;
    char count;
    
    if(master == NULL) return;
    
    moveFlag = 1;
    errcode = 0;
    
    /* Careful usage required, stale data can occur with these variables */
    sHead_x = master->head->x;
    sHead_y = master->head->y;
    sHead_z = master->head->z;
    apple_x = master->fruit_x;
    apple_y = master->fruit_y;
    apple_z = master->fruit_z;
    
    XDIR = YDIR = ZDIR = 0;
    switch(master->direction) {
        case XPOS:
            XDIR = 1;
            break;
        case XNEG:
            XDIR = -1;
            break;
        case YPOS:
            YDIR = 1;
            break;
        case YNEG:
            YDIR = -1;
            break;
        case ZPOS:
            ZDIR = 1;
            break;
        case ZNEG:
            ZDIR = -1;
            break;
            
        default:
            break;
    }
    
    /* Update snake nodes */
    __disable_irq();
    addToHeadLL(master, sHead_x + XDIR, sHead_y + YDIR, sHead_z + ZDIR);
    __enable_irq();
    // BEAMMEUPSCOTTY
    myCube.plotPoint(sHead_x + XDIR, sHead_y + YDIR, sHead_z + ZDIR);
    if(LLDEBUG) printf("debug: adding [%d %d %d]\r\n", sHead_x + XDIR, sHead_y + YDIR, sHead_z + ZDIR);
    
    /* Boundary/Self checking */
    count = 0;
    tmp = master->head;
    while(tmp != NULL) {
        ++count;
        
        /* Check head coordinates against all other nodes */
        if(count > 1) {
            if((master->head->x == tmp->x) && (master->head->y == tmp->y) && (master->head->z == tmp->z)) {
                if(LLDEBUG) printf("debug: conflict with snake head: [%d %d %d] ... resetting game\r\n", master->head->x, master->head->y, master->head->z);
                resetSnake(master);
                reset = 1;
                errcode = 1;
                break;
            }
        }
        
        // TODO: flash cube face on wall hit
        // in if statements
        
        if(tmp->x > BOUNDARY) {
            resetSnake(master);
            reset = 1; 
            errcode = 3;
            // TODO: light right wall
            break;
        }
        if(tmp->x < 0) {
            resetSnake(master);
            reset = 1; 
            errcode = 3;
            // TODO: light left wall
            break;
        }
        if(tmp->y > BOUNDARY) {
            resetSnake(master);
            reset = 1;
            errcode = 3;
            // TODO: light back wall
            break;
        }
        if(tmp->y < 0) {
            resetSnake(master);
            reset = 1;
            errcode = 3;
            // TODO: light front wall
            break;
        }
        if(tmp->z > BOUNDARY) {
            resetSnake(master);
            reset = 1;
            errcode = 3;
            // TODO: light top wall
            break;
        }
        if(tmp->z < 0) {
            resetSnake(master);
            reset = 1;
            errcode = 3;
            // TODO: light bottom wall
            break;
        }
        tmp = tmp->next;
    }
    //master->length = count - 1;

    if(reset) { reset = 0; return; }

    /* Fruit detection */
    if((sHead_x == apple_x) && (sHead_y == apple_y) && (sHead_z == apple_z)) {
        generateFruit(master);
        errcode = 2;
        if(LLDEBUG) printf("debug: keeping tail [%d %d %d]\r\n", master->tail->x, master->tail->y, master->tail->z);
    }else{
        /* If we detect fruit, there is no need to remove the tail node.
         * Otherwise we can remove it to produce the snake movement
         */
        // BEAMMEUPSCOTTY
        
        myCube.clearPoint(master->tail->x, master->tail->y, master->tail->z);
        if(LLDEBUG) printf("debug: removing [%d %d %d] because no fruit detected\r\n", master->tail->x, master->tail->y, master->tail->z);
        __disable_irq();
        removeTailLL(master);
        __enable_irq();
    }
    
    if(LLDEBUG) printListLL(master);       /* Display LL header and node data */
}

void printSnakeCube(const LLRoot * master) {
    int i;
    
    if(master == NULL) return;
    
    for(i = 0; i < 8; i++) {
        printf("~~~~~LAYER%d~~~~~\r\n", i);
        printSnakeLayer(master, i);
    }
    printf("ERRCODE: %d\r\n", errcode);
}

void printSnakeLayer(const LLRoot * master, char layer) {
    LLNode * node = master->head;       /* Begin at the head node */
    char board[8][8];
    int x, y;
        
    if(master == NULL) return;
    
    for(x = 0; x < 8; ++x)
        for(y = 0; y < 8; ++y)
            board[x][y] = '-';

    if(layer == master->fruit_z)
        board[master->fruit_y][master->fruit_x] = 'x';
        
    while(node != NULL) {               /* If the node exists, pull the data from it */
        if(node->z == layer) {
            board[node->y][node->x] = 'o';
        }
        node = node->next;
    }
    
    if(layer == master->head->z)
        board[master->head->y][master->head->x] = 's';
        
    for(y = 7; y >= 0; --y) {
        for(x = 0; x < 8; ++x) {
            printf("%c", board[y][x]);
        }
        printf("\r\n");
    }

}
