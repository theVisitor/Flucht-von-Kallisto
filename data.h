#ifndef DATA_H_INCLUDED
#define DATA_H_INCLUDED

#include <SDL2/SDL.h>

enum button_enum {BUILD, CANCEL, SHOW_UPGRADE, UPGRADE, CONSTRUCTION, LEAVE, MAN, AUTO, NO_STAFF, NO_CREDIT};

/**
 * A struct containing the data for a turret
 * fixed type, level
 */
struct TurretData_struct {
    int range;              ///range in px
    int reload;             ///frames to reload
    int magazine;           ///how many shots per magazine
    int cycleTime;          ///frames per shooting cycle (inverse of firing rate)
    int buildTime;          ///frames it takes to build
    int buildCost;          ///costs of building or upgrading
    int isAuto;             ///1 for automatic turrets, 0 otherwise
};

struct FireObjectData_struct {
    int damage;             ///damage done by a hit
    int speed;              ///travel speed in px/frame
    int radius;             ///effective radius of bullet OR radius of explosion
    int maxLife;            ///how many frames the object can live (caps range), 0 for missiles
    SDL_Rect rect;          ///where the texture is stored
};

struct EnemyData_struct {
    int radius;             ///radius of hit-circle
    int speed;              ///travel speed in px/frame
    int aniCycleFrames;     ///frames per anicycle
    int aniCycleStates;     ///amount of anistates
    SDL_Rect rect;          ///where the texture is stored (left ani)
};

TurretData turretData[8][4];
FireObjectData fOData[8][4];
EnemyData enemyData[3];

int loadTextures(Graphics *graphics, int level);
void unloadTextures(Graphics *graphics);
int createText(Graphics *graphics);

#endif // DATA_H_INCLUDED
