#ifndef ENEMY_H_INCLUDED
#define ENEMY_H_INCLUDED

enum direction_enum {EAST, SOUTH, WEST, NORTH};

/**
 * The struct that describes where enemies
 * spawn and how they should move
 */
struct Path_struct {
	int startcount;			///how many starts
	PathStart *starts;		///pointer to them
	PathElement *arrayStart;///reference to the first PathElement
};

/**
 * Struct describing a spawnpoint
 */
struct PathStart_struct {
    int x;                  ///horizontal position
    int y;                  ///vertical position
	int remain;				///distance to the base
	PathElement *elem;		///first PathElement for this route
};

/**
 * The base struct for paths, representing
 * a straight part of the path between two corners
 */
struct PathElement_struct {
    direction dir;          ///direction of element
    int length;             ///the length of this part
    PathElement *next;      ///reference to the next part
};

/**
 * A struct containing information about an enemy.
 * Enemies are organized in a L-List, where the first
 * one is the one closest to the base.
 */
struct Enemy_struct {
    int type;               ///type of enemy
    int health;             ///healthpoints left
    int x;                  ///horizontal position
    int y;                  ///vertical position
    int dist;               ///distance travelled since last corner
    PathElement *current;   ///refernce to the PathElement this follower is on
    int distToBase;         ///remaining distance to the end of the path
    int anilife;            ///counter for the animation
    Enemy *next;            ///because it is a L-List
};

/**
 * Struct describing a wave
 */
struct Wave_struct {
    int type;               ///type of enemy to spawn
    int health;             ///initial health
    int amount;             ///how many to spawn
    Wave *next;             ///because it is a L-List
};

int spawnEnemy(GameObject *game);
void doEnemies(Graphics *graphics, GameObject *game);
void renderEnemies(Graphics *graphics, GameObject *game);
int moveEnemy(Enemy *enemy, int distance);
void updateEnemies(GameObject *game);
void sortEnemies(GameObject *game);
void doWaves(GameObject *game);
void selectTimes(int *array, int positions, int start, int end);
void cleanupEnemies(GameObject *game);
void cleanupWaves(GameObject *game);
void clenupPaths(GameObject *game);

#endif // ENEMY_H_INCLUDED
