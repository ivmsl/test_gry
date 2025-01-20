#include <stdlib.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include "main.h"
#include "utils.h"
#define TTF_PATH "./res/fonts/Roboto-Medium.ttf"

TTF_Font* font;
SDL_Texture* title;
SDL_Texture* play_option;
SDL_Texture* quit_option;

static SDL_bool playSelected = SDL_TRUE;
SDL_Rect playRect = {0, 0, 0, 0};
SDL_Rect quitRect = {0, 0, 0, 0};

//Create function that will inicialise the menu — load TTF file and change global state to menu
int initMenu(SDL_Renderer* renderer, char* ttext) {
    font = TTF_OpenFont(TTF_PATH, 24);
    if (!font) {
        printf("Error loading font: %s\n", TTF_GetError());
        return 1;
    }
    global_state = MENU;

    title = getTextureFromWords(renderer, font, ttext, (SDL_Color) {255, 255, 255, 255});
    play_option = getTextureFromWords(renderer, font, "Play",(SDL_Color) {255, 255, 255, 255});
    quit_option = getTextureFromWords(renderer, font, "Quit", (SDL_Color) {255, 255, 255, 255});

    if (!title || !play_option || !quit_option) {
        return 1;
    }


    SDL_QueryTexture(play_option, NULL, NULL, &playRect.w, &playRect.h);
    playRect.x = (800 - playRect.w) / 2;
    playRect.y = 600 * 3 / 4 - playRect.h;

    playRect.x -= 4;
    playRect.y -= 4;
    playRect.w += 8;
    playRect.h += 8;
    
    SDL_QueryTexture(quit_option, NULL, NULL, &quitRect.w, &quitRect.h);
    quitRect.x = (800 - quitRect.w) / 2;
    quitRect.y = 600 * 3 / 4 + playRect.h;

    quitRect.x -= 4;
    quitRect.y -= 4;
    quitRect.w += 8;
    quitRect.h += 8;

    return 0;
}


//Function that will render the menu on the screen. Title should be in the center-up of the screen. Options should be in the center-down one on top of the other. 
void renderMenu(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    int windowWidth, windowHeight;
    SDL_GetRendererOutputSize(renderer, &windowWidth, &windowHeight);

    int titleWidth, titleHeight;
    SDL_QueryTexture(title, NULL, NULL, &titleWidth, &titleHeight);
    SDL_Rect titleRectText = { (windowWidth - titleWidth) / 2, windowHeight / 4 - titleHeight / 2, titleWidth, titleHeight };
    SDL_RenderCopy(renderer, title, NULL, &titleRectText);

    int playWidth, playHeight;
    SDL_QueryTexture(play_option, NULL, NULL, &playWidth, &playHeight);
    SDL_Rect playRectText = { (windowWidth - playWidth) / 2, windowHeight * 3 / 4 - playHeight, playWidth, playHeight };
    SDL_RenderCopy(renderer, play_option, NULL, &playRectText);

    int quitWidth, quitHeight;
    SDL_QueryTexture(quit_option, NULL, NULL, &quitWidth, &quitHeight);
    SDL_Rect quitRectText = { (windowWidth - quitWidth) / 2, windowHeight * 3 / 4 + playHeight, quitWidth, quitHeight };
    SDL_RenderCopy(renderer, quit_option, NULL, &quitRectText);

    if (playSelected) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderDrawRect(renderer, &playRect);
    }
    else {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderDrawRect(renderer, &quitRect);
    }   


    SDL_RenderPresent(renderer);
}

//Let's add a function that will handle menu events: 
//draw a rectangle around selected menu button, 
//flip "rectangle" between two buttons on key press UP and DOWN, and 
//change global state to PLAY or QUIT if key pressed is ENTER
//That function will be called in the main loop, only key and renderer will be passed.



void handleMenuEvent(SDL_KeyboardEvent* event, SDL_Renderer* renderer) {

    switch (event->keysym.sym) {
        case SDLK_UP:
            playSelected = SDL_TRUE;
            break;
        case SDLK_DOWN:
            playSelected = SDL_FALSE;
            break;
        case SDLK_RETURN:
            global_state = playSelected ? LOAD_GAME : QUIT;
            break;
    }

}

int deInitMenu() {
    if (title) {
        SDL_DestroyTexture(title);
        title = NULL;
    }
    if (play_option) {
        SDL_DestroyTexture(play_option);
        play_option = NULL;
    }
    if (quit_option) {
        SDL_DestroyTexture(quit_option);
        quit_option = NULL;
    }
    if (font) {
        TTF_CloseFont(font);
        font = NULL;
    }
    return 0;
}
