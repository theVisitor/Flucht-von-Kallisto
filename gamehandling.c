#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <time.h>

#include "declarations.h"

#define XA 0.195090322
#define YA 0.9807852804
#define XB 0.3826834324
#define YB 0.9238795325
#define XC 0.555570233
#define YC 0.8314696123
#define XD 0.7071067812
#define YD 0.7071067812

const int fastFPSwait = (25 * CLOCKS_PER_SEC) / 1000;
const int fastFPSwait_SDL = (23 * CLOCKS_PER_SEC) / 1000;

const SDL_Rect mapRec = {.x = 0, .y = 0, .w = 1000, .h = 720};
const SDL_Rect mapInnerRec = {.x = 10, .y = 10, .w = 980, .h = 700};
const SDL_Rect guiRec = {.x = 1000, .y = 0, .w = 280, .h = 720};
const SDL_Rect textRec = {.x = 1017, .y = 165, .w = 246, .h = 350};
const SDL_Rect leftButtonRec = {.x = 1010, .y = 535, .w = 120, .h = 55};
const SDL_Rect rightButtonRec = {.x = 1150, .y = 535, .w = 120, .h = 55};
const SDL_Rect leftButtonTXRec = {.x = 1017, .y = 535, .w = 106, .h = 55};
const SDL_Rect rightButtonTXRec = {.x = 1157, .y = 535, .w = 106, .h = 55};
const SDL_Rect factoryRec = {.x = 1010, .y = 610, .w = 120, .h = 55};
const SDL_Rect missionRec = {.x = 1150, .y = 610, .w = 120, .h = 55};
const SDL_Rect fullscreenRec = {.x = 1255, .y = 2, .w = 20, .h = 20};
const SDL_Rect speedRec = {.x = 2, .y = 2, .w = 20, .h = 20};

const SDL_Rect pistolRec = {.x = 1005, .y = 24, .w = 66, .h = 66};
const SDL_Rect rifleRec = {.x = 1073, .y = 24, .w = 66, .h = 66};
const SDL_Rect shotgunRec = {.x = 1141, .y = 24, .w = 66, .h = 66};
const SDL_Rect mortarRec = {.x = 1209, .y = 24, .w = 66, .h = 66};
const SDL_Rect rocketRec = {.x = 1005, .y = 92, .w = 66, .h = 66};
const SDL_Rect cannonRec = {.x = 1073, .y = 92, .w = 66, .h = 66};
const SDL_Rect gaussRec = {.x = 1141, .y = 92, .w = 66, .h = 66};
const SDL_Rect laserRec = {.x = 1209, .y = 92, .w = 66, .h = 66};

/**
 * The main playing function, does everything in a game
 * and returns the points awarded (or -1 for exit)
 */
int play(SDL_Renderer *renderer, SDL_Window *window, int mapnumber, int difficulty) {
    ///initialise stuff
    GameObject game = {
        .enemies = NULL,
        .turrets = NULL,
        .fireObjects = NULL,
        .explosions = NULL,

        .factoryStaff = 0,
        .priority = 0, //mission
        .missionProgress = 0,
        .wavetime = 1100,
        .speed = 1,
        .frame = 1,
        .spawntimes = malloc(500 * sizeof(int)),
        .credit = 50,
        .totalcredit = 0};
    if (game.spawntimes == NULL) {
        return -2;
    }
    game.spawntimes[0] = 1;
    game.spawntimes[1] = 0;

    Graphics graphics = {.renderer = renderer};
    Audio audio;
    Field map[560];

    ///loading from files
    printf("init\n");
    if (loadTextures(&graphics, mapnumber)) {
        printf("tx fail\n");
        return -2;
    } else {
        printf("tx success\n");
    }

	if (loadAudio(&audio))
	{
		printf("audio fail\n");
		return -2;
	}
	else
	{
		printf("audio success\n");
	}

    if (loadMap(&game, map, mapnumber, difficulty)) {
        printf("map fail\n");
        cleanupAll(&game);
        unloadTextures(&graphics);
        return -2;
    }
    printf("map success\n");

    ///finding the base
    int basesearch = 0;
    while (map[basesearch].type != 3) {basesearch++;}
    graphics.baseRec.x = (basesearch % 28) * 35 + 10;
    graphics.baseRec.y = (basesearch / 28) * 35 + 10;
    graphics.baseRec.w = 140;
    graphics.baseRec.h = 70;

    ///the state:
    ///0 = nothing, 1 = selected, 2 = upgrade selected, 3 = new turret stats, 4 = new turret build
    int state = 0;
    Turret *selected = NULL;
    turretType building;

    ///the main loop for playing, limited to 40fps
    SDL_Event event;
    SDL_Point click;
    int playing = 1;
    clock_t framestart;

    while (playing && game.missionStaff > 0 && game.missionProgress < game.missionEnd) {
        framestart = clock();
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)) {
                playing = 0;
            } else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                click.x = event.button.x; click.y = event.button.y;
                if (SDL_PointInRect(&click, &fullscreenRec)) {
                    if (SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN) {
                        SDL_SetWindowFullscreen(window, 0);
                    } else {
                        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
                    }
                }
                handleClick(&game, &click, &state, &selected, &building, map);
            } else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_RIGHT) {
                click.x = event.button.x; click.y = event.button.y;
                handleRightClick(&game, &click);
            }
        }
        cycle(&graphics, &game, state, selected, building, map, &audio);  /// <- the actual game calculations
        SDL_RenderPresent(graphics.renderer);
        if (clock() < framestart + fastFPSwait_SDL) {
            ///SDL_Delay is better for CPU
            SDL_Delay(framestart + fastFPSwait_SDL - clock());
        }
        ///Loop is more precise
        while (clock() < framestart + fastFPSwait) {}
    }

    ///counting the survivors, cleaning up
    int score = countPersons(&game);
    cleanupAll(&game);
    unloadTextures(&graphics);
    if (playing) {
        return score;
    } else {
        return -1;
    }
}

/**
 * Function for handling click-events during the game (left click)
 */
void handleClick(GameObject *game, SDL_Point *click, int *state, Turret **selected, turretType *building, Field map[560]) {
    ///menu-click?
    if (SDL_PointInRect(click, &pistolRec)) {
        *state = 3;
        *building = PISTOL;
    } else if (SDL_PointInRect(click, &rifleRec)) {
        *state = 3;
        *building = RIFLE;
    } else if (SDL_PointInRect(click, &shotgunRec)) {
        *state = 3;
        *building = SHOTGUN;
    } else if (SDL_PointInRect(click, &mortarRec)) {
        *state = 3;
        *building = MORTAR;
    } else if (SDL_PointInRect(click, &rocketRec)) {
        *state = 3;
        *building = ROCKET;
    } else if (SDL_PointInRect(click, &cannonRec)) {
        *state = 3;
        *building = CANNON;
    } else if (SDL_PointInRect(click, &gaussRec)) {
        *state = 3;
        *building = GAUSS;
    } else if (SDL_PointInRect(click, &laserRec)) {
        *state = 3;
        *building = LASER;

    ///speed click?
    } else if (*state != 4 && SDL_PointInRect(click, &speedRec)) {
        game->speed *= 2;
        if (game->speed > 4) {
            game->speed = 1;
        }

    ///map click?
    } else if (SDL_PointInRect(click, &mapInnerRec)) {
        int index = (click->x - 10) / 35 + ((click->y - 10) / 35) * 28;
        if (map[index].type == 2) {
            ///turret selected
            *selected = map[index].turret;
            *state = 1;
        } else if (*state == 4) {
            ///turret building
            if (map[index].type == 0 && !getPerson(game, 0)) {
                if (!buildTurret(game, ((click->x - 10) / 35) * 35 + 27, ((click->y - 10) / 35) * 35 + 27, *building)) {
                    *state = 1;
                    map[index].type = 2;
                    map[index].turret = game->turrets;
                    *selected = game->turrets;
                }
            } else {
                *state = 3;
            }
        } else {
            *state = 0;
        }
    ///left gui-button?
    } else if (SDL_PointInRect(click, &leftButtonRec) && (*state == 1 || *state == 2)) {
        if ((*selected)->buildStatus || !turretData[(*selected)->type][(*selected)->level].isAuto) {
            if ((*selected)->isManned) {
                (*selected)->isManned = 0;
                if (game->priority) {
                    game->factoryStaff++;
                } else {
                    game->missionStaff++;
                }
            } else if (!getPerson(game, 0)){
                (*selected)->isManned = 1;
            }
        }
    ///right gui-button?
    } else if (SDL_PointInRect(click, &rightButtonRec)) {
        if (*state == 1 && (*selected)->level < 3 && (*selected)->buildStatus == 0) {
            *state = 2;
        } else if (*state == 2) {
            ///upgrading
            if (game->credit >= turretData[(*selected)->type][(*selected)->level + 1].buildCost) {
                if (((*selected)->isManned && !turretData[(*selected)->type][(*selected)->level].isAuto) || !getPerson(game, 0)) {
                    upgradeTurret(game, *selected);
                    *state = 1;
                }
            }
        } else if (*state == 3 && game->credit >= turretData[*building][0].buildCost && (game->factoryStaff > 0 || game->missionStaff > 1)) {
            *state = 4;
        } else if (*state == 4) {
            *state = 3;
        }
    ///staff?
    } else if (SDL_PointInRect(click, &factoryRec) && game->missionStaff > 1) {
        game->missionStaff--;
        game->factoryStaff++;
    } else if (SDL_PointInRect(click, &missionRec) && game->factoryStaff > 0) {
        game->missionStaff++;
        game->factoryStaff--;
    }
}

/**
 * Function for handling click-events during the game (left click)
 */
void handleRightClick(GameObject *game, SDL_Point *click) {
    ///speed click?
    if (SDL_PointInRect(click, &speedRec)) {
        if (game->speed > 1) {
            game->speed >>= 1;
        }
    ///staff?
    } else if (SDL_PointInRect(click, &factoryRec)) {
        changePriority(game, 1);
    } else if (SDL_PointInRect(click, &missionRec)) {
        changePriority(game, 0);
    }
}

/**
 * Function for updating and rendering the frame
 */
int cycle(Graphics *graphics, GameObject *game, int state, Turret *selected, turretType building, Field map[], Audio *audio) {
    game->frame = 1;
    while(game->frame < game->speed) {
        ///only updates, no render
        ///update factory, mission
        game->missionProgress += game->missionStaff;
        double production = game->factoryStaff * ((game->totalcredit + 500.0)/ 100000.0);
        game->credit += production;
        game->totalcredit += production;

        ///update enemies, turrets, stuff like that
        doWaves(game);
        doEnemies(graphics, game);
        doTurrets(graphics, game, audio);
        doFireObjects(graphics, game);
        doExplosions(graphics, game);
        game->frame++;
    }
    ///update factory, mission
    game->missionProgress += game->missionStaff;
    double production = game->factoryStaff * ((game->totalcredit + 500.0)/ 100000.0);
    game->credit += production;
    game->totalcredit += production;

    SDL_RenderCopy(graphics->renderer, graphics->background, NULL, &mapRec);

    ///update enemies, turrets, stuff like that
    doWaves(game);
    doEnemies(graphics, game);
    doTurrets(graphics, game, audio);
    doFireObjects(graphics, game);
    doExplosions(graphics, game);

    ///render base
    SDL_Rect sRect = {.x = 0, .w = 140, .h = 70, .y = ((5 * game->missionProgress) / game->missionEnd) * 70};
    SDL_RenderCopy(graphics->renderer, graphics->base, &sRect, &(graphics->baseRec));

    ///render user dependent stuff over the map
    switch (state) {
    case 2:
        renderCircle(graphics, selected->x, selected->y, turretData[selected->type][selected->level + 1].range);
    case 1:
        showTurretSelection(graphics, selected);
        break;
    case 4:
        showNewTurret(graphics, building, map);
    }

    ///render GUI
    SDL_RenderCopy(graphics->renderer, graphics->gui, NULL, &guiRec);
    switch (state) {
    case 1:
        showTurretSelectionDescription(graphics, game, 0, selected); break;
    case 2:
        showTurretSelectionDescription(graphics, game, 1, selected); break;
    case 3:
        showNewTurretDescription(graphics, game, 0, building); break;
    case 4:
        showNewTurretDescription(graphics, game, 1, building); break;
    }
    if (game->priority) {
        SDL_RenderCopy(graphics->renderer, graphics->selection, NULL, &factoryRec);
    } else {
        SDL_RenderCopy(graphics->renderer, graphics->selection, NULL, &missionRec);
    }
    renderSpeed(graphics, game->speed);
    renderNumbers(graphics, game);
    return 0;
}

/**
 * Function for rendering the description of a
 * selected turret and the correct button-labels
 * on top of the GUI base-texture
 */
void showTurretSelectionDescription(Graphics *graphics, GameObject *game, int upgrading, Turret *turret) {
    SDL_Rect sTextRec = {.x = (turret->level + upgrading) * 246, .y = turret->type * 350, .w = 246, .h = 350};
    SDL_Rect sButtonRec = {.x = 0, .w = 120, .h = 55};
    SDL_RenderCopy(graphics->renderer, graphics->descriptions, &sTextRec, &textRec);
    if (!turretData[turret->type][turret->level].isAuto || turret->buildStatus) {
        if (turret->isManned) {
            sButtonRec.y = LEAVE * 55;
            SDL_RenderCopy(graphics->renderer, graphics->buttons, &sButtonRec, &leftButtonTXRec);
        } else {
            sButtonRec.y = MAN * 55;
            SDL_RenderCopy(graphics->renderer, graphics->buttons, &sButtonRec, &leftButtonTXRec);
            if (game->factoryStaff == 0 && game->missionStaff <= 1) {
                sButtonRec.y = NO_STAFF * 55;
                SDL_RenderCopy(graphics->renderer, graphics->buttons, &sButtonRec, &leftButtonTXRec);
            }
        }
    } else {
        sButtonRec.y = AUTO * 55;
        SDL_RenderCopy(graphics->renderer, graphics->buttons, &sButtonRec, &leftButtonTXRec);
    }
    if (turret->buildStatus) {
        sButtonRec.y = CONSTRUCTION * 55;
        SDL_RenderCopy(graphics->renderer, graphics->buttons, &sButtonRec, &rightButtonTXRec);
    } else if (turret->level < 3) {
        if (!upgrading) {
            sButtonRec.y = SHOW_UPGRADE * 55;
            SDL_RenderCopy(graphics->renderer, graphics->buttons, &sButtonRec, &rightButtonTXRec);
        } else {
            sButtonRec.y = UPGRADE * 55;
            SDL_RenderCopy(graphics->renderer, graphics->buttons, &sButtonRec, &rightButtonTXRec);
            if (game->credit < turretData[turret->type][turret->level + 1].buildCost) {
                sButtonRec.y = NO_CREDIT * 55;
                SDL_RenderCopy(graphics->renderer, graphics->buttons, &sButtonRec, &rightButtonTXRec);
            } else if (game->factoryStaff == 0 && game->missionStaff <= 1) {
                sButtonRec.y = NO_STAFF * 55;
                SDL_RenderCopy(graphics->renderer, graphics->buttons, &sButtonRec, &rightButtonTXRec);
            }
        }
    }
}

/**
 * Function for rendering the description of a new turret
 * (menu - selection) and the correct button-labels
 * on top of the GUI base-texture
 */
void showNewTurretDescription(Graphics *graphics, GameObject *game, int build, turretType type) {
    SDL_Rect sTextRec = {.x = 0, .y = type * 350, .w = 246, .h = 350};
    SDL_Rect sButtonRec = {.x = 0, .w = 120, .h = 55};
    SDL_RenderCopy(graphics->renderer, graphics->descriptions, &sTextRec, &textRec);
    if (!build) {
        sButtonRec.y = BUILD * 55;
        SDL_RenderCopy(graphics->renderer, graphics->buttons, &sButtonRec, &rightButtonTXRec);
        if (game->credit < turretData[type][0].buildCost) {
            sButtonRec.y = NO_CREDIT * 55;
            SDL_RenderCopy(graphics->renderer, graphics->buttons, &sButtonRec, &rightButtonTXRec);
        } else if (game->factoryStaff == 0 && game->missionStaff <= 1) {
            sButtonRec.y = NO_STAFF * 55;
            SDL_RenderCopy(graphics->renderer, graphics->buttons, &sButtonRec, &rightButtonTXRec);
        }
    } else {
        sButtonRec.y = CANCEL * 55;
        SDL_RenderCopy(graphics->renderer, graphics->buttons, &sButtonRec, &rightButtonTXRec);
    }
    SDL_Rect dSelecRec = {.w = 66, .h = 66};
    if (type < 4) {
        dSelecRec.x = 1005 + 68 * type;
        dSelecRec.y = 24;
    } else {
        dSelecRec.x = 1005 + 68 * (type - 4);
        dSelecRec.y = 92;
    }
    SDL_RenderCopy(graphics->renderer, graphics->selection, NULL, &dSelecRec);
}

/**
 * Function for rendering the selection marker on a turret
 * as well as a range indicating circle around it
 */
void showTurretSelection(Graphics *graphics, Turret *turret) {
    SDL_Rect dRect = {.x = turret->x - 17, .y = turret->y - 17, .w = 35, .h = 35};
    SDL_RenderCopy(graphics->renderer, graphics->selection, NULL, &dRect);
    renderCircle(graphics, turret->x, turret->y, turretData[turret->type][turret->level].range);
}

/**
 * Function for rendering a new turret to the mouse position
 * (moved to the next grid position) as well as a
 * range indicating circle around it
 */
void showNewTurret(Graphics *graphics, turretType building, Field map[]) {
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    if (mouseX > 9 && mouseX < 990 && mouseY > 9 && mouseY < 710) {
        ///get grid coords
        mouseX = (mouseX - 10) / 35;
        mouseY = (mouseY - 10) / 35;
        if (map[28 * mouseY + mouseX].type == 0) {
            ///back to pixel coords
            mouseX = 35 * mouseX + 10;
            mouseY = 35 * mouseY + 10;
            SDL_Rect dRect = {.x = mouseX, .y = mouseY, .w = 35, .h = 35};
            SDL_RenderCopy(graphics->renderer, graphics->turretbase, NULL, &dRect);
            renderCircle(graphics, mouseX + 17, mouseY + 17, turretData[building][0].range);
        }
    }
}

/**
 * Function for rendering a circle
 * (approximated by a 32-gon)
 */
void renderCircle(Graphics *graphics, int x, int y, int r) {
    SDL_Point circ[] = {
        {.x = x, .y = y + r}, {.x = x + XA*r, .y = y + YA*r}, {.x = x + XB*r, .y = y + YB*r}, {.x = x + XC*r, .y = y + YC*r},
        {.x = x + XD*r, .y = y + YD*r}, {.x = x + YC*r, .y = y + XC*r}, {.x = x + YB*r, .y = y + XB*r}, {.x = x + YA*r, .y = y + XA*r},

        {.x = x + r, .y = y}, {.x = x + YA*r, .y = y - XA*r}, {.x = x + YB*r, .y = y - XB*r}, {.x = x + YC*r, .y = y - XC*r},
        {.x = x + XD*r, .y = y - YD*r}, {.x = x + XC*r, .y = y - YC*r}, {.x = x + XB*r, .y = y - YB*r}, {.x = x + XA*r, .y = y - YA*r},

        {.x = x, .y = y - r}, {.x = x - XA*r, .y = y - YA*r}, {.x = x - XB*r, .y = y - YB*r}, {.x = x - XC*r, .y = y - YC*r},
        {.x = x - XD*r, .y = y - YD*r}, {.x = x - YC*r, .y = y - XC*r}, {.x = x - YB*r, .y = y - XB*r}, {.x = x - YA*r, .y = y - XA*r},

        {.x = x - r, .y = y}, {.x = x - YA*r, .y = y + XA*r}, {.x = x - YB*r, .y = y + XB*r}, {.x = x - YC*r, .y = y + XC*r},
        {.x = x - XD*r, .y = y + YD*r}, {.x = x - XC*r, .y = y + YC*r}, {.x = x - XB*r, .y = y + YB*r}, {.x = x - XA*r, .y = y + YA*r},

        {.x = x, .y = y + r},
    };
    SDL_SetRenderDrawColor(graphics->renderer, 255, 255, 255, 255);
    SDL_RenderDrawLines(graphics->renderer, circ, 33);
}

void renderSpeed(Graphics *graphics, int speed) {
    SDL_Rect sRect = {.x = 0, .y = 0, .w = 20, .h = 20};
    switch (speed) {
        case 2: sRect.x = 20; break;
        case 4: sRect.x = 40; break;
    }
    SDL_RenderCopy(graphics->renderer, graphics->speedbutton, &sRect, &speedRec);
}

/**
 * Function for rendering the important numbers
 *
 * *graphics: The used graphics
 * *game: The active GameObject
 */
void renderNumbers(Graphics *graphics, GameObject *game) {
    ///credits
    SDL_Rect dRect = {.x = 1005, .y = 4, .w = 63, .h = 17};
    SDL_Rect sRect = {.x = 0, .y = 17, .w = 63, .h = 17};
    SDL_RenderCopy(graphics->renderer, graphics->numbers, &sRect, &dRect);
    dRect.x = 1075;
    dRect.w = 7;
    renderNumber(graphics, (int) game->credit, &dRect);
    ///progress
    SDL_Rect progressBar = {.x = 1010, .y = 679, .w = (260 * game->missionProgress) / game->missionEnd, .h = 20};
    SDL_SetRenderDrawColor(graphics->renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(graphics->renderer, &progressBar);

    dRect.x = 1025;
    dRect.y = 680;
    int progress = (100 * game->missionProgress) / game->missionEnd;
    renderNumber(graphics, progress, &dRect);
    ///staff
    dRect.x = 1064;
    dRect.y = 632;
    renderNumber(graphics, game->factoryStaff, &dRect);
    dRect.x = 1203;
    renderNumber(graphics, game->missionStaff, &dRect);
}

/**
 * Function for rendering a single
 * positive number to the given position
 */
void renderNumber(Graphics *graphics, int number, SDL_Rect *dRect) {
    SDL_Rect sRect = {.x = 0, .y = 0, .w = 7, .h = 17};
    if (number == 0) {
        SDL_RenderCopy(graphics->renderer, graphics->numbers, &sRect, dRect);
        return;
    }
    int length = ceil(log10(number + 1)); ///how many digits?
    int power = pow(10, length) + 0.5;
    int display; ///active digit
    while (length) {
        number = number % power;
        power /= 10;
        length--;
        display = number / power;
        sRect.x = 7 * display;
        SDL_RenderCopy(graphics->renderer, graphics->numbers, &sRect, dRect);
        dRect->x += dRect->w;
    }
}

/**
 * Function for changing the priority of workers,
 * also moves the free workers
 */
void changePriority(GameObject *game, int newpriority) {
    game->priority = newpriority;
    if (newpriority) {
        while (game->missionStaff > 1) {
            game->missionStaff--;
            game->factoryStaff++;
        }
    } else {
        while (game->factoryStaff) {
            game->factoryStaff--;
            game->missionStaff++;
        }
    }
}

/**
 * Function for getting a person from the facility
 * returns 0 if a person was retrieved, -1 otherwise
 */
int getPerson(GameObject *game, int force) {
    if (game->priority) {
        if (game->missionStaff > 1) {
            game->missionStaff--;
            return 0;
        }
        if (game->factoryStaff) {
            game->factoryStaff--;
            return 0;
        }
    } else {
        if (game->factoryStaff) {
            game->factoryStaff--;
            return 0;
        }
        if (game->missionStaff > 1) {
            game->missionStaff--;
            return 0;
        }
    }

    if (force) {
        ///try turrets, without those in building process
        for (Turret *turret = game->turrets; turret != NULL; turret = turret->next) {
            if (turret->isManned && !turretData[turret->type][turret->level].isAuto && !turret->buildStatus) {
                turret->isManned = 0;
                return 0;
            }
        }
        ///try turrets in building process
        for (Turret *turret = game->turrets; turret != NULL; turret = turret->next) {
            if (turret->isManned && turret->buildStatus) {
                turret->isManned = 0;
                return 0;
            }
        }
        if (game->missionStaff) {
            game->missionStaff = 0;
            return 0;
        }
    }
    return -1;
}

/**
 * Function that returns how many people are alive
 * Removes all people!
 */
int countPersons(GameObject *game) {
    int survivors = 0;
    while (!getPerson(game, 1)) {
        survivors++;
    }
    return survivors;
}

/**
 * Function that cleans up non-texture ressources
 * used inside a level
 */
void cleanupAll(GameObject *game) {
    cleanupWaves(game);
    clenupPaths(game);
    cleanupEnemies(game);
    cleanupTurrets(game);
    cleanupFOs(game);
    cleanupExplosives(game);
    free(game->spawntimes);
}
