#ifndef TURRET_H_INCLUDED
#define TURRET_H_INCLUDED

enum turretType_enum {PISTOL, RIFLE, SHOTGUN, MORTAR, ROCKET, CANNON, GAUSS, LASER};

/**
 * A struct containing information about a turret
 */
struct Turret_struct {
    turretType type;        ///type of turret
    int level;              ///level of turret
	int isManned;			///0 if not manned, 1 if manned or automatic
    int buildStatus;        ///0 if build, otherwise frames left to build
    int x;                  ///horizontal position
    int y;                  ///vertical position
    int cooldown;           ///frames remaining to next shot (negative, positive while firing)
	double rotation;		///direction of last shot - used for rendering
    Turret *next;           ///because it is a L-List
};

/**
 * A struct containing information about a FireObject (projectile, missile)
 */
struct FireObject_struct {
    turretType type;        ///who fired
    int level;              ///level of who fired
    double x;               ///horizontal position
    double y;               ///vertical position
    double dx;              ///horizontal speed (px/frame)
    double dy;              ///vertical speed (px/frame)
    int lifetime;           ///frames left until destruction / explosion
    FireObject *next;       ///because it is a L-List
};

/**
 * A struct describing an explosion
 */
struct Explosion_struct {
    double x;               ///horizontal position
    double y;               ///vertical position
    int radius;             ///radius of explosion
    int damage;             ///damage in the center
    Explosion *next;        ///because it is a L-List
};

int buildTurret(GameObject *game, int x, int y, turretType type);
void upgradeTurret(GameObject *game, Turret *turret);
void doTurrets(Graphics *graphics, GameObject *game, Audio *audio);
void renderTurretBases(Graphics *graphics, GameObject *game);
void renderTurretHeads(Graphics *graphics, GameObject *game);
void updateTurrets(Graphics *graphics, GameObject *game, Audio *audio);
int fire(Graphics *graphics, GameObject *game, Turret *turret, Enemy *enemy, Audio *audio);
int fireLaser(Graphics *graphics, Turret *turret, Enemy *enemy);
int fireInDirection(GameObject *game, Turret *turret, Enemy *enemy, double sectorStart, double sectorEnd, double advance);
void transformVector(double *x, double *y, double alpha, double length);
int fireAtEnemy(GameObject *game, Turret *turret, Enemy *enemy);
int generateFO(GameObject *game, double x, double y, double dx, double dy, turretType type, int level, int lifetime);
void doFireObjects(Graphics *graphics, GameObject *game);
void renderFireObjects(Graphics *graphics, GameObject *game);
void destroyDeadFireObjects(GameObject *game);
void updateFireObjects(GameObject *game);
void doExplosions(Graphics *graphics, GameObject *game);
void cleanupTurrets(GameObject *game);
void cleanupFOs(GameObject *game);
void cleanupExplosives(GameObject *game);

#endif // TURRET_H_INCLUDED
