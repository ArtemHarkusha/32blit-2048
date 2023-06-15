#include "2048.hpp"
#include "assets.hpp"
#include "game_logic.hpp"

using namespace blit;

#define SCREEN_HEIGHT 240
#define SCREEN_WEIGHT 320
#define SQURE_SIZE 58

int MAP[16] = {0};
// int MAP[16] = {16384, 8192, 4096, 2048,
//                65536, 32768, 2, 4,
//                2, 2, 2, 4,
//                2, 2, 2, 4};

Surface* menuSurface;

bool IN_MENU = true;
bool IN_HS = false;
bool IN_GAME = false;
bool GAME_OVER = false;
uint32_t SCORE = 0;
uint32_t MOVES = 0;

struct S_MENU {
    bool ngSelected = true;
    bool hsSelected = false;
    Pen inactive = Pen(255, 255, 255);
    Pen active = Pen(0, 255, 0);
};

struct S_MENU MY_MENU;


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
    genRandomPiece(MAP);
    genRandomPiece(MAP);
}

void renderBackground(){
    // clear the screen -- screen is a reference to the frame buffer and can be used to draw all things with the 32blit
    screen.clear();
    screen.pen = Pen(0, 0, 0);
    screen.rectangle(Rect(0, 0, 320, 240));
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

        if (MY_MENU.ngSelected){
            screen.pen = MY_MENU.active;
            screen.text("NEW GAME", font, Point(125, 150));
        }else {
            screen.pen = MY_MENU.inactive;
            screen.text("NEW GAME", font, Point(125, 150));
        }
        
        if (MY_MENU.hsSelected){
            screen.pen = MY_MENU.active;
            screen.text("HIGH SCORE", font, Point(117, 171));
        }else {
            screen.pen = MY_MENU.inactive;
            screen.text("HIGH SCORE", font, Point(117, 171));
        }
        
        return;
    }

    if (IN_HS){
        screen.alpha = 128;
        screen.pen = Pen(0,0,0);
        screen.rectangle(Rect(0, 0, 240, 240));
    }

    if(IN_GAME){
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
  screen.pen = Pen(0, 255, 0);
  screen.text("SCORE: " + std::to_string(SCORE), minimal_font, Point(255, 2));
  screen.text("MOVES: " + std::to_string(MOVES), minimal_font, Point(255, 15));
    }
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
        if (buttons.released & (Button::DPAD_UP | Button::DPAD_DOWN)){
            MY_MENU.hsSelected = ! MY_MENU.hsSelected;
            MY_MENU.ngSelected = ! MY_MENU.ngSelected;
        }     
        if (buttons.released & Button::A && MY_MENU.ngSelected){
            IN_MENU = false;
            IN_GAME = true;
        }
        if (buttons.released & Button::A && MY_MENU.hsSelected){
            IN_MENU = false;
            IN_HS = true;
        }
        return;
    }

    if (buttons.released & Button::DPAD_UP){
        moved = moveUp(MAP, &SCORE);
    }
    if (buttons.released & Button::DPAD_DOWN){
        moved = moveDown(MAP, &SCORE);
    }
    if (buttons.released & Button::DPAD_LEFT){
        moved = moveLeft(MAP, &SCORE);
    }
    if (buttons.released & Button::DPAD_RIGHT){
        moved = moveRight(MAP, &SCORE);
    }
    if (moved){
        genRandomPiece(MAP);
        MOVES++;
    }
    GAME_OVER = ! canMove(MAP);
}