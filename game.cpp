#include "game.hpp"
#include "assets.hpp"
// #include "32blit.hpp"
using namespace blit;

#define SCREEN_HEIGHT 240
#define SCREEN_WEIGHT 320
#define SQURE_SIZE 58


int MAP[4][4] = {
    {2, 4, 2, 8,},
	{32, 2, 16, 2},
	{2, 1024, 2, 0},
	{512, 2, 32, 2048},	
};

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
}

void renderBackground(){
    // clear the screen -- screen is a reference to the frame buffer and can be used to draw all things with the 32blit
    screen.clear();
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

    for (int x =0; x <4; x++) 
    {
        for (int y = 0; y < 4; y++)
        {
            if (MAP[x][y] == 0){ screen.sprite(n0, Point(42 + x * SQURE_SIZE, 2 + y * SQURE_SIZE));}
            if (MAP[x][y] == 2){ screen.sprite(n2, Point(42 + x * SQURE_SIZE, 2 + y * SQURE_SIZE));}
            if (MAP[x][y] == 4){screen.sprite(n4, Point(42 + x * SQURE_SIZE, 2 + y * SQURE_SIZE));}
            if (MAP[x][y] == 8){screen.sprite(n8, Point(42 + x * SQURE_SIZE, 2 + y * SQURE_SIZE));}
            if (MAP[x][y] == 16){screen.sprite(n16, Point(42 + x * SQURE_SIZE, 2 + y * SQURE_SIZE));}
            if (MAP[x][y] == 32){screen.sprite(n32, Point(42 + x * SQURE_SIZE, 2 + y * SQURE_SIZE));}
            if (MAP[x][y] == 64){screen.sprite(n64, Point(42 + x * SQURE_SIZE, 2 + y * SQURE_SIZE));}
            if (MAP[x][y] == 128){screen.sprite(n128, Point(42 + x *SQURE_SIZE, 2 + y * SQURE_SIZE));}
            if (MAP[x][y] == 256){screen.sprite(n256, Point(42 + x * SQURE_SIZE, 2 + y * SQURE_SIZE));}
            if (MAP[x][y] == 512){screen.sprite(n512, Point(42 + x * SQURE_SIZE, 2 + y * SQURE_SIZE));}
            if (MAP[x][y] == 1024){screen.sprite(n1024, Point(42 + x * SQURE_SIZE, 2 + y * SQURE_SIZE));}
            if (MAP[x][y] == 2048){screen.sprite(n2048, Point(42 + x * SQURE_SIZE, 2 + y * SQURE_SIZE));}
            if (MAP[x][y] == 4096){screen.sprite(n4096, Point(42 + x * SQURE_SIZE, 2 + y * SQURE_SIZE));} 
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
}