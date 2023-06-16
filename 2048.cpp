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

// to display menu if true
bool IN_MENU = true;
// to display high score if true
bool IN_HS = false;
// go to the game if true
bool IN_GAME = false;
// to display 'GAME OVER' if true
bool GAME_OVER = false;
// to display high score saving
bool IN_HS_INPUT = true;
// initial highlighted charecter in high score saving
char HS_CHAR_SELECTED = 'A';
// initial SCORE
uint32_t SCORE = 0;
// initial MOVES number
uint32_t MOVES = 0;

// struct of the menu entry
struct MENU_ENTRY {
    std::string name;
    bool active;
    Point pos;
};

MENU_ENTRY MENU_ENTRIES[2] = 
 {
    { "NEW GAME", true, Point(125, 150) },
    { "HIGH SCORE", false,  Point(117, 171) },
 };

// game tiles
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

void renderKeyboard(){
   char k[2];

   screen.alpha = 128;
   screen.pen = Pen(0, 255, 0);
   screen.rectangle(Rect(0, 120, 320, 120));
   screen.alpha = 255;
   for (int j = 0; j < 2; j++){
    for (int i = 0; i < 13; i++){
        k[0] = 'A' + i + 13 * j;
        screen.pen = HS_CHAR_SELECTED == k[0] ? Pen(0, 255, 0) : Pen(255, 255, 255);
        screen.text(k, font, Point(60 + 16 * i, 130 + 16 * j));
    }
   }
   screen.pen = Pen(255, 255, 255);
   screen.text("CONFIRM", font, Point(120, 180));
   
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
        renderKeyboard();
        for (auto e : MENU_ENTRIES){
            screen.pen = e.active ? Pen(0, 255, 0) : Pen(255, 255, 255);
            screen.text(e.name, font, e.pos);
        }
    }

    if (IN_HS){
        menuSurface->alpha = 25;
        
        menuSurface->pen = Pen(0,128,0);
        
        screen.stretch_blit(menuSurface, Rect(0, 0, SCREEN_WEIGHT, SCREEN_HEIGHT), Rect(0, 0, SCREEN_WEIGHT, SCREEN_HEIGHT));
        screen.pen = Pen(0,128,0);
        screen.alpha = 128;
        screen.rectangle(Rect(40, 0, 240, 240));
        screen.pen = Pen(255, 255, 255);
        screen.alpha = 255;
        screen.text("PETER:\t\t\t\t\t\t1000", font, Point(80, 85));
        screen.text("NORMAN:\t\t\t\t\t\t1000", font, Point(80, 105));
        screen.text("PETER:\t\t\t\t\t\t1000", font, Point(80, 125));
        screen.text("NORMAN:\t\t\t\t\t\t1000", font, Point(80, 145));
        screen.text("PETER:\t\t\t\t\t\t1000", font, Point(80, 165));
        screen.text("NORMAN:\t\t\t\t\t\t1000", font, Point(80, 185));
        screen.text("PETER:\t\t\t\t\t\t1000", font, Point(80, 205));

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
        screen.text("SCORE:" + std::to_string(SCORE), font, Point(245, 5));
        screen.text("MOVES:" + std::to_string(MOVES), font, Point(245, 25));
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

    if (IN_HS_INPUT){
        if (buttons.released & Button::DPAD_UP){
            HS_CHAR_SELECTED -= 13;
            if (HS_CHAR_SELECTED < 'A'){
                HS_CHAR_SELECTED += 26;
            }
        }
        if (buttons.released & Button::DPAD_DOWN){
            HS_CHAR_SELECTED += 13;
            if (HS_CHAR_SELECTED > 'Z'){
                HS_CHAR_SELECTED -= 26;
            }
        }
        if (buttons.released & Button::DPAD_LEFT){
            HS_CHAR_SELECTED--;
            if (HS_CHAR_SELECTED < 'A'){
                HS_CHAR_SELECTED = 'A';
            }
        }
        if (buttons.released & Button::DPAD_RIGHT){
            HS_CHAR_SELECTED++;
            if (HS_CHAR_SELECTED > 'Z'){
                HS_CHAR_SELECTED = 'Z';
            }
        }
    }

    if (IN_MENU) {
        if (buttons.released & (Button::DPAD_UP | Button::DPAD_DOWN)){
            MENU_ENTRIES[0].active = ! MENU_ENTRIES[0].active;
            MENU_ENTRIES[1].active = ! MENU_ENTRIES[1].active;
        }     
        if (buttons.released & Button::A && MENU_ENTRIES[0].active){
            IN_MENU = false;
            IN_GAME = true;
        }
        if (buttons.released & Button::A && MENU_ENTRIES[1].active){
            IN_HS = true;
            IN_MENU = false;
        }
        return;
    }

    if (IN_HS){
        if (buttons.released & Button::B){
            IN_HS = false;
            IN_MENU = true;
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