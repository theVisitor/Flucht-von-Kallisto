#include <stdio.h>
#include <stdlib.h>
//#include <string.h>
//#include <stdint.h>
//#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <time.h>

#include "declarations.h"

/**
 * Function that shows a window where the user
 * can type their name
 */
int getName(SDL_Renderer *renderer, char name[]) {
    ///clearing the array - just in case
    for (int i = 1; i < 15; i++) {
        name[i] = 0;
    }
    name[0] = ' ';

    TTF_Font *roboto = TTF_OpenFont("resources/fonts/RobotoMono-Medium.ttf", 24);
    SDL_Color white = {255, 255, 255};

    SDL_Surface *text_surf = TTF_RenderText_Blended(roboto, "Bitte Namen eingeben", white);
    SDL_Texture *text_texture = SDL_CreateTextureFromSurface(renderer, text_surf);
    SDL_Rect dRect = {.x = 200, .y = 220, .w = text_surf->w, .h = text_surf->h};
    SDL_FreeSurface(text_surf);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, text_texture, NULL, &dRect);
    SDL_RenderPresent(renderer);

    dRect.y = 320;

    SDL_Rect clearing = {.x = 0, .y = 300, .w = 1280, .h = 420};
    int position = 0;
    int running = 1;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.scancode) {
                case SDL_SCANCODE_A: setLetter(&position, name, 'A'); break;
                case SDL_SCANCODE_B: setLetter(&position, name, 'B'); break;
                case SDL_SCANCODE_C: setLetter(&position, name, 'C'); break;
                case SDL_SCANCODE_D: setLetter(&position, name, 'D'); break;
                case SDL_SCANCODE_E: setLetter(&position, name, 'E'); break;
                case SDL_SCANCODE_F: setLetter(&position, name, 'F'); break;
                case SDL_SCANCODE_G: setLetter(&position, name, 'G'); break;
                case SDL_SCANCODE_H: setLetter(&position, name, 'H'); break;
                case SDL_SCANCODE_I: setLetter(&position, name, 'I'); break;
                case SDL_SCANCODE_J: setLetter(&position, name, 'J'); break;
                case SDL_SCANCODE_K: setLetter(&position, name, 'K'); break;
                case SDL_SCANCODE_L: setLetter(&position, name, 'L'); break;
                case SDL_SCANCODE_M: setLetter(&position, name, 'M'); break;
                case SDL_SCANCODE_N: setLetter(&position, name, 'N'); break;
                case SDL_SCANCODE_O: setLetter(&position, name, 'O'); break;
                case SDL_SCANCODE_P: setLetter(&position, name, 'P'); break;
                case SDL_SCANCODE_Q: setLetter(&position, name, 'Q'); break;
                case SDL_SCANCODE_R: setLetter(&position, name, 'R'); break;
                case SDL_SCANCODE_S: setLetter(&position, name, 'S'); break;
                case SDL_SCANCODE_T: setLetter(&position, name, 'T'); break;
                case SDL_SCANCODE_U: setLetter(&position, name, 'U'); break;
                case SDL_SCANCODE_V: setLetter(&position, name, 'V'); break;
                case SDL_SCANCODE_W: setLetter(&position, name, 'W'); break;
                case SDL_SCANCODE_X: setLetter(&position, name, 'X'); break;
                case SDL_SCANCODE_Y: setLetter(&position, name, 'Y'); break;
                case SDL_SCANCODE_Z: setLetter(&position, name, 'Z'); break;
                case SDL_SCANCODE_0: setLetter(&position, name, '0'); break;
                case SDL_SCANCODE_1: setLetter(&position, name, '1'); break;
                case SDL_SCANCODE_2: setLetter(&position, name, '2'); break;
                case SDL_SCANCODE_3: setLetter(&position, name, '3'); break;
                case SDL_SCANCODE_4: setLetter(&position, name, '4'); break;
                case SDL_SCANCODE_5: setLetter(&position, name, '5'); break;
                case SDL_SCANCODE_6: setLetter(&position, name, '6'); break;
                case SDL_SCANCODE_7: setLetter(&position, name, '7'); break;
                case SDL_SCANCODE_8: setLetter(&position, name, '8'); break;
                case SDL_SCANCODE_9: setLetter(&position, name, '9'); break;
                case SDL_SCANCODE_SPACE: setLetter(&position, name, ' '); break;
                case SDL_SCANCODE_BACKSPACE: if (position) {position--; if (position) {name[position] = '\0';} else {name[0] = ' ';}} break;
                case SDL_SCANCODE_RETURN: if (position) {running = 0;} break;
                default: break;
                }
            }
        }
        ///render Name
        text_surf = TTF_RenderText_Blended(roboto, name, white);
        text_texture = SDL_CreateTextureFromSurface(renderer, text_surf);
        dRect.w = text_surf->w;
        SDL_FreeSurface(text_surf);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(renderer, &clearing);
        SDL_RenderCopy(renderer, text_texture, NULL, &dRect);
        SDL_DestroyTexture(text_texture);
        SDL_RenderPresent(renderer);
    }
    SDL_DestroyTexture(text_texture);
    TTF_CloseFont(roboto);
    return 0;
}

/**
 * Helper function for writing in the name-array
 * (used for getName only)
 */
void setLetter(int *position, char name[], char letter) {
    if (*position < 14) {
        name[*position] = letter;
        (*position)++;
    }
}

/**
 * Function for creating and starting a new campaign
 */
int newCampaign(SDL_Renderer *renderer, SDL_Window *window, SDL_Texture *background) {
    ///ask for difficulty
    SDL_RenderCopy(renderer, background, NULL, NULL);

    TTF_Font *roboto = TTF_OpenFont("resources/fonts/RobotoMono-Medium.ttf", 50);
    SDL_Color white = {255, 255, 255};

    SDL_Surface *text_surf = TTF_RenderText_Blended(roboto, "Einfach", white);
    SDL_Texture *text_texture = SDL_CreateTextureFromSurface(renderer, text_surf);
    SDL_Rect easyRec = {.x = 900, .y = 350, .w = text_surf->w, .h = text_surf->h};
    SDL_FreeSurface(text_surf);
    SDL_RenderCopy(renderer, text_texture, NULL, &easyRec);
    SDL_DestroyTexture(text_texture);

    text_surf = TTF_RenderText_Blended(roboto, " Normal", white);
    text_texture = SDL_CreateTextureFromSurface(renderer, text_surf);
    SDL_Rect normalRec = {.x = 900, .y = 430, .w = text_surf->w, .h = text_surf->h};
    SDL_FreeSurface(text_surf);
    SDL_RenderCopy(renderer, text_texture, NULL, &normalRec);
    SDL_DestroyTexture(text_texture);

    text_surf = TTF_RenderText_Blended(roboto, " Schwer", white);
    text_texture = SDL_CreateTextureFromSurface(renderer, text_surf);
    SDL_Rect hardRec = {.x = 900, .y = 510, .w = text_surf->w, .h = text_surf->h};
    SDL_FreeSurface(text_surf);
    SDL_RenderCopy(renderer, text_texture, NULL, &hardRec);
    SDL_DestroyTexture(text_texture);
    TTF_CloseFont(roboto);

    SDL_RenderPresent(renderer);

    int difficulty = 0;
    SDL_Event event;
    SDL_Point click;
    while (!difficulty) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)) {
                difficulty = -1;
            } else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                click.x = event.button.x; click.y = event.button.y;
                if (SDL_PointInRect(&click, &fullscreenRec)) {
                    if (SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN) {
                        SDL_SetWindowFullscreen(window, 0);
                    } else {
                        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
                    }
                    return newCampaign(renderer, window, background);
                } else if (SDL_PointInRect(&click, &easyRec)) {
                    difficulty = 1;
                } else if (SDL_PointInRect(&click, &normalRec)) {
                    difficulty = 2;
                } else if (SDL_PointInRect(&click, &hardRec)) {
                    difficulty = 3;
                }
            }
        }
    }
    if (difficulty == -1) {
        return -1;
    }
    ///save as campaign, start playing
    saveCampaign(1, difficulty, 0);
    if (playIntro(renderer, window)) {
        return -1;
    }
    return requestPlay(renderer, window, background, 1, difficulty, 0);
}

/**
 * Function for displaying how many settlers
 * survived in this map
 */
int showWinScreen(SDL_Renderer *renderer, int score) {
    TTF_Font *roboto = TTF_OpenFont("resources/fonts/RobotoMono-Medium.ttf", 50);
    SDL_Color white = {255, 255, 255};
    char finalsaying[100] = {0};
    sprintf(finalsaying, "Du konntest %d Siedler retten.", score);
    SDL_Surface *text_surf = TTF_RenderText_Blended(roboto, finalsaying, white);
    SDL_Texture *text_texture = SDL_CreateTextureFromSurface(renderer, text_surf);
    SDL_Rect dRect = {.x = 200, .y = 320, .w = text_surf->w, .h = text_surf->h};
    SDL_FreeSurface(text_surf);
    TTF_CloseFont(roboto);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, text_texture, NULL, &dRect);
    SDL_DestroyTexture(text_texture);
    SDL_RenderPresent(renderer);

    SDL_Event event;
    SDL_Delay(750);
    while (SDL_PollEvent(&event)) {}
    while (1) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)) {
                return -1;
            } else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                return 0;
            }
        }
    }
}

/**
 * Function for displaying that no settlers
 * survived in this map
 */
int showLostScreen(SDL_Renderer *renderer) {
    TTF_Font *roboto = TTF_OpenFont("resources/fonts/RobotoMono-Medium.ttf", 50);
    SDL_Color white = {255, 255, 255};
    SDL_Surface *text_surf = TTF_RenderText_Blended(roboto, "Alle Siedler sind gestorben.", white);
    SDL_Texture *text_texture = SDL_CreateTextureFromSurface(renderer, text_surf);
    SDL_Rect dRect = {.x = 200, .y = 320, .w = text_surf->w, .h = text_surf->h};
    SDL_FreeSurface(text_surf);
    TTF_CloseFont(roboto);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, text_texture, NULL, &dRect);
    SDL_DestroyTexture(text_texture);
    SDL_RenderPresent(renderer);

    SDL_Event event;
    SDL_Delay(750);
    while (SDL_PollEvent(&event)) {}
    while (1) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)) {
                return -1;
            } else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                return 0;
            }
        }
    }
}

/**
 * Function for showing the pre-play screen
 * Starts the active map if user wants to play
 */
int requestPlay(SDL_Renderer *renderer, SDL_Window *window, SDL_Texture *background, int map, int difficulty, int score) {
    SDL_RenderCopy(renderer, background, NULL, NULL);

    TTF_Font *roboto = TTF_OpenFont("resources/fonts/RobotoMono-Medium.ttf", 70);
    SDL_Color white = {255, 255, 255};

    char mapname[10] = {0};
    sprintf(mapname, "Level %d", map);

    SDL_Surface *text_surf = TTF_RenderText_Blended(roboto, mapname, white);
    SDL_Texture *text_texture = SDL_CreateTextureFromSurface(renderer, text_surf);
    SDL_Rect titleRec = {.x = 816, .y = 100, .w = text_surf->w, .h = text_surf->h};
    SDL_FreeSurface(text_surf);
    SDL_RenderCopy(renderer, text_texture, NULL, &titleRec);
    SDL_DestroyTexture(text_texture);

    TTF_CloseFont(roboto);
    roboto = TTF_OpenFont("resources/fonts/RobotoMono-Medium.ttf", 50);

    text_surf = TTF_RenderText_Blended(roboto, "Start", white);
    text_texture = SDL_CreateTextureFromSurface(renderer, text_surf);
    SDL_Rect startRec = {.x = 960, .y = 430, .w = text_surf->w, .h = text_surf->h};
    SDL_FreeSurface(text_surf);
    SDL_RenderCopy(renderer, text_texture, NULL, &startRec);
    SDL_DestroyTexture(text_texture);

    text_surf = TTF_RenderText_Blended(roboto, " Menü", white);
    text_texture = SDL_CreateTextureFromSurface(renderer, text_surf);
    SDL_Rect menuRec = {.x = 960, .y = 510, .w = text_surf->w, .h = text_surf->h};
    SDL_FreeSurface(text_surf);
    SDL_RenderCopy(renderer, text_texture, NULL, &menuRec);
    SDL_DestroyTexture(text_texture);

    TTF_CloseFont(roboto);

    SDL_RenderPresent(renderer);

    int started = 0;
    SDL_Event event;
    SDL_Point click;
    while (!started) {
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
                    return requestPlay(renderer, window, background, map, difficulty, score);
                } else if (SDL_PointInRect(&click, &startRec)) {
                    started = 1;
                } else if (SDL_PointInRect(&click, &menuRec)) {
                    return 0;
                }
            }
        }
    }

    int survivors = play(renderer, window, map, difficulty);
    if (survivors < 0) {
        return -1;
    }
    score += survivors;
    map++;
    if (map <= 3) {
        ///save progress
        saveCampaign(map, difficulty, score);
    } else {
        ///all maps played
        ///destroy file
        remove("resources/data/savegame.dat");
    }

    if (survivors == 0) {
        showLostScreen(renderer);
    } else {
        showWinScreen(renderer, survivors);
    }

    if (map > 3) {
        ///get player name for highscore list
        char name[15];
        getName(renderer, name);
        putScore(difficulty, score, name);
        return showHighscores(renderer, window, background);
    }
    ///play next map
    return requestPlay(renderer, window, background, map, difficulty, score);
}
