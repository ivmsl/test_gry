#include <SDL.h>
#include <SDL_ttf.h>
#include "main.h"
#include "utils.h"
#include "render_game.h"
#define TTF_PATH "./res/fonts/Roboto-Medium.ttf"

uint score = 0;
player plr;
prostokat pr_arr[10];
char num_present = 0;
TTF_Font* font;
SDL_Rect scoreRect;
SDL_Texture* scoreTexture;
Uint32 ticks;
char keyLock = 0;
int left_wall = 0;
int bottom = 0;

void updateCounterTexture(SDL_Renderer* renderer) {
    if (!renderer || !font) {
        printf("Error in updateCounterTexture: renderer, font or scoreTexture is NULL\n");
        return;
    }

    if (scoreTexture) SDL_DestroyTexture(scoreTexture);
    
    char text[128];
    sprintf(text, "Score: %d", score);

    scoreTexture = getTextureFromWords(renderer, font, text, (SDL_Color) { 255, 255, 255, 255 });
    if (!scoreTexture) {
        printf("Error creating scoreTexture in updateCounterTexture: %s\n", SDL_GetError());
    }
}


int initGame(SDL_Renderer* renderer) {
    score = 0;
    font = TTF_OpenFont(TTF_PATH, 24);
    if (!font) {
        printf("Error loading font: %s\n", TTF_GetError());
        return 1;
    }

    //create a player — triangle on the bottom part of the screen, like 10 pixels from the bottom of the screen
    int windowWidth, windowHeight;
    SDL_GetRendererOutputSize(renderer, &windowWidth, &windowHeight);
    plr.top.x = windowWidth / 2;
    plr.top.y = windowHeight - 45;
    plr.left.x = plr.top.x - 15;
    plr.left.y = windowHeight - 10;
    plr.right.x = plr.top.x + 15;
    plr.right.y = windowHeight - 10;

    //place scoreRect on the upper right corner of the screen
    SDL_GetRendererOutputSize(renderer, &scoreRect.w, &scoreRect.h);
    scoreRect.x = scoreRect.w - 150;
    scoreRect.y = 0;
    scoreRect.w = 150;
    scoreRect.h = 50;

    for (int i = 0; i < 10; i++) {
        pr_arr[i].present = 0;
    }

    return 0;
}

//Create a render game function -> function that will render for now only the player triangle
void renderGame(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderDrawLine(renderer, plr.top.x, plr.top.y, plr.left.x, plr.left.y);
    SDL_RenderDrawLine(renderer, plr.left.x, plr.left.y, plr.right.x, plr.right.y);
    SDL_RenderDrawLine(renderer, plr.right.x, plr.right.y, plr.top.x, plr.top.y);

    for (int i = 0; i < 10; i++) {
        if (pr_arr[i].present > 0) {
            SDL_SetRenderDrawColor(renderer, pr_arr[i].color.r, pr_arr[i].color.g, pr_arr[i].color.b, pr_arr[i].color.a);
            SDL_RenderFillRect(renderer, &pr_arr[i].rect);
        }
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &scoreRect);
    SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreRect);

    SDL_RenderPresent(renderer);

}

void spawnSquare(SDL_Renderer* renderer) {
    int w, h;
    SDL_GetRendererOutputSize(renderer, &w, &h);
    
    if (num_present < 5) {
        for (int i = 0; i < 10; i++) {
            if (pr_arr[i].present == 0) {
                pr_arr[i].present = 1;
                pr_arr[i].rect.x = rand() % w;
                pr_arr[i].rect.y = 0;
                pr_arr[i].rect.w = rand() % 50 + 50;
                pr_arr[i].rect.h = rand() % 50 + 50;
                pr_arr[i].speed = rand() % 5 + 1;
                pr_arr[i].color.r = rand() % 255;
                pr_arr[i].color.g = rand() % 255;
                pr_arr[i].color.b = rand() % 255;
                pr_arr[i].color.a = 255;
                num_present++;
                break;
            }
        }
    }
}

void updateSquarePosition(SDL_Renderer* renderer) {
    //move squares down the screen at a constant speed (speed)
    int windowWidth, windowHeight;
    SDL_GetRendererOutputSize(renderer, &windowWidth, &windowHeight);

    for (int i = 0; i < 10; i++) {
        if (pr_arr[i].present) {
            pr_arr[i].rect.y += pr_arr[i].speed;
     
        
            if (pr_arr[i].rect.y >= windowHeight) {
                pr_arr[i].present = 0;
                num_present--;
                score++;
                updateCounterTexture(renderer);
        }
        }

    }
}

//Make a function that checks if our triangle lines are colliding with any of the squares. Use SDL_IntersectRectAndLine
void handlePlayerCollision(SDL_Renderer* renderer) {
    for (int i = 0; i < 10; i++) {
        if (!pr_arr[i].present) continue;
        if (SDL_IntersectRectAndLine(&pr_arr[i].rect, &plr.left.x, &plr.left.y, &plr.top.x, &plr.top.y) == SDL_TRUE ||
                SDL_IntersectRectAndLine(&pr_arr[i].rect, &plr.right.x, &plr.right.y, &plr.top.x, &plr.top.y) == SDL_TRUE) 
        {
                    printf("Collision detected!\n");
                    global_state = GAMEOVER;
        }

    }
}


void gameRoutine(SDL_Renderer* renderer) {
    //if (SDL_GetTicks() - ticks > 300) {
    //    ticks = SDL_GetTicks();
        spawnSquare(renderer);
        updateSquarePosition(renderer);
        handlePlayerCollision(renderer);    
    //}
    
    SDL_GetRendererOutputSize(renderer, &left_wall, &bottom);
}

void updatePlayerPosition(SDL_KeyboardEvent* event) {
    Uint32 currentTime = SDL_GetTicks();

    if (event->type == SDL_KEYDOWN && keyLock == 0) {
        ticks = currentTime;
        keyLock = 1;
    }
    
    int acceleration = (currentTime - ticks) / 100;

    if (event->type == SDL_KEYUP) {
        keyLock = 0;
        acceleration = 0;
        return;
    }
    
    if (event->keysym.sym == SDLK_LEFT) {
        if ((plr.left.x - 5 - acceleration) <= 0) {
            plr.left.x = 0;
            plr.right.x = 30;
            plr.top.x = 15;
        }
        else {
            plr.left.x -= 5 + acceleration;
            plr.right.x -= 5 + acceleration;
            plr.top.x -= 5 + acceleration;
        }
        
    }
    if (event->keysym.sym == SDLK_RIGHT) {
        if (plr.right.x + 5 + acceleration >= left_wall) {
            plr.left.x = left_wall - 30;
            plr.right.x = left_wall;
            plr.top.x = left_wall - 15;
        }
        else { 
            plr.left.x += 2 + acceleration;
            plr.right.x += 2 + acceleration;
            plr.top.x += 2 + acceleration;
        }
        
    }
}

//Deinits and destroys allocated memory
uint gameDeinit() {
    if (scoreTexture != NULL) {
        SDL_DestroyTexture(scoreTexture);
        scoreTexture = NULL;
    }
    if (font != NULL) {
        TTF_CloseFont(font);
        font = NULL;
    }

    for (int i = 0; i < 10; i++) {
        if (pr_arr[i].present) {
            pr_arr[i].present = 0;
            num_present--;
        }
        num_present = 0;
    return score;
    }
}

