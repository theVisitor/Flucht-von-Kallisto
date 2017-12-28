#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <time.h>

#define WAIT 5

const int slowFPSwait = (40 * CLOCKS_PER_SEC) / 1000;

/**
 * Function for displaying the SDL-Animation
 */
int createdWithSdl(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    SDL_Texture *sdl_logo = IMG_LoadTexture(renderer, "resources/textures/sdl.png");
    if (sdl_logo == NULL) {
        printf(SDL_GetError());
        return -1;
    }
    SDL_Rect source = {.x = 0, .y = 0, .h = 200, .w = 350};
    SDL_Rect dest = {.x = 393, .y = 217, .h = 200, .w = 350};

    SDL_RenderCopy(renderer, sdl_logo, &source, &dest);
    SDL_RenderPresent(renderer);
    source.w = 457;
    source.h = 266;
    dest.w = 457;
    dest.h = 266;

    SDL_Event event;
    clock_t framestart;
    int state = 0;
    while (state < 60) {
        framestart = clock();
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                SDL_DestroyTexture(sdl_logo);
                return -1;
            }
        }
        if (state == 20) {
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, sdl_logo, &source, &dest);
            SDL_RenderPresent(renderer);
        } else if (state > 20) {
            if (source.x == 1371) {
                if (source.y < 798) {
                    source.x = 0;
                    source.y += 266;
                }
            } else {
                source.x += 457;
            }
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, sdl_logo, &source, &dest);
            SDL_RenderPresent(renderer);
        }
        state++;
        while (framestart + slowFPSwait > clock()) {}
    }
    SDL_DestroyTexture(sdl_logo);
    return 0;
}

/**
 * Function for displaying the GIMP and Blender credits
 */
int createdWithGimpBlender(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    SDL_Texture *logo = IMG_LoadTexture(renderer, "resources/textures/gimpblender.png");
    if (logo == NULL) {
        printf(SDL_GetError());
        return -1;
    }

    SDL_Rect dRect = {.x = 140, .y = 160, .w = 1000, .h = 400};
    SDL_RenderCopy(renderer, logo, NULL, &dRect);
    SDL_RenderPresent(renderer);
    SDL_Delay(1500);
    SDL_DestroyTexture(logo);
    return 0;
}

/**
 * Function for breaking the intro and
 * cleaning up the textures
 */
int breakIntro(SDL_Texture *i1, SDL_Texture *i2, SDL_Texture *i3, SDL_Texture *i4, SDL_Texture *i5, SDL_Texture *i6, SDL_Texture *i7) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)) {
            SDL_DestroyTexture(i1);
            SDL_DestroyTexture(i2);
            SDL_DestroyTexture(i3);
            SDL_DestroyTexture(i4);
            SDL_DestroyTexture(i5);
            SDL_DestroyTexture(i6);
            SDL_DestroyTexture(i7);
            return -1;
        } else if ((event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) || (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_SPACE)) {
            SDL_DestroyTexture(i1);
            SDL_DestroyTexture(i2);
            SDL_DestroyTexture(i3);
            SDL_DestroyTexture(i4);
            SDL_DestroyTexture(i5);
            SDL_DestroyTexture(i6);
            SDL_DestroyTexture(i7);
            return -2;
        }
    }
    return 0;
}

/**
 * Function that renders the intro to the screen
 */
int playIntro(SDL_Renderer *renderer, SDL_Window *window) {
    ///loading all the textures
    ///if one fails, destroy the ones already created
    SDL_Surface *tmp;
    int breakVar = 0;
    clock_t framestart;

    tmp = IMG_Load("resources/textures/Intro1.png");
    if (tmp == NULL) {
        return 0;
    }
    SDL_Texture *intro1 = SDL_CreateTextureFromSurface(renderer, tmp); SDL_FreeSurface(tmp);

    tmp = IMG_Load("resources/textures/Intro2.png");
    if (tmp == NULL) {
        SDL_DestroyTexture(intro1);
        return 0;
    }
    SDL_Texture *intro2 = SDL_CreateTextureFromSurface(renderer, tmp); SDL_FreeSurface(tmp);

    tmp = IMG_Load("resources/textures/Intro3.png");
    if (tmp == NULL) {
        SDL_DestroyTexture(intro1);
        SDL_DestroyTexture(intro2);
        return 0;
    }
    SDL_Texture *intro3 = SDL_CreateTextureFromSurface(renderer, tmp); SDL_FreeSurface(tmp);

    tmp = IMG_Load("resources/textures/Intro4.png");
    if (tmp == NULL) {
        SDL_DestroyTexture(intro1);
        SDL_DestroyTexture(intro2);
        SDL_DestroyTexture(intro3);
        return 0;
    }
    SDL_Texture *intro4 = SDL_CreateTextureFromSurface(renderer, tmp); SDL_FreeSurface(tmp);

    tmp = IMG_Load("resources/textures/Intro5.png");
    if (tmp == NULL) {
        SDL_DestroyTexture(intro1);
        SDL_DestroyTexture(intro2);
        SDL_DestroyTexture(intro3);
        SDL_DestroyTexture(intro4);
        return 0;
    }
    SDL_Texture *intro5 = SDL_CreateTextureFromSurface(renderer, tmp); SDL_FreeSurface(tmp);

    tmp = IMG_Load("resources/textures/Intro6.png");
    if (tmp == NULL) {
        SDL_DestroyTexture(intro1);
        SDL_DestroyTexture(intro2);
        SDL_DestroyTexture(intro3);
        SDL_DestroyTexture(intro4);
        SDL_DestroyTexture(intro5);
        return 0;
    }
    SDL_Texture *intro6 = SDL_CreateTextureFromSurface(renderer, tmp); SDL_FreeSurface(tmp);

    tmp = IMG_Load("resources/textures/Intro7.png");
    if (tmp == NULL) {
        SDL_DestroyTexture(intro1);
        SDL_DestroyTexture(intro2);
        SDL_DestroyTexture(intro3);
        SDL_DestroyTexture(intro4);
        SDL_DestroyTexture(intro5);
        SDL_DestroyTexture(intro6);
        return 0;
    }
    SDL_Texture *intro7 = SDL_CreateTextureFromSurface(renderer, tmp); SDL_FreeSurface(tmp);

    ///create a font
    TTF_Font *roboto = TTF_OpenFont("resources/fonts/RobotoMono-Medium.ttf", 24);
    SDL_Color white = {255, 255, 255};

    breakVar = breakIntro(intro1, intro2, intro3, intro4, intro5, intro6, intro7);
    if (breakVar == -1) {
        return -1;
    } else if (breakVar == -2) {
        return 0;
    }

    ///Let's show the intro!
    SDL_Surface *text_surf = TTF_RenderText_Blended(roboto, "Ende des 21. Jahrhunderts war die Überbevölkerung der Erde auf ihrem Höhepunkt.", white);
    SDL_Texture *text_texture = SDL_CreateTextureFromSurface(renderer, text_surf);
    SDL_Rect textRec = {.x = 50, .y = 650, .w = text_surf->w, .h = text_surf->h};
    SDL_FreeSurface(text_surf);
    SDL_RenderCopy(renderer, intro1, NULL, NULL);
    SDL_RenderCopy(renderer, text_texture, NULL, &textRec);
    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(text_texture);

    framestart = clock();
    while (clock() < framestart + WAIT * CLOCKS_PER_SEC) {
        breakVar = breakIntro(intro1, intro2, intro3, intro4, intro5, intro6, intro7);
        if (breakVar == -1) {
            return -1;
        } else if (breakVar == -2) {
            return 0;
        }
        SDL_Delay(25);
    }

    text_surf = TTF_RenderText_Blended(roboto, "Die Kolonialisierung des Sonnensystems wurde unausweichlich.", white);
    text_texture = SDL_CreateTextureFromSurface(renderer, text_surf);
    textRec.w = text_surf->w;
    textRec.h = text_surf->h;
    SDL_FreeSurface(text_surf);
    SDL_RenderCopy(renderer, intro1, NULL, NULL);
    SDL_RenderCopy(renderer, text_texture, NULL, &textRec);
    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(text_texture);

    framestart = clock();
    while (clock() < framestart + WAIT * CLOCKS_PER_SEC) {
        breakVar = breakIntro(intro1, intro2, intro3, intro4, intro5, intro6, intro7);
        if (breakVar == -1) {
            return -1;
        } else if (breakVar == -2) {
            return 0;
        }
        SDL_Delay(25);
    }

    text_surf = TTF_RenderText_Blended(roboto, "Untersucht wurde unter anderem der zweitgrößte Mond des Jupiter, Kallisto.", white);
    text_texture = SDL_CreateTextureFromSurface(renderer, text_surf);
    textRec.w = text_surf->w;
    textRec.h = text_surf->h;
    SDL_FreeSurface(text_surf);
    SDL_RenderCopy(renderer, intro2, NULL, NULL);
    SDL_RenderCopy(renderer, text_texture, NULL, &textRec);
    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(text_texture);

    framestart = clock();
    while (clock() < framestart + WAIT * CLOCKS_PER_SEC) {
        breakVar = breakIntro(intro1, intro2, intro3, intro4, intro5, intro6, intro7);
        if (breakVar == -1) {
            return -1;
        } else if (breakVar == -2) {
            return 0;
        }
        SDL_Delay(25);
    }

    text_surf = TTF_RenderText_Blended(roboto, "Frühere Missionen zeigten bereits, dass dieser zwar ohne Leben, aber bewohnbar war.", white);
    text_texture = SDL_CreateTextureFromSurface(renderer, text_surf);
    textRec.w = text_surf->w;
    textRec.h = text_surf->h;
    SDL_FreeSurface(text_surf);
    SDL_RenderCopy(renderer, intro3, NULL, NULL);
    SDL_RenderCopy(renderer, text_texture, NULL, &textRec);
    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(text_texture);

    framestart = clock();
    while (clock() < framestart + WAIT * CLOCKS_PER_SEC) {
        breakVar = breakIntro(intro1, intro2, intro3, intro4, intro5, intro6, intro7);
        if (breakVar == -1) {
            return -1;
        } else if (breakVar == -2) {
            return 0;
        }
        SDL_Delay(25);
    }

    text_surf = TTF_RenderText_Blended(roboto, "Mehrere Teams machten sich auf den Weg, um den Mond zu besiedeln.", white);
    text_texture = SDL_CreateTextureFromSurface(renderer, text_surf);
    textRec.w = text_surf->w;
    textRec.h = text_surf->h;
    SDL_FreeSurface(text_surf);
    SDL_RenderCopy(renderer, intro4, NULL, NULL);
    SDL_RenderCopy(renderer, text_texture, NULL, &textRec);
    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(text_texture);

    framestart = clock();
    while (clock() < framestart + WAIT * CLOCKS_PER_SEC) {
        breakVar = breakIntro(intro1, intro2, intro3, intro4, intro5, intro6, intro7);
        if (breakVar == -1) {
            return -1;
        } else if (breakVar == -2) {
            return 0;
        }
        SDL_Delay(25);
    }

    text_surf = TTF_RenderText_Blended(roboto, "Die Distanz zu Kallisto war bald überwunden und der Aufbau der Kolonialzentren begann.", white);
    text_texture = SDL_CreateTextureFromSurface(renderer, text_surf);
    textRec.w = text_surf->w;
    textRec.h = text_surf->h;
    SDL_FreeSurface(text_surf);
    SDL_RenderCopy(renderer, intro5, NULL, NULL);
    SDL_RenderCopy(renderer, text_texture, NULL, &textRec);
    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(text_texture);

    framestart = clock();
    while (clock() < framestart + WAIT * CLOCKS_PER_SEC) {
        breakVar = breakIntro(intro1, intro2, intro3, intro4, intro5, intro6, intro7);
        if (breakVar == -1) {
            return -1;
        } else if (breakVar == -2) {
            return 0;
        }
        SDL_Delay(25);
    }

    text_surf = TTF_RenderText_Blended(roboto, "Doch anders als zuerst angenommen, waren die Siedler nicht alleine...", white);
    text_texture = SDL_CreateTextureFromSurface(renderer, text_surf);
    textRec.w = text_surf->w;
    textRec.h = text_surf->h;
    SDL_FreeSurface(text_surf);
    SDL_RenderCopy(renderer, intro6, NULL, NULL);
    SDL_RenderCopy(renderer, text_texture, NULL, &textRec);
    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(text_texture);

    framestart = clock();
    while (clock() < framestart + WAIT * CLOCKS_PER_SEC) {
        breakVar = breakIntro(intro1, intro2, intro3, intro4, intro5, intro6, intro7);
        if (breakVar == -1) {
            return -1;
        } else if (breakVar == -2) {
            return 0;
        }
        SDL_Delay(25);
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    text_surf = TTF_RenderText_Blended(roboto, "Und keineswegs willkommen.", white);
    text_texture = SDL_CreateTextureFromSurface(renderer, text_surf);
    textRec.w = text_surf->w;
    textRec.h = text_surf->h;
    SDL_FreeSurface(text_surf);
    SDL_RenderCopy(renderer, intro7, NULL, NULL);
    SDL_RenderCopy(renderer, text_texture, NULL, &textRec);
    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(text_texture);

    framestart = clock();
    while (clock() < framestart + WAIT * CLOCKS_PER_SEC) {
        breakVar = breakIntro(intro1, intro2, intro3, intro4, intro5, intro6, intro7);
        if (breakVar == -1) {
            return -1;
        } else if (breakVar == -2) {
            return 0;
        }
        SDL_Delay(25);
    }

    if (SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN) {
        ///fading out doesn't work properly in fullscreen :/
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
        SDL_Delay(500);
    } else {
        ///no fullsreen - use the awesome fading out feature
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 12);
        for (int i = 0; i < 30; i++) {
            SDL_RenderFillRect(renderer, NULL);
            SDL_RenderPresent(renderer);
            SDL_Delay(50);
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 50);
        for (int i = 0; i < 30; i++) {
            SDL_RenderFillRect(renderer, NULL);
            SDL_RenderPresent(renderer);
            SDL_Delay(50);
    }

    }

    framestart = clock();
    while (clock() < framestart + CLOCKS_PER_SEC / 2) {
        breakVar = breakIntro(intro1, intro2, intro3, intro4, intro5, intro6, intro7);
        if (breakVar == -1) {
            return -1;
        } else if (breakVar == -2) {
            return 0;
        }
        SDL_Delay(25);
    }

    ///final texts, no images
    text_surf = TTF_RenderText_Blended(roboto, "Die Siedler konnten den Angriffen nicht lange standhalten.", white);
    text_texture = SDL_CreateTextureFromSurface(renderer, text_surf);
    textRec.x = 220;
    textRec.y = 340;
    textRec.w = text_surf->w;
    textRec.h = text_surf->h;
    SDL_FreeSurface(text_surf);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, text_texture, NULL, &textRec);
    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(text_texture);

    framestart = clock();
    while (clock() < framestart + WAIT * CLOCKS_PER_SEC) {
        breakVar = breakIntro(intro1, intro2, intro3, intro4, intro5, intro6, intro7);
        if (breakVar == -1) {
            return -1;
        } else if (breakVar == -2) {
            return 0;
        }
        SDL_Delay(25);
    }

    text_surf = TTF_RenderText_Blended(roboto, "Abgeschnitten vom Rest der Menschheit, blieb ihnen nur eine Wahl...", white);
    text_texture = SDL_CreateTextureFromSurface(renderer, text_surf);
    textRec.x = 160;
    textRec.w = text_surf->w;
    textRec.h = text_surf->h;
    SDL_FreeSurface(text_surf);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, text_texture, NULL, &textRec);
    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(text_texture);
    TTF_CloseFont(roboto);

    framestart = clock();
    while (clock() < framestart + WAIT * CLOCKS_PER_SEC) {
        breakVar = breakIntro(intro1, intro2, intro3, intro4, intro5, intro6, intro7);
        if (breakVar == -1) {
            return -1;
        } else if (breakVar == -2) {
            return 0;
        }
        SDL_Delay(25);
    }

    roboto = TTF_OpenFont("resources/fonts/RobotoMono-Medium.ttf", 50);
    text_surf = TTF_RenderText_Blended(roboto, "Die Flucht von Kallisto", white);
    text_texture = SDL_CreateTextureFromSurface(renderer, text_surf);
    textRec.x = 300;
    textRec.y = 330;
    textRec.w = text_surf->w;
    textRec.h = text_surf->h;
    SDL_FreeSurface(text_surf);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, text_texture, NULL, &textRec);
    SDL_RenderPresent(renderer);
    ///clean up text-stuff
    SDL_DestroyTexture(text_texture);
    TTF_CloseFont(roboto);

    framestart = clock();
    while (clock() < framestart + WAIT * CLOCKS_PER_SEC) {
        breakVar = breakIntro(intro1, intro2, intro3, intro4, intro5, intro6, intro7);
        if (breakVar == -1) {
            return -1;
        } else if (breakVar == -2) {
            return 0;
        }
        SDL_Delay(25);
    }

    ///clean up textures
    SDL_DestroyTexture(intro1);
    SDL_DestroyTexture(intro2);
    SDL_DestroyTexture(intro3);
    SDL_DestroyTexture(intro4);
    SDL_DestroyTexture(intro5);
    SDL_DestroyTexture(intro6);
    SDL_DestroyTexture(intro7);

    return 0;
}
