#ifndef DECLARATIONS_H_INCLUDED
#define DECLARATIONS_H_INCLUDED

#define SAVESIZE 10
#define SCORESIZE 25
#define MAPSIZE 560
#define SALT "5D168E49078002F6A86E869C7DCF601A7B6689681C7D475161CA0A6D3925C5D85B3A2C8FED825694270A242F4A1725B1D6C4DA11E1642046B00ED9F0798E6924"
#define SALTSIZE 129

typedef struct Enemy_struct Enemy;
typedef enum direction_enum direction;
typedef struct Path_struct Path;
typedef struct PathStart_struct PathStart;
typedef struct PathElement_struct PathElement;
typedef struct Wave_struct Wave;

typedef enum turretType_enum turretType;
typedef struct Turret_struct Turret;
typedef struct FireObject_struct FireObject;
typedef struct Explosion_struct Explosion;

typedef struct TurretData_struct TurretData;
typedef struct FireObjectData_struct FireObjectData;
typedef struct EnemyData_struct EnemyData;

typedef struct Graphics_struct Graphics;
typedef struct Audio_struct Audio;
typedef struct GameObject_struct GameObject;
typedef struct Field_struct Field;

#include "gamehandling.h"
#include "enemy.h"
#include "turret.h"
#include "data.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

const SDL_Rect fullscreenRec;

#endif // DECLARATIONS_H_INCLUDED
