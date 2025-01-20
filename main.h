#ifndef g_game_h_
#define g_game_h_

enum gameState {
    PRELOAD,
    MENU,
    GAME,
    GAMEOVER,
    QUIT
};

extern enum gameState global_state;

#endif
