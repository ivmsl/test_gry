#include <SDL.h>
#include <SDL_ttf.h>
#include "utils.h"
#include "render_game.h"
#include "render_menu.h"
#include "main.h"
#include <time.h>


/************************************************* 
*   Uninvited triangle in the city of sqares 
*   
*   This project is a take on a primitive game, 
*   in which player controls a small triangle trying
*   to dodge squares falling from the sky.
*
*   The goal is to dodge as many squares as it is possible.
*   If player collides with a square, the game ends.
*
*   This is a project created as a part of a university course.
*   (SiNWO, UŚ). It uses Codeium and TabNine AI agents 
*   to test their capabilities (as a part of the university task).
*
*************************************************/


enum gameState global_state;

// Initialize SDL modules, create main window and renderer, set up the game loop and resource deallocation on quit 
int main(void) {

    srand(time(0));
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Game Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    if (TTF_Init() < 0) {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    
    if (initMenu(renderer, "Uninvited triangle in the city of squares") != 0) {
        printf("Error initializing menu\n");
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;       
    }

    uint score = 0;
    int quit = 0;
    SDL_Event e;
    while (!quit) {

        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(renderer);


        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
            else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_ESCAPE) {
                    quit = 1;
                }
                else if (global_state == MENU) {
                    handleMenuEvent(&e.key, renderer);
                }
                else if (global_state == GAME && (e.key.keysym.sym == SDLK_LEFT || e.key.keysym.sym == SDLK_RIGHT)) {
                    
                    updatePlayerPosition(&e.key);
                }
            }
            else if (e.type == SDL_KEYUP) {
             if (global_state == GAME && (e.key.keysym.sym == SDLK_LEFT || e.key.keysym.sym == SDLK_RIGHT)) {
                    
                    updatePlayerPosition(&e.key);
                }
            }
        }

        switch (global_state) {
            case MENU:
                renderMenu(renderer);
                break;
            case LOAD_GAME:
                deInitMenu();
                if (initGame(renderer) != 0) {
                    printf("Error initializing game\n");
                };
                global_state = GAME;
                break;
            case GAME:
                gameRoutine(renderer);
                renderGame(renderer);
                break;
            case GAMEOVER:
                score = gameDeinit();
                char buf[128];
                sprintf(buf, "Your score: %d. Wanna play again?", score);
                initMenu(renderer, buf);
                global_state = MENU;
                break;
            
            case QUIT:
                quit = 1;
                break;
            default:
                break;
        }


//        SDL_RenderPresent(renderer);
        
        SDL_Delay(10);
    }

    deInitMenu();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
