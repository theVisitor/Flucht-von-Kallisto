#ifndef GAMEHANDLING_H_INCLUDED
#define GAMEHANDLING_H_INCLUDED

#include <SDL2/SDL.h>

/**
 * Struct containing (mostly) textures used in a level
 */
struct Graphics_struct {
    SDL_Texture *turretbase;
    SDL_Texture *turrethead;
    SDL_Texture *construction;
    SDL_Texture *fireObjectTX;
    SDL_Texture *explosionTX;
    SDL_Texture *enemyTX;

    SDL_Texture *background;
    SDL_Texture *gui;
    SDL_Texture *selection;
    SDL_Texture *descriptions;
    SDL_Texture *buttons;
    SDL_Texture *numbers;
    SDL_Texture *base;
    SDL_Texture *speedbutton;

    SDL_Rect baseRec;
    SDL_Renderer *renderer;
};

/**
 * Struct describing a game
 */
struct GameObject_struct {
    Path *path;             ///the path where enemies walk
    Wave *waves;            ///the list of waves
    Enemy *enemies;         ///the list of enemies
    Turret *turrets;        ///the list of turrets currently on the map
    FireObject *fireObjects;///the list of fireobjects currently on the map
    Explosion *explosions;  ///the list of explosions in the current frame

    int missionStaff;       ///people working for the mission
    int factoryStaff;       ///people producing credits in the factory
    int priority;           ///put to factory or mission?
    int missionProgress;    ///how far the mission has proceeded
    int missionEnd;         ///when the mission will end
    int wavetime;           ///frames passed since start of this wave
    int speed;              ///speed of the game: 1 = normal, 2 = double, 4 = quad
    int frame;              ///which physicsframe in a renderframe
    int *spawntimes;        ///array saving when to spawn an enemy in this wave
    double credit;          ///how much credit the player has
    double totalcredit;     ///how much credit the factory produced since levelstart
};

/**
 * Struct describing a single tile of the map
 */
struct Field_struct {
    int type;               ///0 = empty, 1 = path, 2 = turret, 3 = base
    Turret *turret;         ///reference to the turret on this tile (NULL if there is none)
};

int play(SDL_Renderer *renderer, SDL_Window *window, int mapnumber, int difficulty);
void handleClick(GameObject *game, SDL_Point *click, int *state, Turret **selected, turretType *building, Field map[560]);
int cycle(Graphics *graphics, GameObject *game, int state, Turret *selected, turretType building, Field map[]);
void showTurretSelectionDescription(Graphics *graphics, GameObject *game, int upgrading, Turret *turret);
void showNewTurretDescription(Graphics *graphics, GameObject *game, int build, turretType type);
void showTurretSelection(Graphics *graphics, Turret *turret);
void showNewTurret(Graphics *graphics, turretType building, Field map[]);
void renderCircle(Graphics *graphics, int x, int y, int r);
void renderSpeed(Graphics *graphics, int speed);
void renderNumbers(Graphics *graphics, GameObject *game);
void renderNumber(Graphics *graphics, int number, SDL_Rect *dRect);
int getPerson(GameObject *game, int force);
int countPersons(GameObject *game);
void cleanupAll(GameObject *game);

#endif // GAMEHANDLING_H_INCLUDED
