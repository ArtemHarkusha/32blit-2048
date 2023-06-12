#include "game.hpp"
#include "assets.hpp"
#include <string.h>
// #include "32blit.hpp"
using namespace blit;

#define SCREEN_HEIGHT 240
#define SCREEN_WEIGHT 320
#define SQURE_SIZE 58

int genRandomPiece();
bool moveLeft();
bool moveRight();
bool moveUp();
bool moveDown();
int MAP[16] = {2, 4, 8, 16,
               32, 64, 128, 256,
               8, 8, 2, 2,
               32, 64, 128, 256};

bool GAME_OVER=false;

// numbers
Rect n0 = Rect(84,0,7,7);
Rect n2 = Rect(0,0,7,7);
Rect n4 = Rect(7,0,7,7);
Rect n8 = Rect(14,0,7,7);
Rect n16 = Rect(21,0,7,7);
Rect n32 = Rect(28,0,7,7);
Rect n64 = Rect(35,0,7,7);
Rect n128 = Rect(42,0,7,7);
Rect n256 = Rect(49,0,7,7);
Rect n512 = Rect(56,0,7,7);
Rect n1024 = Rect(63,0,7,7);
Rect n2048 = Rect(70,0,7,7);
Rect n4096 = Rect(77,0,7,7);
///////////////////////////////////////////////////////////////////////////


void init() {
    set_screen_mode(ScreenMode::hires);
    screen.sprites = Surface::load(sheet);
    //genRandomPiece();
    //genRandomPiece();
}

void renderBackground(){
    // clear the screen -- screen is a reference to the frame buffer and can be used to draw all things with the 32blit
    screen.clear();
}

int genRandomPiece(){
    
    uint pos, val;
    uint max_rand = 10;

    // lower the changes of appearing '4'
    for (int i = 0; i < 4; i++){
        val = ((blit::random() % 2) == 0) ? 2 : 4;
        if (val == 2){
            break;
        }
    }
    
    while (max_rand != 0){
        pos = blit::random() % 16;
        if (MAP[pos] == 0){
            MAP[pos] = val;
            return 0;
        }
        max_rand--;
    }

    //did not find with rand(); try search
    for (int i = 0; i < 16; i++){
        if (MAP[i] == 0){
            MAP[i] = val;
            return 0;
        }
    }
    
    // we should not reach here. error
    return 1;
}

void swap(int * a, int * b){
    int tmp = * a;
    * a = * b;
    * b = tmp;
}

bool pushZerosInRaw(int start, int end, int step){
    bool moved = false;
    //1 - forward; -1 - backward
    int direction = (start - end) < 0 ? 1 : -1;
    int firstNonZero;
    if (direction == 1){
        firstNonZero = start;
        for (int i = start; i < end; i = i + step){
            if (MAP[i] != 0){
                swap(&MAP[i], &MAP[firstNonZero]);
                if (i != firstNonZero){
                    moved = true;
                }
                firstNonZero = firstNonZero + step;
            }
        }
    }
    if (direction == -1){
        firstNonZero = start - 1;
        for (int i = start - 1; i >= end; i = i + step){
            if (MAP[i] != 0){
                swap(&MAP[i], &MAP[firstNonZero]);
                if (i != firstNonZero){
                    moved = true;
                }
                firstNonZero = firstNonZero + step;
            }
        }
    }
    return moved;
}

bool mergeRaw(int start, int end, int step){
    bool moved = false;
    //1 - forward; -1 - backward
    int direction = (start - end) < 0 ? 1 : -1;
    if (direction == 1){
        for (int i = start; i < end - step; i = i + step){
            if (MAP[i] == MAP[i + step]){
                MAP[i] *= 2;
                MAP[i + step] = 0;
                if (MAP[i] != 0){
                    moved = true;
                }            
            }
        }
    }
    if (direction == -1){
        for (int i = start - 1; i > end - step; i = i + step){
            if (MAP[i] == MAP[i + step]){
                MAP[i] *= 2;
                MAP[i + step] = 0;
                if (MAP[i] != 0){
                    moved = true;
                }
            }
        }
    }
    return moved;
}

bool moveLeft(){
    bool moved = false;
    for (int i = 0; i < 4; i++){
        moved |= pushZerosInRaw(i * 4, i * 4 + 4, 1) | mergeRaw(i * 4, i * 4 + 4, 1) | 
                 pushZerosInRaw(i * 4, i * 4 + 4, 1);
    }
    return moved;
}

bool moveRight(){
    bool moved = false;
    for (int i = 0; i < 4; i++){
        moved |= pushZerosInRaw(i * 4 + 4, i * 4, -1) | mergeRaw(i * 4 + 4, i * 4, -1) | 
                 pushZerosInRaw(i * 4 + 4, i * 4, -1);
    }
    return moved;
}

bool moveUp(){
    bool moved = false;
    for (int i = 0; i < 4; i++){
        moved |= pushZerosInRaw(i, 16, 4) | mergeRaw(i, 16, 4) | pushZerosInRaw(i, 16, 4);
    }
    return moved;
}

bool moveDown(){
    bool moved = false;
    for (int i = 0; i < 4; i++){
        moved |= pushZerosInRaw(16 - i, 0, -4) | mergeRaw(16 - i, 0, -4) | pushZerosInRaw(16 - i, 0, -4);
    }
    return moved;
}

bool canMove(){
    bool canMove = false;
    // if there is an empty slot we can move
    for (int i = 0; i < 16; i++){
        if(MAP[i] == 0){
            canMove = true;
            break;
        }
    }

    // if numbers can be merged we can move
    for (int x = 0; x < 3; x++){
        for (int y = 0; y < 4; y++){
            if (MAP[x + 4*y] == MAP[MAP[x + 4*y +1]]){
                canMove = true;
                break;
            }
            if (MAP[x + 4*y] == MAP[x + 4*y + 4]){
                canMove = true;
                break;
            }
        }
    }

    return canMove;
}

void renderGameOver(){
    screen.clear();
    screen.pen = Pen(255, 255, 255);
    screen.rectangle(Rect(0, 0, 320, 240));
    screen.pen = Pen(0, 0, 0);
    screen.text("GAME OVER", fat_font, Point(115, 140));
}

///////////////////////////////////////////////////////////////////////////
//
// render(time)
//
// This function is called to perform rendering of the game. time is the 
// amount if milliseconds elapsed since the start of your game
//
void render(uint32_t time) {

    renderBackground();
    if(GAME_OVER){
        renderGameOver();
        return;
    }
    for (int x = 0; x < 4; x++) 
    {
        for (int y = 0; y < 4; y++)
        {
            if (MAP[x + 4*y] == 0){ screen.sprite(n0, Point(42 + x * SQURE_SIZE, 2 + y * SQURE_SIZE));}
            if (MAP[x + 4*y] == 2){ screen.sprite(n2, Point(42 + x * SQURE_SIZE, 2 + y * SQURE_SIZE));}
            if (MAP[x + 4*y] == 4){screen.sprite(n4, Point(42 + x * SQURE_SIZE, 2 + y * SQURE_SIZE));}
            if (MAP[x + 4*y] == 8){screen.sprite(n8, Point(42 + x * SQURE_SIZE, 2 + y * SQURE_SIZE));}
            if (MAP[x + 4*y] == 16){screen.sprite(n16, Point(42 + x * SQURE_SIZE, 2 + y * SQURE_SIZE));}
            if (MAP[x + 4*y] == 32){screen.sprite(n32, Point(42 + x * SQURE_SIZE, 2 + y * SQURE_SIZE));}
            if (MAP[x + 4*y] == 64){screen.sprite(n64, Point(42 + x * SQURE_SIZE, 2 + y * SQURE_SIZE));}
            if (MAP[x + 4*y] == 128){screen.sprite(n128, Point(42 + x *SQURE_SIZE, 2 + y * SQURE_SIZE));}
            if (MAP[x + 4*y] == 256){screen.sprite(n256, Point(42 + x * SQURE_SIZE, 2 + y * SQURE_SIZE));}
            if (MAP[x + 4*y] == 512){screen.sprite(n512, Point(42 + x * SQURE_SIZE, 2 + y * SQURE_SIZE));}
            if (MAP[x + 4*y] == 1024){screen.sprite(n1024, Point(42 + x * SQURE_SIZE, 2 + y * SQURE_SIZE));}
            if (MAP[x + 4*y] == 2048){screen.sprite(n2048, Point(42 + x * SQURE_SIZE, 2 + y * SQURE_SIZE));}
            if (MAP[x + 4*y] == 4096){screen.sprite(n4096, Point(42 + x * SQURE_SIZE, 2 + y * SQURE_SIZE));} 
        }
        
    }

}

    
///////////////////////////////////////////////////////////////////////////
//
// update(time)
//
// This is called to update your game state. time is the 
// amount if milliseconds elapsed since the start of your game
//
void update(uint32_t time) {
    //if (canMove()){
    //    memset(MAP, 0, sizeof(MAP));
    //}
    if (buttons.released & Button::DPAD_UP){
        if (moveUp()) {
            genRandomPiece();
        }
    }
    if (buttons.released & Button::DPAD_DOWN){
        if (moveDown()) {
            genRandomPiece();
        }
    }
    if (buttons.released & Button::DPAD_LEFT){
        if (moveLeft()) {
            genRandomPiece();
        }
    }
    if (buttons.released & Button::DPAD_RIGHT){
        if (moveRight()) {
            genRandomPiece();
        }
    }
    GAME_OVER = ! canMove();
}