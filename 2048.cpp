#include "2048.hpp"
#include "assets.hpp"
#include "game_logic.hpp"
#include <cstring>

using namespace blit;

#define SCREEN_HEIGHT 240
#define SCREEN_WEIGHT 320
#define SQURE_SIZE 58
//#define SHOW_FPS

int MAP[16] = {0};

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
bool IN_HS_INPUT = false;
// initial highlighted charecter in high score saving
char HS_CHAR_SELECTED = 'A';
char HS_ENTRY_NAME[10] = {'_', '_', '_', '_', '_', '_', '_', '_', '_', '_'};
bool HS_CONFIRM_SELECTED = false;
int HS_ENTRY_INDEX = 0;
// initial SCORE
uint32_t SCORE = 0;
// initial MOVES number
uint32_t MOVES = 0;

struct SAVE_ENTRY {
    char name[10];
    uint32_t score;
};

SAVE_ENTRY SAVE_DATA[7];

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

    if (read_save(SAVE_DATA, 0)){
        // loaded
    } else {
        for (int i = 0; i < 7; i++){
            strncpy(SAVE_DATA[i].name, "__________", 10);
            SAVE_DATA[i].score = 0;
        }
    }

    genRandomPiece(MAP);
    genRandomPiece(MAP);
}

void renderBackground(){
    // clear the screen -- screen is a reference to the frame buffer and can be used to draw all things with the 32blit
    screen.clear();
    screen.pen = Pen(0, 0, 0);
    screen.rectangle(Rect(0, 0, 320, 240));
}

void renderKeyboard(){
   char k[2];

   screen.alpha = 128;
   screen.pen = Pen(0, 255, 0);
   screen.rectangle(Rect(0, 120, 320, 120));
   screen.alpha = 255;

   screen.pen = Pen(255, 255, 255);
   
   screen.text(HS_ENTRY_NAME, font, Point(120, 120));

   for (int j = 0; j < 2; j++){
    for (int i = 0; i < 13; i++){
        k[0] = 'A' + i + 13 * j;
        screen.pen = HS_CHAR_SELECTED == k[0] ? Pen(0, 255, 0) : Pen(255, 255, 255);
        screen.text(k, font, Point(60 + 16 * i, 150 + 16 * j));
    }
   }
   screen.pen = HS_CONFIRM_SELECTED ? Pen(0, 255, 0) : Pen(255, 255, 255);
   screen.text("CONFIRM", font, Point(120, 180));
   
}

int compareScore(const void *a, const void *b) {
  
    SAVE_ENTRY *A = (SAVE_ENTRY *)a;
    SAVE_ENTRY *B = (SAVE_ENTRY *)b;
  
    return (B->score - A->score);
}


bool newRecord(){
    // if we hve free slot
    for (int i = 6; i >= 0; i--){
        if(SAVE_DATA[i].score == 0) {
            return true;
        }
    }

    // if no free slots
    for (int i = 6; i >= 0; i--){
        if(SAVE_DATA[i].score < SCORE){
            return true;
        }
    }

    return false;
}

void saveRecord(){
    int newScorePos = -1;

    // if we hve free slot
    for (int i = 6; i >= 0; i--){
        if(SAVE_DATA[i].score == 0) {
            newScorePos = i;
        }
    }

    // if no free slots
    if (newScorePos == -1){
        for (int i = 6; i >= 0; i--){
            if(SAVE_DATA[i].score < SCORE){
                newScorePos = i;
            }
        }
    }

    // put new record
    if (newScorePos >= 0){
        strncpy(SAVE_DATA[newScorePos].name, HS_ENTRY_NAME, 10);
        SAVE_DATA[newScorePos].score = SCORE;
        // sort record table
        qsort(SAVE_DATA, 7, sizeof(SAVE_ENTRY), compareScore);
        write_save(SAVE_DATA);
    }
}

#ifdef SHOW_FPS
    void draw_fps(uint32_t &ms_start, uint32_t &ms_end) {

        //Draw fps box
        screen.alpha = 255;
        screen.pen = Pen(0, 0, 0);
        //screen.rectangle(Rect(0, screen.bounds.h - 5, 20, 9));
        screen.pen = Pen(0, 255, 0);

        //Calculate frames per second (fps)
        float time_difference_in_sec = static_cast<float>(ms_end - ms_start) / 1000;
        if (time_difference_in_sec == 0) {
            time_difference_in_sec = 1;
        }
        int fps = static_cast<int>(1 / time_difference_in_sec);

        //Cap max shown fps
        if (fps > 999) {
            fps = 999;
        }

        //Draw fps counter
        std::string fps_string = std::to_string(fps);
        screen.text(fps_string, minimal_font, Rect(1, screen.bounds.h - 7, 10, 10));

        //Draw frame time boxes
        int block_size = 4;
        for (int i = 0; i < static_cast<int>(ms_end - ms_start); i++) {
            screen.pen = Pen(i * 5, 255 - (i * 5), 0);
            screen.rectangle(Rect(i * (block_size + 1) + 1 + 21, screen.bounds.h - block_size - 1, block_size, block_size));
        }
    }
#endif

///////////////////////////////////////////////////////////////////////////
//
// render(time)
//
// This function is called to perform rendering of the game. time is the 
// amount if milliseconds elapsed since the start of your game
//
void render(uint32_t time) {

    #ifdef SHOW_FPS
        uint32_t ms_start = now();
    #endif
    renderBackground();

    if (IN_MENU) {
        screen.stretch_blit(menuSurface, Rect(0, 0, SCREEN_WEIGHT, SCREEN_HEIGHT), Rect(0, 0, SCREEN_WEIGHT, SCREEN_HEIGHT));
        if(! IN_HS_INPUT){
            for (auto e : MENU_ENTRIES){
                screen.pen = e.active ? Pen(0, 255, 0) : Pen(255, 255, 255);
                screen.text(e.name, font, e.pos);
            }
        }
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
        screen.text("SCORE:" + std::to_string(SCORE), minimal_font, Point(245, 5));
        screen.text("MOVES:" + std::to_string(MOVES), minimal_font, Point(245, 25));
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

        for (int i = 0; i < 7; i++){
            screen.text(std::to_string(i + 1) + ".", font, Point(80, 85 + i * 20));
            screen.text(SAVE_DATA[i].name, font, Point(100, 85 + i * 20), TextAlign::left);
            screen.text(std::to_string(SAVE_DATA[i].score), font, Point(225, 85 + i * 20), TextAlign::center_right);
        }
    }

    if (IN_HS_INPUT){
        renderKeyboard();
    }

    #ifdef SHOW_FPS
        uint32_t ms_end = now();
        draw_fps(ms_start, ms_end);
    #endif
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

        if (HS_CHAR_SELECTED <= 'Z'){
            HS_CONFIRM_SELECTED = false;
        }
        if (HS_CHAR_SELECTED > 'Z'){
            HS_CONFIRM_SELECTED = true;
        }

        if (buttons.released & Button::A){
            if (HS_CONFIRM_SELECTED){
                saveRecord();
                SCORE = 0;
                strncpy(HS_ENTRY_NAME, "__________", 10);
                HS_CHAR_SELECTED = 'A';
                HS_ENTRY_INDEX = 0;
                IN_HS = false;
                IN_HS_INPUT = false;
                IN_MENU = true;                
            }
            HS_ENTRY_NAME[HS_ENTRY_INDEX] = HS_CHAR_SELECTED;
            HS_ENTRY_INDEX++;

            if(HS_ENTRY_INDEX > 9){
               HS_ENTRY_INDEX--;
            }
        }

        if (buttons.released & Button::B){
            HS_ENTRY_NAME[HS_ENTRY_INDEX] = '_';
            HS_ENTRY_INDEX--;
            if(HS_ENTRY_INDEX < 0){
               HS_ENTRY_INDEX++;
            }
        }

        if (buttons.released & Button::DPAD_UP){
            HS_CHAR_SELECTED -= 13;

            if (HS_CHAR_SELECTED < 'A'){
                HS_CHAR_SELECTED += 39;
            }
        }
        if (buttons.released & Button::DPAD_DOWN){
            HS_CHAR_SELECTED += 13;
            
            if (HS_CHAR_SELECTED >= 'h'){
                HS_CHAR_SELECTED -= 39;
            }
        }
        if (buttons.released & Button::DPAD_LEFT){
            if (HS_CONFIRM_SELECTED) return; 
            HS_CHAR_SELECTED--;
            if (HS_CHAR_SELECTED < 'A'){
                HS_CHAR_SELECTED = 'A';
            }
        }
        if (buttons.released & Button::DPAD_RIGHT){
            if (HS_CONFIRM_SELECTED) return;
            HS_CHAR_SELECTED++;
            if (HS_CHAR_SELECTED > 'Z'){
                HS_CHAR_SELECTED = 'Z';
            }
        }
        return;
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

    if (IN_GAME) {
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
        if (GAME_OVER) {
            IN_GAME = false;
            if (newRecord()){
                IN_HS = false;
                IN_MENU = true;
                IN_HS_INPUT = true;
            } else {
                SCORE = 0;
            }
            memset(MAP, 0, sizeof(MAP));
            MOVES = 0;
            genRandomPiece(MAP);
            genRandomPiece(MAP);
        }
    }
}