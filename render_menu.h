#ifndef g_render_menu_h_
#define g_render_menu_h_
#include <SDL.h>


extern int initMenu(SDL_Renderer* renderer);
extern void renderMenu(SDL_Renderer* renderer);

extern void handleMenuEvent(SDL_KeyboardEvent* event, SDL_Renderer* renderer);

extern void deInitMenu(void);

#endif
