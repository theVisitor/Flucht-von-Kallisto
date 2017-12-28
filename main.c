#include <stdio.h>
#include <stdlib.h>
//#include <string.h>
//#include <stdint.h>
//#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <time.h>

#include "declarations.h"

/**
 * Function for displaying the main menu
 * and handling input
 */
int mainMenu(SDL_Renderer *renderer, SDL_Window *window, SDL_Texture *background) {
    SDL_RenderCopy(renderer, background, NULL, NULL);

    ///try loading campaign - let's see if there is something to continue
    int map, difficulty, score;
    int cLoadFail = loadCampaign(&map, &difficulty, &score);

    TTF_Font *roboto = TTF_OpenFont("resources/fonts/RobotoMono-Medium.ttf", 70);
    SDL_Color white = {255, 255, 255};

    SDL_Surface *text_surf = TTF_RenderText_Blended_Wrapped(roboto, "Flucht von\n  Kallisto", white, 420);
    SDL_Texture *text_texture = SDL_CreateTextureFromSurface(renderer, text_surf);
    SDL_Rect titleRec = {.x = 690, .y = 53, .w = text_surf->w, .h = text_surf->h};
    SDL_FreeSurface(text_surf);
    SDL_RenderCopy(renderer, text_texture, NULL, &titleRec);
    SDL_DestroyTexture(text_texture);

    TTF_CloseFont(roboto);
    roboto = TTF_OpenFont("resources/fonts/RobotoMono-Medium.ttf", 50);

    text_surf = TTF_RenderText_Blended_Wrapped(roboto, "Weiterspielen", white, 420);
    text_texture = SDL_CreateTextureFromSurface(renderer, text_surf);
    SDL_Rect contRec = {.x = 720, .y = 350, .w = text_surf->w, .h = text_surf->h};
    SDL_FreeSurface(text_surf);
    if (!cLoadFail) {
        SDL_RenderCopy(renderer, text_texture, NULL, &contRec);
    }
    SDL_DestroyTexture(text_texture);

    text_surf = TTF_RenderText_Blended_Wrapped(roboto, "  Neues Spiel", white, 420);
    text_texture = SDL_CreateTextureFromSurface(renderer, text_surf);
    SDL_Rect newRec = {.x = 720, .y = 430, .w = text_surf->w, .h = text_surf->h};
    SDL_FreeSurface(text_surf);
    SDL_RenderCopy(renderer, text_texture, NULL, &newRec);
    SDL_DestroyTexture(text_texture);

    text_surf = TTF_RenderText_Blended_Wrapped(roboto, "   Highscores", white, 420);
    text_texture = SDL_CreateTextureFromSurface(renderer, text_surf);
    SDL_Rect highRec = {.x = 720, .y = 510, .w = text_surf->w, .h = text_surf->h};
    SDL_FreeSurface(text_surf);
    SDL_RenderCopy(renderer, text_texture, NULL, &highRec);
    SDL_DestroyTexture(text_texture);
    TTF_CloseFont(roboto);

    SDL_RenderPresent(renderer);

    SDL_Event event;
    SDL_Point click;
    while (1) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)) {
                return -1;
            } else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                click.x = event.button.x; click.y = event.button.y;
                if (SDL_PointInRect(&click, &fullscreenRec)) {
                    if (SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN) {
                        SDL_SetWindowFullscreen(window, 0);
                    } else {
                        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
                    }
                    return 0;
                } else if (!cLoadFail && SDL_PointInRect(&click, &contRec)) {
                    return requestPlay(renderer, window, background, map, difficulty, score);
                } else if (SDL_PointInRect(&click, &newRec)) {
                    return newCampaign(renderer, window, background);
                } else if (SDL_PointInRect(&click, &highRec)) {
                    return showHighscores(renderer, window, background);
                }
            }
        }
    }
}

/**
 * Function for showing the table of highscores
 */
int showHighscores(SDL_Renderer *renderer, SDL_Window *window, SDL_Texture *background) {
    SDL_RenderCopy(renderer, background, NULL, NULL);
    char scorearray[17][SCORESIZE];
    readScore(scorearray);

    TTF_Font *roboto = TTF_OpenFont("resources/fonts/RobotoMono-Medium.ttf", 24);
    SDL_Color white = {255, 255, 255};

    SDL_Surface *text_surf = TTF_RenderText_Blended(roboto, "Einfach                   Normal                    Schwer                   ", white);
    SDL_Texture *text_texture = SDL_CreateTextureFromSurface(renderer, text_surf);
    SDL_Rect titleRec = {.x = 150, .y = 300, .w = text_surf->w, .h = text_surf->h};
    printf("%d\n", text_surf->w);
    SDL_FreeSurface(text_surf);
    SDL_RenderCopy(renderer, text_texture, NULL, &titleRec);
    SDL_DestroyTexture(text_texture);

    char name[15] = {0};

    SDL_Rect scoreRec;
    for (int diff = 0; diff < 3; diff++) {
        for (int scorenum = 0; scorenum < 5; scorenum++) {
            text_surf = TTF_RenderText_Blended(roboto, scorearray[5 * diff + scorenum], white);
            text_texture = SDL_CreateTextureFromSurface(renderer, text_surf);
            scoreRec.x = 150 + 364 * diff;
            scoreRec.y = 350 + 40 * scorenum;
            scoreRec.w = text_surf->w;
            scoreRec.h = text_surf->h;
            SDL_FreeSurface(text_surf);
            SDL_RenderCopy(renderer, text_texture, NULL, &scoreRec);
            SDL_DestroyTexture(text_texture);
        }
    }
    TTF_CloseFont(roboto);
    SDL_RenderPresent(renderer);

    SDL_Event event;
    SDL_Point click;
    while (1) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)) {
                return -1;
            } else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                click.x = event.button.x; click.y = event.button.y;
                if (SDL_PointInRect(&click, &fullscreenRec)) {
                    if (SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN) {
                        SDL_SetWindowFullscreen(window, 0);
                    } else {
                        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
                    }
                    return showHighscores(renderer, window, background);
                } else {
                    return 0;
                }
            }
        }
    }
}

/**
 * Main function
 * Responsible for initialising SDL&co and
 * closing everything afterwards
 */
int main(int argc, char *args[]) {
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("SDL_Init failed!\n");
        SDL_Quit();
        return -1;
    }
    int flags = MIX_INIT_MP3;
    Mix_Init(flags);
	///initialize audio playback
	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
        printf("%s", Mix_GetError());

    Mix_AllocateChannels(16);

    IMG_Init(IMG_INIT_PNG);
    TTF_Init();

    SDL_Window *window = SDL_CreateWindow("Flucht von Kallisto v1.1.2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, 0);
    if (window == NULL) {
        return -1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
    if (renderer == NULL) {
        return -1;
    }

    createdWithSdl(renderer);
    createdWithGimpBlender(renderer);
    SDL_Texture *background = IMG_LoadTexture(renderer, "resources/textures/background.png");

    srand(time(NULL));

    ///main loop
    int quit = 0;
    while (!quit) {
        quit = mainMenu(renderer, window, background);
    }

    ///clean up
    SDL_DestroyTexture(background);
    IMG_Quit();
    TTF_Quit();
    Mix_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}


