#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "declarations.h"

#define BULLETREC {.x = 15, .y = 3, .w = 5, .h = 3}

TurretData turretData[8][4] = {
    {
        {.range = 105, .reload = 40, .magazine = 12, .cycleTime = 20, .buildTime = 40, .buildCost = 10, .isAuto = 0},
        {.range = 140, .reload = 40, .magazine = 32, .cycleTime = 4, .buildTime = 80, .buildCost = 20, .isAuto = 0},
        {.range = 175, .reload = 40, .magazine = 20, .cycleTime = 15, .buildTime = 80, .buildCost = 20, .isAuto = 0},
        {.range = 210, .reload = 40, .magazine = 3, .cycleTime = 20, .buildTime = 120, .buildCost = 40, .isAuto = 0},
    },
    {
        {.range = 140, .reload = 80, .magazine = 30, .cycleTime = 8, .buildTime = 80, .buildCost = 20, .isAuto = 0},
        {.range = 175, .reload = 100, .magazine = 150, .cycleTime = 4, .buildTime = 120, .buildCost = 40, .isAuto = 0},
        {.range = 175, .reload = 80, .magazine = 200, .cycleTime = 3, .buildTime = 40, .buildCost = 50, .isAuto = 0},
        {.range = 200, .reload = 40, .magazine = 60, .cycleTime = 1, .buildTime = 200, .buildCost = 100, .isAuto = 1},
    },
    {
        {.range = 105, .reload = 40, .magazine = 1, .cycleTime = 1, .buildTime = 60, .buildCost = 20, .isAuto = 0},
        {.range = 140, .reload = 40, .magazine = 6, .cycleTime = 15, .buildTime = 120, .buildCost = 25, .isAuto = 0},
        {.range = 140, .reload = 80, .magazine = 120, .cycleTime = 1, .buildTime = 120, .buildCost = 90, .isAuto = 0},
        {.range = 175, .reload = 60, .magazine = 160, .cycleTime = 1, .buildTime = 160, .buildCost = 60, .isAuto = 1},
    },
    {
        {.range = 175, .reload = 50, .magazine = 1, .cycleTime = 1, .buildTime = 60, .buildCost = 50, .isAuto = 0},
        {.range = 210, .reload = 40, .magazine = 1, .cycleTime = 1, .buildTime = 40, .buildCost = 40, .isAuto = 0},
        {.range = 245, .reload = 35, .magazine = 1, .cycleTime = 1, .buildTime = 200, .buildCost = 60, .isAuto = 1},
        {.range = 315, .reload = 120, .magazine = 8, .cycleTime = 15, .buildTime = 200, .buildCost = 60, .isAuto = 1},
    },
    {
        {.range = 105, .reload = 50, .magazine = 5, .cycleTime = 20, .buildTime = 60, .buildCost = 80, .isAuto = 0},
        {.range = 150, .reload = 30, .magazine = 1, .cycleTime = 1, .buildTime = 60, .buildCost = 30, .isAuto = 0},
        {.range = 200, .reload = 40, .magazine = 2, .cycleTime = 15, .buildTime = 160, .buildCost = 50, .isAuto = 1},
        {.range = 220, .reload = 40, .magazine = 6, .cycleTime = 15, .buildTime = 200, .buildCost = 70, .isAuto = 1},
    },
    {
        {.range = 175, .reload = 30, .magazine = 40, .cycleTime = 4, .buildTime = 120, .buildCost = 80, .isAuto = 1},
        {.range = 210, .reload = 30, .magazine = 80, .cycleTime = 3, .buildTime = 160, .buildCost = 60, .isAuto = 1},
        {.range = 210, .reload = 60, .magazine = 160, .cycleTime = 1, .buildTime = 200, .buildCost = 80, .isAuto = 1},
        {.range = 210, .reload = 60, .magazine = 120, .cycleTime = 1, .buildTime = 240, .buildCost = 170, .isAuto = 1},
    },
    {
        {.range = 175, .reload = 29, .magazine = 1, .cycleTime = 1, .buildTime = 160, .buildCost = 70, .isAuto = 1},
        {.range = 210, .reload = 19, .magazine = 1, .cycleTime = 1, .buildTime = 160, .buildCost = 70, .isAuto = 1},
        {.range = 245, .reload = 19, .magazine = 1, .cycleTime = 1, .buildTime = 200, .buildCost = 80, .isAuto = 1},
        {.range = 280, .reload = 14, .magazine = 1, .cycleTime = 1, .buildTime = 200, .buildCost = 120, .isAuto = 1},
    },
    {
        {.range = 175, .reload = 30, .magazine = 5, .cycleTime = 1, .buildTime = 200, .buildCost = 60, .isAuto = 1},
        {.range = 175, .reload = 30, .magazine = 10, .cycleTime = 1, .buildTime = 240, .buildCost = 70, .isAuto = 1},
        {.range = 200, .reload = 5, .magazine = 5, .cycleTime = 1, .buildTime = 280, .buildCost = 120, .isAuto = 1},
        {.range = 280, .reload = 0, .magazine = 1, .cycleTime = 1, .buildTime = 280, .buildCost = 220, .isAuto = 1},
    },
};

FireObjectData fOData[8][4] = {
    {
        {.damage = 4, .speed = 15, .radius = 1, .maxLife = 7, .rect = BULLETREC},
        {.damage = 4, .speed = 15, .radius = 1, .maxLife = 9, .rect = BULLETREC},
        {.damage = 22, .speed = 20, .radius = 1, .maxLife = 9, .rect = BULLETREC},
        {.damage = 75, .speed = 25, .radius = 1, .maxLife = 9, .rect = BULLETREC},
    },
    {
        {.damage = 5, .speed = 15, .radius = 1, .maxLife = 10, .rect = BULLETREC},
        {.damage = 7, .speed = 15, .radius = 1, .maxLife = 12, .rect = BULLETREC},
        {.damage = 8, .speed = 15, .radius = 1, .maxLife = 12, .rect = BULLETREC},
        {.damage = 8, .speed = 15, .radius = 1, .maxLife = 14, .rect = BULLETREC},
    },
    {
        {.damage = 3, .speed = 15, .radius = 1, .maxLife = 7, .rect = BULLETREC},
        {.damage = 3, .speed = 15, .radius = 1, .maxLife = 9, .rect = BULLETREC},
        {.damage = 2, .speed = 12, .radius = 4, .maxLife = 12, .rect = {.x = 0, .y = 0, .w = 15, .h = 15}},
        {.damage = 2, .speed = 15, .radius = 4, .maxLife = 12, .rect = {.x = 0, .y = 0, .w = 15, .h = 15}},
    },
    {
        {.damage = 45, .speed = 15, .radius = 20, .maxLife = 0, .rect = {.x = 15, .y = 0, .w = 9, .h = 3}},
        {.damage = 55, .speed = 15, .radius = 30, .maxLife = 0, .rect = {.x = 15, .y = 0, .w = 9, .h = 3}},
        {.damage = 65, .speed = 20, .radius = 35, .maxLife = 0, .rect = {.x = 19, .y = 20, .w = 7, .h = 3}},
        {.damage = 70, .speed = 25, .radius = 50, .maxLife = 0, .rect = {.x = 0, .y = 15, .w = 29, .h = 5}},
    },
    {
        {.damage = 35, .speed = 15, .radius = 20, .maxLife = 7, .rect = {.x = 19, .y = 20, .w = 7, .h = 3}},
        {.damage = 50, .speed = 20, .radius = 25, .maxLife = 8, .rect = {.x = 15, .y = 7, .w = 11, .h = 3}},
        {.damage = 60, .speed = 25, .radius = 35, .maxLife = 8, .rect = {.x = 0, .y = 20, .w = 19, .h = 3}},
        {.damage = 60, .speed = 25, .radius = 35, .maxLife = 9, .rect = {.x = 0, .y = 20, .w = 19, .h = 3}},
    },
    {
        {.damage = 8, .speed = 15, .radius = 2, .maxLife = 12, .rect = BULLETREC},
        {.damage = 10, .speed = 15, .radius = 2, .maxLife = 14, .rect = BULLETREC},
        {.damage = 7, .speed = 15, .radius = 2, .maxLife = 14, .rect = BULLETREC},
        {.damage = 7, .speed = 15, .radius = 2, .maxLife = 14, .rect = BULLETREC},
    },
    {
        {.damage = 25, .speed = 25, .radius = 4, .maxLife = 8, .rect = {.x = 15, .y = 10, .w = 11, .h = 5}},
        {.damage = 35, .speed = 25, .radius = 4, .maxLife = 9, .rect = {.x = 15, .y = 10, .w = 11, .h = 5}},
        {.damage = 55, .speed = 30, .radius = 4, .maxLife = 9, .rect = {.x = 15, .y = 10, .w = 11, .h = 5}},
        {.damage = 70, .speed = 35, .radius = 4, .maxLife = 9, .rect = {.x = 15, .y = 10, .w = 11, .h = 5}},
    },
    {
        {.damage = 5, .speed = 0, .radius = 0, .maxLife = 0, .rect = {.x = 0, .y = 0, .w = 0, .h = 0}},
        {.damage = 6, .speed = 0, .radius = 0, .maxLife = 0, .rect = {.x = 0, .y = 0, .w = 0, .h = 0}},
        {.damage = 10, .speed = 0, .radius = 0, .maxLife = 0, .rect = {.x = 0, .y = 0, .w = 0, .h = 0}},
        {.damage = 10, .speed = 0, .radius = 0, .maxLife = 0, .rect = {.x = 0, .y = 0, .w = 0, .h = 0}},
    },
};

EnemyData enemyData[3] = {
     {.radius = 18, .speed = 1, .aniCycleFrames = 70, .aniCycleStates = 2, .rect = {.x = 0, .y = 32, .w = 35, .h = 35}},
     {.radius = 10, .speed = 2, .aniCycleFrames = 28, .aniCycleStates = 2, .rect = {.x = 0, .y = 0, .w = 23, .h = 15}},
     {.radius = 8, .speed = 3, .aniCycleFrames = 8, .aniCycleStates = 2, .rect = {.x = 0, .y = 15, .w = 13, .h = 17}},
};

/**
 * Function for loading all textures needed
 */
int loadTextures(Graphics *graphics, int level) {
    SDL_Surface *tmp;

    tmp = IMG_Load("resources/textures/turretbase.png");
    if (tmp == NULL) {
        return -1;
    }
    graphics->turretbase = SDL_CreateTextureFromSurface(graphics->renderer, tmp); SDL_FreeSurface(tmp);

    tmp = IMG_Load("resources/textures/turrets.png");
    if (tmp == NULL) {
        return -1;
    }
    graphics->turrethead = SDL_CreateTextureFromSurface(graphics->renderer, tmp); SDL_FreeSurface(tmp);

    tmp = IMG_Load("resources/textures/construction.png");
    if (tmp == NULL) {
        return -1;
    }
    graphics->construction = SDL_CreateTextureFromSurface(graphics->renderer, tmp); SDL_FreeSurface(tmp);

    tmp = IMG_Load("resources/textures/fos.png");
    if (tmp == NULL) {
        return -1;
    }
    graphics->fireObjectTX = SDL_CreateTextureFromSurface(graphics->renderer, tmp); SDL_FreeSurface(tmp);

    tmp = IMG_Load("resources/textures/explosion.png");
    if (tmp == NULL) {
        return -1;
    }
    graphics->explosionTX = SDL_CreateTextureFromSurface(graphics->renderer, tmp); SDL_FreeSurface(tmp);

    tmp = IMG_Load("resources/textures/enemies.png");
    if (tmp == NULL) {
        return -1;
    }
    graphics->enemyTX = SDL_CreateTextureFromSurface(graphics->renderer, tmp); SDL_FreeSurface(tmp);

    tmp = IMG_Load("resources/textures/gui.png");
    if (tmp == NULL) {
        return -1;
    }
    graphics->gui = SDL_CreateTextureFromSurface(graphics->renderer, tmp); SDL_FreeSurface(tmp);

    tmp = IMG_Load("resources/textures/selection.png");
    if (tmp == NULL) {
        return -1;
    }
    graphics->selection = SDL_CreateTextureFromSurface(graphics->renderer, tmp); SDL_FreeSurface(tmp);

    tmp = IMG_Load("resources/textures/base.png");
    if (tmp == NULL) {
        return -1;
    }
    graphics->base = SDL_CreateTextureFromSurface(graphics->renderer, tmp); SDL_FreeSurface(tmp);

    tmp = IMG_Load("resources/textures/speed.png");
    if (tmp == NULL) {
        return -1;
    }
    graphics->speedbutton = SDL_CreateTextureFromSurface(graphics->renderer, tmp); SDL_FreeSurface(tmp);


    switch (level) {
    case 1: tmp = IMG_Load("resources/textures/starterslevel.png"); break;
    case 2: tmp = IMG_Load("resources/textures/helixlevel.png"); break;
    case 3: tmp = IMG_Load("resources/textures/canyonlevel.png"); break;
    }
    if (tmp == NULL) {
        return -1;
    }
    graphics->background = SDL_CreateTextureFromSurface(graphics->renderer, tmp); SDL_FreeSurface(tmp);

    return createText(graphics);
}

/**
 * Function to free graphics memory after level is done
 */
void unloadTextures(Graphics *graphics) {
    SDL_DestroyTexture(graphics->turretbase);
    SDL_DestroyTexture(graphics->turrethead);
    SDL_DestroyTexture(graphics->construction);
    SDL_DestroyTexture(graphics->fireObjectTX);
    SDL_DestroyTexture(graphics->explosionTX);
    SDL_DestroyTexture(graphics->enemyTX);
    SDL_DestroyTexture(graphics->gui);
    SDL_DestroyTexture(graphics->selection);
    SDL_DestroyTexture(graphics->background);
    SDL_DestroyTexture(graphics->numbers);
    SDL_DestroyTexture(graphics->descriptions);
    SDL_DestroyTexture(graphics->buttons);
    SDL_DestroyTexture(graphics->base);
    SDL_DestroyTexture(graphics->speedbutton);
}

/**
 * Function for creating text-based textures
 */
int createText(Graphics *graphics) {
    TTF_Font *roboto = TTF_OpenFont("resources/fonts/RobotoMono-Medium.ttf", 12);

    SDL_Color black = {0, 0, 0};

    SDL_Surface *finalSurf = SDL_CreateRGBSurfaceWithFormat(0, 246 * 4, 350 * 8, 32, SDL_PIXELFORMAT_ARGB8888);
    SDL_FillRect(finalSurf, NULL, -1);
    SDL_Surface *textsurf;
    SDL_Rect target;

    ///descriptions
    target.y = 350 * PISTOL;
    textsurf = TTF_RenderText_Blended_Wrapped(roboto, "Pistole\
                                              manuell\
                                              \n\
                                              Reichweite: 105\
                                              Schaden:    4\
                                              Magazin:    12\
                                              Kadenz:     120 rpm\
                                              Nachladen:  1s\
                                              \n\
                                              Bauzeit:    1s\
                                              Baukosten:  10\
                                              \n\
                                              Eine mit Pistole bewaffnete Person.",
                                              black, 246);
    target.x = 0;
    SDL_BlitSurface(textsurf, NULL, finalSurf, &target); SDL_FreeSurface(textsurf);

    textsurf = TTF_RenderText_Blended_Wrapped(roboto, "Maschinenpistole\
                                              manuell\
                                              \n\
                                              Reichweite: 140\
                                              Schaden:    4\
                                              Magazin:    32\
                                              Kadenz:     600 rpm\
                                              Nachladen:  1s\
                                              \n\
                                              Bauzeit:    2s\
                                              Baukosten:  20\
                                              \n\
                                              Mit einer Maschinenpistole kann der Schütze stärkere Gegner bekämpfen.",
                                              black, 246);
    target.x = 246;
    SDL_BlitSurface(textsurf, NULL, finalSurf, &target); SDL_FreeSurface(textsurf);

    textsurf = TTF_RenderText_Blended_Wrapped(roboto, "Semiautomatik\
                                              manuell\
                                              \n\
                                              Reichweite: 175\
                                              Schaden:    22\
                                              Magazin:    20\
                                              Kadenz:     160 rpm\
                                              Nachladen:  1s\
                                              \n\
                                              Bauzeit:    2s\
                                              Baukosten:  20\
                                              \n\
                                              Die Semiautomatik schießt weniger schnell, aber sehr durschlagskräftige Projektile.",
                                              black, 246);
    target.x = 492;
    SDL_BlitSurface(textsurf, NULL, finalSurf, &target); SDL_FreeSurface(textsurf);

    textsurf = TTF_RenderText_Blended_Wrapped(roboto, "Scharfschützgewehr\
                                              manuell\
                                              \n\
                                              Reichweite: 210\
                                              Schaden:    75\
                                              Magazin:    3\
                                              Kadenz:     90 rpm\
                                              Nachladen:  1s\
                                              \n\
                                              Bauzeit:    3s\
                                              Baukosten:  40\
                                              \n\
                                              Durch verbesserte Visieroptiken trifft der Scharfschütze nahezu immer und mit hoher Kraft.",
                                              black, 246);
    target.x = 738;
    SDL_BlitSurface(textsurf, NULL, finalSurf, &target); SDL_FreeSurface(textsurf);


    target.y = 350 * RIFLE;
    textsurf = TTF_RenderText_Blended_Wrapped(roboto, "Sturmgewehr\
                                              manuell\
                                              \n\
                                              Reichweite: 140\
                                              Schaden:    5\
                                              Magazin:    30\
                                              Kadenz:     300 rpm\
                                              Nachladen:  2s\
                                              \n\
                                              Bauzeit:    2s\
                                              Baukosten:  20\
                                              \n\
                                              Das Sturmgewehr hat eine hohe Feuerrate bei geringem Preis.",
                                              black, 246);
    target.x = 0;
    SDL_BlitSurface(textsurf, NULL, finalSurf, &target); SDL_FreeSurface(textsurf);

    textsurf = TTF_RenderText_Blended_Wrapped(roboto, "Maschinengewehr\
                                              manuell\
                                              \n\
                                              Reichweite: 175\
                                              Schaden:    7\
                                              Magazin:    150\
                                              Kadenz:     600 rpm\
                                              Nachladen:  2,5s\
                                              \n\
                                              Bauzeit:    3s\
                                              Baukosten:  40\
                                              \n\
                                              Das Maschinengewehr ist eine starke und günstige Schnellfeuerwaffe.",
                                              black, 246);
    target.x = 246;
    SDL_BlitSurface(textsurf, NULL, finalSurf, &target); SDL_FreeSurface(textsurf);

    textsurf = TTF_RenderText_Blended_Wrapped(roboto, "Schweres MG\
                                              manuell\
                                              \n\
                                              Reichweite: 175\
                                              Schaden:    8\
                                              Magazin:    150\
                                              Kadenz:     800 rpm\
                                              Nachladen:  2s\
                                              \n\
                                              Bauzeit:    1s\
                                              Baukosten:  50\
                                              \n\
                                              Das schwere Maschinengewehr hat ein noch höhere Feuerrate und verursacht durch sein größeres Kaliber mehr Schaden.",
                                              black, 246);
    target.x = 492;
    SDL_BlitSurface(textsurf, NULL, finalSurf, &target); SDL_FreeSurface(textsurf);

    textsurf = TTF_RenderText_Blended_Wrapped(roboto, "Automatikgeschütz\
                                              automatisch\
                                              \n\
                                              Reichweite: 200\
                                              Schaden:    8\
                                              Magazin:    200\
                                              Kadenz:     2400 rpm\
                                              Nachladen:  1s\
                                              \n\
                                              Bauzeit:    5s\
                                              Baukosten:  100\
                                              \n\
                                              Das Automatikgeschütz zielt selbständig und schießt mit einer der höchsten Feuerraten.",
                                              black, 246);
    target.x = 738;
    SDL_BlitSurface(textsurf, NULL, finalSurf, &target); SDL_FreeSurface(textsurf);


    target.y = 350 * SHOTGUN;
    textsurf = TTF_RenderText_Blended_Wrapped(roboto, "Schrotflinte\
                                              manuell\
                                              \n\
                                              Reichweite: 105\
                                              Schaden:    3 (8x)\
                                              Magazin:    1\
                                              Nachladen:  1s\
                                              \n\
                                              Bauzeit:    1,5s\
                                              Baukosten:  20\
                                              \n\
                                              Mit einer Schrotflinte lassen sich mehrere Gegner gleichzeitig treffen.",
                                              black, 246);
    target.x = 0;
    SDL_BlitSurface(textsurf, NULL, finalSurf, &target); SDL_FreeSurface(textsurf);

    textsurf = TTF_RenderText_Blended_Wrapped(roboto, "Halbautomatische Schrotflinte\
                                              manuell\
                                              \n\
                                              Reichweite: 140\
                                              Schaden:    3 (8x)\
                                              Magazin:    6\
                                              Kadenz:     160 rpm\
                                              Nachladen:  1s\
                                              \n\
                                              Bauzeit:    3s\
                                              Baukosten:  25\
                                              \n\
                                              Die halbautomatische Schrotflinte lagert mehrere Patronen und kann so schneller feuern.",
                                              black, 246);
    target.x = 246;
    SDL_BlitSurface(textsurf, NULL, finalSurf, &target); SDL_FreeSurface(textsurf);

    textsurf = TTF_RenderText_Blended_Wrapped(roboto, "Flammenwerfer\
                                              manuell\
                                              \n\
                                              Reichweite: 140\
                                              Schaden:    240/s\
                                              Magazin:    3s\
                                              Nachladen:  2s\
                                              \n\
                                              Bauzeit:    3s\
                                              Baukosten:  90\
                                              \n\
                                              Der Flammenwerfer ist auf kurze Distanz eine sehr starke Waffe.",
                                              black, 246);
    target.x = 492;
    SDL_BlitSurface(textsurf, NULL, finalSurf, &target); SDL_FreeSurface(textsurf);

    textsurf = TTF_RenderText_Blended_Wrapped(roboto, "Flammenwerfergeschütz\
                                              automatisch\
                                              \n\
                                              Reichweite: 175\
                                              Schaden:    240/s\
                                              Magazin:    4s\
                                              Nachladen:  1,5s\
                                              \n\
                                              Bauzeit:    4s\
                                              Baukosten:  60\
                                              \n\
                                              Als automatisches Geschütz schießt der Flammenwerfer weiter und länger.",
                                              black, 246);
    target.x = 738;
    SDL_BlitSurface(textsurf, NULL, finalSurf, &target); SDL_FreeSurface(textsurf);


    target.y = 350 * MORTAR;
    textsurf = TTF_RenderText_Blended_Wrapped(roboto, "Mörser I\
                                              manuell\
                                              \n\
                                              Reichweite: 175\
                                              Schaden:    45\
                                              Exp.Radius: 20\
                                              Kadenz:     47 rpm\
                                              \n\
                                              Bauzeit:    1,5s\
                                              Baukosten:  50\
                                              \n\
                                              Der Mörser verschießt explosive Geschosse mit hoher Präzision.",
                                              black, 246);
    target.x = 0;
    SDL_BlitSurface(textsurf, NULL, finalSurf, &target); SDL_FreeSurface(textsurf);

    textsurf = TTF_RenderText_Blended_Wrapped(roboto, "Mörser II\
                                              manuell\
                                              \n\
                                              Reichweite: 210\
                                              Schaden:    55\
                                              Exp.Radius: 30\
                                              Kadenz:     58 rpm\
                                              \n\
                                              Bauzeit:    1s\
                                              Baukosten:  40\
                                              \n\
                                              Der verbesserte Mörser schießt weiter, schneller und mit stärkeren Geschossen.",
                                              black, 246);
    target.x = 246;
    SDL_BlitSurface(textsurf, NULL, finalSurf, &target); SDL_FreeSurface(textsurf);

    textsurf = TTF_RenderText_Blended_Wrapped(roboto, "Artillerie\
                                              automatisch\
                                              \n\
                                              Reichweite: 245\
                                              Schaden:    65\
                                              Exp.Radius: 35\
                                              Kadenz:     67 rpm\
                                              \n\
                                              Bauzeit:    5s\
                                              Baukosten:  60\
                                              \n\
                                              Die Artillerie kann auch weit entfernte Gegnergruppen gezielt bekämpfen.",
                                              black, 246);
    target.x = 492;
    SDL_BlitSurface(textsurf, NULL, finalSurf, &target); SDL_FreeSurface(textsurf);

    textsurf = TTF_RenderText_Blended_Wrapped(roboto, "Raketenartillerie\
                                              automatisch\
                                              \n\
                                              Reichweite: 315\
                                              Schaden:    70\
                                              Exp.Radius: 50\
                                              Magazin:    8\
                                              Kadenz:     160 rpm\
                                              Nachladen:  3s\
                                              \n\
                                              Bauzeit:    5s\
                                              Baukosten:  60\
                                              \n\
                                              Die Raketenartillerie verschießt eine Serie von Raketen mit hoher Reichweite und Explosionskraft.",
                                              black, 246);
    target.x = 738;
    SDL_BlitSurface(textsurf, NULL, finalSurf, &target); SDL_FreeSurface(textsurf);


    target.y = 350 * ROCKET;
    textsurf = TTF_RenderText_Blended_Wrapped(roboto, "Granatwerfer\
                                              manuell\
                                              \n\
                                              Reichweite: 105\
                                              Schaden:    35\
                                              Exp.Radius: 20\
                                              Magazin:    5\
                                              Kadenz:     120 rpm\
                                              Nachladen:  1,25s\
                                              \n\
                                              Bauzeit:    1,5s\
                                              Baukosten:  80\
                                              \n\
                                              Der Granatwerfer bringt hohe explosive Feuerkraft auf kurze Distanz.",
                                              black, 246);
    target.x = 0;
    SDL_BlitSurface(textsurf, NULL, finalSurf, &target); SDL_FreeSurface(textsurf);

    textsurf = TTF_RenderText_Blended_Wrapped(roboto, "RPG\
                                              manuell\
                                              \n\
                                              Reichweite: 150\
                                              Schaden:    50\
                                              Exp.Radius: 25\
                                              Kadenz:     77 rpm\
                                              \n\
                                              Bauzeit:    1,5s\
                                              Baukosten:  30\
                                              \n\
                                              Die RPG schießt weiter und kraftvoller.",
                                              black, 246);
    target.x = 246;
    SDL_BlitSurface(textsurf, NULL, finalSurf, &target); SDL_FreeSurface(textsurf);

    textsurf = TTF_RenderText_Blended_Wrapped(roboto, "Raketenwerfer\
                                              automatisch\
                                              \n\
                                              Reichweite: 200\
                                              Schaden:    60\
                                              Exp.Radius: 35\
                                              Magazin:    2\
                                              Kadenz:     160 rpm\
                                              Nachladen:  1s\
                                              \n\
                                              Bauzeit:    4s\
                                              Baukosten:  50\
                                              \n\
                                              Der Raketenwerfer feuert mit zwei Kurzstrecken Raketen auf sein Ziel.",
                                              black, 246);
    target.x = 492;
    SDL_BlitSurface(textsurf, NULL, finalSurf, &target); SDL_FreeSurface(textsurf);

    textsurf = TTF_RenderText_Blended_Wrapped(roboto, "Raketenwerfer II\
                                              automatisch\
                                              \n\
                                              Reichweite: 220\
                                              Schaden:    60\
                                              Exp.Radius: 35\
                                              Magazin:    6\
                                              Kadenz:     160 rpm\
                                              Nachladen:  1s\
                                              \n\
                                              Bauzeit:    5s\
                                              Baukosten:  70\
                                              \n\
                                              Der größere Raketenwerfer kann größere Salven abfeuern.",
                                              black, 246);
    target.x = 738;
    SDL_BlitSurface(textsurf, NULL, finalSurf, &target); SDL_FreeSurface(textsurf);


    target.y = 350 * CANNON;
    textsurf = TTF_RenderText_Blended_Wrapped(roboto, "Maschinenkanone I\
                                              automatisch\
                                              \n\
                                              Reichweite: 175\
                                              Schaden:    8\
                                              Magazin:    40\
                                              Kadenz:     600 rpm\
                                              Nachladen:  0,75s\
                                              \n\
                                              Bauzeit:    3s\
                                              Baukosten:  80\
                                              \n\
                                              Die Maschinenkanone verbindet hohe Feuerraten mit großem Schaden.",
                                              black, 246);
    target.x = 0;
    SDL_BlitSurface(textsurf, NULL, finalSurf, &target); SDL_FreeSurface(textsurf);

    textsurf = TTF_RenderText_Blended_Wrapped(roboto, "Maschinenkanone II\
                                              automatisch\
                                              \n\
                                              Reichweite: 210\
                                              Schaden:    10\
                                              Magazin:    80\
                                              Kadenz:     800 rpm\
                                              Nachladen:  0,75s\
                                              \n\
                                              Bauzeit:    4s\
                                              Baukosten:  60\
                                              \n\
                                              Die verbesserte Maschinenkanone schießt noch schneller und stärkere Projektile.",
                                              black, 246);
    target.x = 246;
    SDL_BlitSurface(textsurf, NULL, finalSurf, &target); SDL_FreeSurface(textsurf);

    textsurf = TTF_RenderText_Blended_Wrapped(roboto, "Gatling Kanone I\
                                              automatisch\
                                              \n\
                                              Reichweite: 210\
                                              Schaden:    7\
                                              Magazin:    160\
                                              Kadenz:     2400 rpm\
                                              Nachladen:  1,5s\
                                              \n\
                                              Bauzeit:    5s\
                                              Baukosten:  80\
                                              \n\
                                              Die Gatling Kanone ist durch ihre hohe Feuerrate eines der stärksten Geschütze.",
                                              black, 246);
    target.x = 492;
    SDL_BlitSurface(textsurf, NULL, finalSurf, &target); SDL_FreeSurface(textsurf);

    textsurf = TTF_RenderText_Blended_Wrapped(roboto, "Gatling Kanone II\
                                              automatisch\
                                              \n\
                                              Reichweite: 210\
                                              Schaden:    7\
                                              Magazin:    240\
                                              Kadenz:     4800 rpm\
                                              Nachladen:  1,5s\
                                              \n\
                                              Bauzeit:    6s\
                                              Baukosten:  170\
                                              \n\
                                              Die verbessert Gatling Kanone hat eine unübertroffene Feuerrate und kann auch die stärksten Gegner effektiv bekämpfen.",
                                              black, 246);
    target.x = 738;
    SDL_BlitSurface(textsurf, NULL, finalSurf, &target); SDL_FreeSurface(textsurf);


    target.y = 350 * GAUSS;
    textsurf = TTF_RenderText_Blended_Wrapped(roboto, "Gaussgewehr I\
                                              automatisch\
                                              \n\
                                              Reichweite: 175\
                                              Schaden:    25\
                                              Kadenz:     80 rpm\
                                              \n\
                                              Bauzeit:    4s\
                                              Baukosten:  70\
                                              \n\
                                              Das Gaussgewehr schießt Projektile mit sehr hoher Geschwindigkeit ab, sodass diese mehrere Gegner treffen können.",
                                              black, 246);
    target.x = 0;
    SDL_BlitSurface(textsurf, NULL, finalSurf, &target); SDL_FreeSurface(textsurf);

    textsurf = TTF_RenderText_Blended_Wrapped(roboto, "Gaussgewehr II\
                                              automatisch\
                                              \n\
                                              Reichweite: 210\
                                              Schaden:    35\
                                              Kadenz:     120 rpm\
                                              \n\
                                              Bauzeit:    4s\
                                              Baukosten:  70\
                                              \n\
                                              Das verbesserte Gaussgewehr schießt mehr Projektile und verursacht größeren Schaden.",
                                              black, 246);
    target.x = 246;
    SDL_BlitSurface(textsurf, NULL, finalSurf, &target); SDL_FreeSurface(textsurf);

    textsurf = TTF_RenderText_Blended_Wrapped(roboto, "Railgun I\
                                              automatisch\
                                              \n\
                                              Reichweite: 245\
                                              Schaden:    55\
                                              Kadenz:     120 rpm\
                                              \n\
                                              Bauzeit:    5s\
                                              Baukosten:  80\
                                              \n\
                                              Die Railgun kann schwerere Projektile noch weiter schießen.",
                                              black, 246);
    target.x = 492;
    SDL_BlitSurface(textsurf, NULL, finalSurf, &target); SDL_FreeSurface(textsurf);

    textsurf = TTF_RenderText_Blended_Wrapped(roboto, "Railgun II\
                                              automatisch\
                                              \n\
                                              Reichweite: 280\
                                              Schaden:    70\
                                              Kadenz:     160 rpm\
                                              \n\
                                              Bauzeit:    5s\
                                              Baukosten:  120\
                                              \n\
                                              Die verbesserte Railgun ist dafür gebaut, Gegnergruppen auch noch in großer Entfernung zu bekämpfen.",
                                              black, 246);
    target.x = 738;
    SDL_BlitSurface(textsurf, NULL, finalSurf, &target); SDL_FreeSurface(textsurf);


    target.y = 350 * LASER;
    textsurf = TTF_RenderText_Blended_Wrapped(roboto, "Laserkanone I\
                                              automatisch\
                                              \n\
                                              Reichweite: 175\
                                              Schaden:    200/s\
                                              Magazin:    0,125s\
                                              Nachladen:  0,75s\
                                              \n\
                                              Bauzeit:    5s\
                                              Baukosten:  60\
                                              \n\
                                              Der Laserpointer blendet den Killer.\
                                              Moment, falsches Spiel...\
                                              Der militärische Laser ist die stärkste verfügbare Waffentechnologie.",
                                              black, 246);
    target.x = 0;
    SDL_BlitSurface(textsurf, NULL, finalSurf, &target); SDL_FreeSurface(textsurf);

    textsurf = TTF_RenderText_Blended_Wrapped(roboto, "Laserkanone II\
                                              automatisch\
                                              \n\
                                              Reichweite: 175\
                                              Schaden:    240/s\
                                              Magazin:    0,25s\
                                              Nachladen:  0,75s\
                                              \n\
                                              Bauzeit:    6s\
                                              Baukosten:  70\
                                              \n\
                                              Durch verbesserte Kühlsysteme kann dieser Laser längere Impulse mit höherer Energie verschießen.",
                                              black, 246);
    target.x = 246;
    SDL_BlitSurface(textsurf, NULL, finalSurf, &target); SDL_FreeSurface(textsurf);

    textsurf = TTF_RenderText_Blended_Wrapped(roboto, "Laserkanone III\
                                              automatisch\
                                              \n\
                                              Reichweite: 200\
                                              Schaden:    400/s\
                                              Magazin:    0,125s\
                                              Nachladen:  0,125s\
                                              \n\
                                              Bauzeit:    7s\
                                              Baukosten:  120\
                                              \n\
                                              Dieser Laser kann mit deutlich mehr Energie schießen.",
                                              black, 246);
    target.x = 492;
    SDL_BlitSurface(textsurf, NULL, finalSurf, &target); SDL_FreeSurface(textsurf);

    textsurf = TTF_RenderText_Blended_Wrapped(roboto, "Laserkanone IV\
                                              automatisch\
                                              \n\
                                              Reichweite: 280\
                                              Schaden:    400/s\
                                              \n\
                                              Bauzeit:    7s\
                                              Baukosten:  220\
                                              \n\
                                              Die ultimative Waffe: Dieser Laser kann durchgehend feuern und trifft auch weit entfernte Gegner zuverlässig.",
                                              black, 246);
    target.x = 492;
    target.x = 738;
    SDL_BlitSurface(textsurf, NULL, finalSurf, &target); SDL_FreeSurface(textsurf);

    graphics->descriptions = SDL_CreateTextureFromSurface(graphics->renderer, finalSurf);
    SDL_FreeSurface(finalSurf);
    if (graphics->descriptions == NULL) {
        return -1;
    }

    ///buttons
    finalSurf = SDL_CreateRGBSurfaceWithFormat(0, 106, 55 * 10, 32, SDL_PIXELFORMAT_ARGB8888);

    target.x = 0;
    target.y = 55 * BUILD;
    textsurf = TTF_RenderText_Blended_Wrapped(roboto, "Bauen", black, 106);
    SDL_BlitSurface(textsurf, NULL, finalSurf, &target); SDL_FreeSurface(textsurf);
    target.y = 55 * CANCEL;
    textsurf = TTF_RenderText_Blended_Wrapped(roboto, "Abbrechen", black, 106);
    SDL_BlitSurface(textsurf, NULL, finalSurf, &target); SDL_FreeSurface(textsurf);
    target.y = 55 * SHOW_UPGRADE;
    textsurf = TTF_RenderText_Blended_Wrapped(roboto, "Ausbau zeigen", black, 106);
    SDL_BlitSurface(textsurf, NULL, finalSurf, &target); SDL_FreeSurface(textsurf);
    target.y = 55 * UPGRADE;
    textsurf = TTF_RenderText_Blended_Wrapped(roboto, "Ausbauen", black, 106);
    SDL_BlitSurface(textsurf, NULL, finalSurf, &target); SDL_FreeSurface(textsurf);
    target.y = 55 * CONSTRUCTION;
    textsurf = TTF_RenderText_Blended_Wrapped(roboto, "Im Bau", black, 106);
    SDL_BlitSurface(textsurf, NULL, finalSurf, &target); SDL_FreeSurface(textsurf);
    target.y = 55 * LEAVE;
    textsurf = TTF_RenderText_Blended_Wrapped(roboto, "Verlassen", black, 106);
    SDL_BlitSurface(textsurf, NULL, finalSurf, &target); SDL_FreeSurface(textsurf);
    target.y = 55 * MAN;
    textsurf = TTF_RenderText_Blended_Wrapped(roboto, "Besetzen", black, 106);
    SDL_BlitSurface(textsurf, NULL, finalSurf, &target); SDL_FreeSurface(textsurf);
    target.y = 55 * AUTO;
    textsurf = TTF_RenderText_Blended_Wrapped(roboto, "\nAutomatik", black, 106);
    SDL_BlitSurface(textsurf, NULL, finalSurf, &target); SDL_FreeSurface(textsurf);
    target.y = 55 * NO_STAFF;
    textsurf = TTF_RenderText_Blended_Wrapped(roboto, "\nKein freier Arbeiter!", black, 106);
    SDL_BlitSurface(textsurf, NULL, finalSurf, &target); SDL_FreeSurface(textsurf);
    target.y = 55 * NO_CREDIT;
    textsurf = TTF_RenderText_Blended_Wrapped(roboto, "\nNicht genug Baustoff!", black, 106);
    SDL_BlitSurface(textsurf, NULL, finalSurf, &target); SDL_FreeSurface(textsurf);

    graphics->buttons = SDL_CreateTextureFromSurface(graphics->renderer, finalSurf);
    SDL_FreeSurface(finalSurf);
    if (graphics->buttons == NULL) {
        return -1;
    }

    ///numbers
    textsurf = TTF_RenderText_Blended_Wrapped(roboto, "0123456789\
                                              Baustoff:", black, 70);
    graphics->numbers = SDL_CreateTextureFromSurface(graphics->renderer, textsurf);
    SDL_FreeSurface(textsurf);
    if (graphics->numbers == NULL) {
        return -1;
    }
    return 0;
}
