#include "2048.hpp"
#include "assets.hpp"
using namespace blit;

#define SCREEN_HEIGHT 240
#define SCREEN_WEIGHT 320
#define SQURE_SIZE 58

int genRandomPiece();
bool moveLeft();
bool moveRight();
bool moveUp();
bool moveDown();
int MAP[16] = {0};
// int MAP[16] = {16384, 8192, 4096, 2048,
//                65536, 32768, 2, 4,
//                2, 2, 2, 4,
//                2, 2, 2, 4};

Surface* menuSurface;

bool IN_MENU = true;
bool GAME_OVER = false;
uint32_t SCORE = 0;
uint32_t MOVES = 0;

// numbers
Rect n0 = Rect(0,0,7,7);
Rect n2 = Rect(7,0,7,7);
Rect n4 = Rect(14,0,7,7);
Rect n8 = Rect(21,0,7,7);
Rect n16 = Rect(28,0,7,7);
Rect n32 = Rect(35,0,7,7);
Rect n64 = Rect(42,0,7,7);
Rect n128 = Rect(49,0,7,7);
Rect n256 = Rect(56,0,7,7);
Rect n512 = Rect(63,0,7,7);
Rect n1024 = Rect(70,0,7,7);
Rect n2048 = Rect(77,0,7,7);
Rect n4096 = Rect(84,0,7,7);
Rect n8191 = Rect(91,0,7,7);
Rect n16384 = Rect(98,0,7,7);
Rect n32768 = Rect(105,0,7,7);
Rect n65536 = Rect(112,0,7,7);
///////////////////////////////////////////////////////////////////////////


void init() {
    set_screen_mode(ScreenMode::hires);
    screen.sprites = Surface::load(sheet);
    menuSurface = Surface::load(menu);
    genRandomPiece();
    genRandomPiece();
}

void renderBackground(){
    // clear the screen -- screen is a reference to the frame buffer and can be used to draw all things with the 32blit
    screen.clear();
    screen.pen = Pen(0, 0, 0);
    screen.rectangle(Rect(0, 0, 320, 240));
}

int genRandomPiece(){
    
    int pos, val;
    int max_rand = 15;

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

bool pushZerosInRaw(int start, int end, int step){
    bool moved = false;
    //1 - forward; -1 - backward
    int direction = (start - end) < 0 ? 1 : -1;
    int firstNonZero;
    if (direction == 1){
        firstNonZero = start;
        for (int i = start; i < end; i = i + step){
            if (MAP[i] != 0){
                if (i != firstNonZero){
                    MAP[firstNonZero] = MAP[i];
                    MAP[i] = 0;
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
                if (i != firstNonZero){
                    MAP[firstNonZero] = MAP[i];
                    MAP[i] = 0;
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
            if (MAP[i] != 0 && MAP[i] == MAP[i + step]){
                MAP[i] *= 2;
                SCORE += MAP[i + step];
                MAP[i + step] = 0;
                moved = true;        
            }
        }
    }
    if (direction == -1){
        for (int i = start - 1; i >= end - step; i = i + step){
            if (MAP[i] != 0 && MAP[i] == MAP[i + step]){
                MAP[i] *= 2;
                SCORE += MAP[i + step];
                MAP[i + step] = 0;
                moved = true;
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
    // if there is an empty slot we can move
    for (int i = 0; i < 16; i++){
        if(MAP[i] == 0){
            return true;
            break;
        }
    }

    // if numbers can be merged we can move
    // these nested loops only checks 3x3 field
    // x x x o
    // x x x o
    // x x x o
    // o o o 0
    for (int x = 0; x < 3; x++){     
        for (int y = 0; y < 3; y++){
            if (MAP[x + 4*y] == MAP[x + 4*y +1]){
                 return true;
                 break;
            }
            if (MAP[x + 4*y] == MAP[x + 4*y + 4]){
                 return true;
                 break;
            }
        }
    }
    
    // now let's check the final raw
    for (int i = 12; i < 15; i++){
        if (MAP[i] == MAP[i + 1]){
            return true;
            break;
        }
    }

    // and the final column
    for (int i = 3; i < 12; i = i + 4){
        if (MAP[i] == MAP[i + 4]){
            return true;
            break;
        }
    }


    return false;
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
    if (IN_MENU) {
        screen.stretch_blit(menuSurface, Rect(0, 0, SCREEN_WEIGHT, SCREEN_HEIGHT), Rect(0, 0, SCREEN_WEIGHT, SCREEN_HEIGHT));
        screen.pen = Pen(255, 255, 255);
        //screen.rectangle(Rect(85, 45, 120, 56));
        screen.text("NEW GAME", outline_font, Point(135, 150));
        return;
    }
    for (int x = 0; x < 4; x++) 
    {
        for (int y = 0; y < 4; y++)
        {
            if (MAP[x + 4*y] == 0){ screen.sprite(n0, Point(5 + x * SQURE_SIZE, 2 + y * SQURE_SIZE));}
            if (MAP[x + 4*y] == 2){ screen.sprite(n2, Point(5 + x * SQURE_SIZE, 2 + y * SQURE_SIZE));}
            if (MAP[x + 4*y] == 4){screen.sprite(n4, Point(5 + x * SQURE_SIZE, 2 + y * SQURE_SIZE));}
            if (MAP[x + 4*y] == 8){screen.sprite(n8, Point(5 + x * SQURE_SIZE, 2 + y * SQURE_SIZE));}
            if (MAP[x + 4*y] == 16){screen.sprite(n16, Point(5 + x * SQURE_SIZE, 2 + y * SQURE_SIZE));}
            if (MAP[x + 4*y] == 32){screen.sprite(n32, Point(5 + x * SQURE_SIZE, 2 + y * SQURE_SIZE));}
            if (MAP[x + 4*y] == 64){screen.sprite(n64, Point(5 + x * SQURE_SIZE, 2 + y * SQURE_SIZE));}
            if (MAP[x + 4*y] == 128){screen.sprite(n128, Point(5 + x *SQURE_SIZE, 2 + y * SQURE_SIZE));}
            if (MAP[x + 4*y] == 256){screen.sprite(n256, Point(5 + x * SQURE_SIZE, 2 + y * SQURE_SIZE));}
            if (MAP[x + 4*y] == 512){screen.sprite(n512, Point(5 + x * SQURE_SIZE, 2 + y * SQURE_SIZE));}
            if (MAP[x + 4*y] == 1024){screen.sprite(n1024, Point(5 + x * SQURE_SIZE, 2 + y * SQURE_SIZE));}
            if (MAP[x + 4*y] == 2048){screen.sprite(n2048, Point(5 + x * SQURE_SIZE, 2 + y * SQURE_SIZE));}
            if (MAP[x + 4*y] == 4096){screen.sprite(n4096, Point(5 + x * SQURE_SIZE, 2 + y * SQURE_SIZE));}
            if (MAP[x + 4*y] == 8192){screen.sprite(n8191, Point(5 + x * SQURE_SIZE, 2 + y * SQURE_SIZE));} 
            if (MAP[x + 4*y] == 16384){screen.sprite(n16384, Point(5 + x * SQURE_SIZE, 2 + y * SQURE_SIZE));} 
            if (MAP[x + 4*y] == 32768){screen.sprite(n32768, Point(5 + x * SQURE_SIZE, 2 + y * SQURE_SIZE));}
            if (MAP[x + 4*y] == 65536){screen.sprite(n65536, Point(5 + x * SQURE_SIZE, 2 + y * SQURE_SIZE));} 
            
        }
        
    }

  // Draw the SCORE
  screen.pen = Pen(255, 255, 255);
  screen.text("SCORE: " + std::to_string(SCORE), minimal_font, Point(255, 2));
  screen.text("MOVES: " + std::to_string(MOVES), minimal_font, Point(255, 15));

  if(GAME_OVER){
    renderGameOver();
    return;
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
    
    bool moved = false;
    if (IN_MENU) {
        if (buttons.released & Button::A){
            IN_MENU = false;
        }
        return;
    }

    if (buttons.released & Button::DPAD_UP){
        moved = moveUp();
    }
    if (buttons.released & Button::DPAD_DOWN){
        moved = moveDown();
    }
    if (buttons.released & Button::DPAD_LEFT){
        moved = moveLeft();
    }
    if (buttons.released & Button::DPAD_RIGHT){
        moved = moveRight();
    }
    if (moved){
        genRandomPiece();
        MOVES++;
    }
    GAME_OVER = ! canMove();
}