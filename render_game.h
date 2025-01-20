#ifndef g_ren_game_h_
#define g_ren_game_h_

#include <SDL.h>
#include "main.h"
#include "util.h"

typedef struct player {
    SDL_Point top;
    SDL_Point left;
    SDL_Point right;
} player;

typedef struct prostokat {
    int present;
    SDL_Rect rect;
    SDL_Color color;
    int speed;
} prostokat;


extern int initGame(SDL_Renderer* renderer);
extern void renderGame(SDL_Renderer* renderer);

extern void gameRoutine( SDL_Renderer* renderer);

extern void updatePlayerPosition(SDL_KeyboardEvent* event);

extern uint gameDeinit(void);

#endif
