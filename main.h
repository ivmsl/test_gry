#ifndef g_game_h_
#define g_game_h_

enum gameState {
    PRELOAD,
    MENU,
    LOAD_GAME,
    GAME,
    GAMEOVER,
    GAMEOVER_SCREEN,
    QUIT
};

extern enum gameState global_state;

#endif
