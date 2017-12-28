#include <stdio.h>
#include <stdlib.h>

#include <math.h>
#include <SDL2/SDL.h>

#include "declarations.h"


#define SQUARE(x) ((x) * (x))
#define SCALARPRODUCT(x1, y1, x2, y2) ((x1) * (x2) + (y1) * (y2))
#define SAMESIGN(a, b) (((a)<=0 && (b) <= 0) || ((a)>=0 && (b)>=0))
#define MIN(a,b) ((a)<=(b) ? (a) : (b))


/**
 * Function for building a new turret
 */
int buildTurret(GameObject *game, int x, int y, turretType type) {
    ///allocate memory, return -1 if it fails
    Turret *newTurret = NULL;
    newTurret = malloc(sizeof(Turret));
    if (newTurret == NULL) {
        printf("A pointer is Null where it should not be!\n(newTurret)\n");
        return -1;
    }

    ///pay
    game->credit -= turretData[type][0].buildCost;

    ///initialize values
    newTurret->type = type;
    newTurret->level = 0;
    newTurret->isManned = 1;
    newTurret->buildStatus = turretData[type][0].buildTime;
    newTurret->x = x;
    newTurret->y = y;
    newTurret->cooldown = -10;
    newTurret->rotation = 0.0;
    newTurret->next = game->turrets;

    ///update the list
    game->turrets = newTurret;
    return 0;
}

/**
 * Function for upgrading a turret
 */
void upgradeTurret(GameObject *game, Turret *turret) {
    turret->level++;
    game->credit -= turretData[turret->type][turret->level].buildCost;
    turret->isManned = 1;
    turret->buildStatus = turretData[turret->type][turret->level].buildTime;
    turret->cooldown = -10;
}

/**
 * Function for updating and rendering turrets for one frame
 */
void doTurrets(Graphics *graphics, GameObject *game) {
    if(game->frame == game->speed) {
        renderTurretBases(graphics, game);
        updateTurrets(graphics, game);
        renderTurretHeads(graphics, game);
    } else {
        updateTurrets(graphics, game);
    }
}

/**
 * Function for rendering the turret bases
 */
void renderTurretBases(Graphics *graphics, GameObject *game) {
    SDL_Rect dRect = {.w = 35, .h = 35};
    for (Turret *turret = game->turrets; turret != NULL; turret = turret->next) {
        dRect.x = turret->x - 17;
        dRect.y = turret->y - 17;
        SDL_RenderCopy(graphics->renderer, graphics->turretbase, NULL, &dRect);
    }
}

/**
 * Function for rendering the turret heads
 * or maybe a construction symbol
 */
void renderTurretHeads(Graphics *graphics, GameObject *game) {
    SDL_Rect sRect = {.w = 45, .h = 45};
    SDL_Rect dRect = {.w = 45, .h = 45};
    for (Turret *turret = game->turrets; turret != NULL; turret = turret->next) {
        if (turret->buildStatus) {
            sRect.w = 35; sRect.h = 35;
            dRect.w = 35; dRect.h = 35;
            sRect.x = 35 * floor((9 * turret->buildStatus) / turretData[turret->type][turret->level].buildTime);
            sRect.y = 0;
            dRect.x = turret->x - 17;
            dRect.y = turret->y - 17;
            SDL_RenderCopy(graphics->renderer, graphics->construction, &sRect, &dRect);
            sRect.w = 45; sRect.h = 45;
            dRect.w = 45; dRect.h = 45;
        } else if (turret->isManned) {
            sRect.x = turret->level * 45;
            sRect.y = turret->type * 45;
            dRect.x = turret->x - 22;
            dRect.y = turret->y - 22;
            SDL_RenderCopyEx(graphics->renderer, graphics->turrethead, &sRect, &dRect, turret->rotation, NULL, SDL_FLIP_NONE);
        }
    }
}

/**
 * Function for updating turrets
 */
void updateTurrets(Graphics *graphics, GameObject *game) {
    int rangeSq;
    for (Turret *turret = game->turrets; turret != NULL; turret = turret->next) {
        if (!turret->isManned) {
            ///no one is here
            continue;
        }
        if (turret->buildStatus) {
            ///under construction
            turret->buildStatus--;
            if (!turret->buildStatus && turretData[turret->type][turret->level].isAuto) {
                ///construction of auto-turret completed,
                ///let's send the guy back to the base
                game->missionStaff++;
            }
        } else if (turret->cooldown >= 0 && turret->cooldown % turretData[turret->type][turret->level].cycleTime == 0) {
            ///It can fire!
            rangeSq = SQUARE(turretData[turret->type][turret->level].range);
            ///check if any enemy is in range
            for (Enemy *enemy = game->enemies; enemy != NULL; enemy = enemy->next) {
                if (SQUARE(turret->x - enemy->x) + SQUARE(turret->y - enemy->y) <= rangeSq) {
                    ///FIRE!
                    fire(graphics, game, turret, enemy);

                    if (turret->cooldown == (turretData[turret->type][turret->level].magazine - 1) * turretData[turret->type][turret->level].cycleTime) {
                        ///end of magazine
                        turret->cooldown = -turretData[turret->type][turret->level].reload;
                    } else {
                        turret->cooldown++;
                    }
                    break;
                }
            }
        } else {
            ///reload...
            turret->cooldown++;
        }
    }
}

/**
 * Function for shooting according
 * to the turrets specifications
 */
int fire(Graphics *graphics, GameObject *game, Turret *turret, Enemy *enemy) {
    int fail = 0;
    double sec1, sec2;
    switch (turret->type) {
    case PISTOL:
        switch (turret->level) {
        case 3: fail += fireAtEnemy(game, turret, enemy); break;
        default: fail += fireInDirection(game, turret, enemy, -2.0, 2.0, 0.0);
        } break;
    case RIFLE:
        switch (turret->level) {
        case 0: fail += fireInDirection(game, turret, enemy, -2.0, 2.0, 0.0); break;
        case 1: fail += fireInDirection(game, turret, enemy, -4.0, 4.0, 0.0); break;
        case 2: fail += fireInDirection(game, turret, enemy, -5.0, 5.0, 0.0); break;
        case 3: fail += fireInDirection(game, turret, enemy, -6.0, 6.0, 0.0); break;
        } break;
    case SHOTGUN:
        switch (turret->level) {
        case 0:
        case 1:
            fail += fireInDirection(game, turret, enemy, -14.0, -9.0, 0.0);
            fail += fireInDirection(game, turret, enemy, -9.0, -5.0, 0.0);
            fail += fireInDirection(game, turret, enemy, -5.0, -2.0, 0.0);
            fail += fireInDirection(game, turret, enemy, -2.0, 0.0, 0.0);
            fail += fireInDirection(game, turret, enemy, 0.0, 2.0, 0.0);
            fail += fireInDirection(game, turret, enemy, 2.0, 5.0, 0.0);
            fail += fireInDirection(game, turret, enemy, 5.0, 9.0, 0.0);
            fail += fireInDirection(game, turret, enemy, 9.0, 14.0, 0.0);
            break;
        case 2:
            sec1 = ((double) (rand() % 3)) / 3.0;
            sec2 = ((double) (rand() % 2 + 1)) / 3.0;
            fail += fireInDirection(game, turret, enemy, -5.0, 5.0, sec1);
            fail += fireInDirection(game, turret, enemy, -15.0, 0.0, fmod(sec1 + sec2, 1.0));
            fail += fireInDirection(game, turret, enemy, 0.0, 15.0, fmod(sec1 + 2 * sec2, 1.0));
            break;
        case 3:
            sec1 = ((double) (rand() % 3)) / 3.0;
            sec2 = ((double) (rand() % 2 + 1)) / 3.0;
            fail += fireInDirection(game, turret, enemy, -4.0, 4.0, sec1);
            fail += fireInDirection(game, turret, enemy, -10.0, 0.0, fmod(sec1 + sec2, 1.0));
            fail += fireInDirection(game, turret, enemy, 0.0, 10.0, fmod(sec1 + 2 * sec2, 1.0));
            break;
        } break;
    case MORTAR:
        fail += fireAtEnemy(game, turret, enemy); break;
    case ROCKET:
        switch (turret->level) {
        case 0: fail += fireInDirection(game, turret, enemy, -2.0, 2.0, 0.0); break;
        case 1: fail += fireInDirection(game, turret, enemy, 0.0, 0.0, 0.0); break;
        default: fail += fireAtEnemy(game, turret, enemy); break;
        } break;
    case CANNON:
        switch (turret->level) {
        case 2:
            fail += fireInDirection(game, turret, enemy, -5.0, 5.0, 0.0);
            break;
        case 3:
            sec1 = ((double) (rand() % 2)) / 2.0;
            fail += fireInDirection(game, turret, enemy, -7.0, 3.0, sec1);
            fail += fireInDirection(game, turret, enemy, -3.0, 7.0, 0.5 - sec1);
            break;
        default:
            fail += fireInDirection(game, turret, enemy, -3.0, 3.0, 0.0);
            break;
        } break;
    case GAUSS:
        fail += fireAtEnemy(game, turret, enemy); break;
    case LASER:
        fail += fireLaser(graphics, turret, enemy); break;
    }
    return fail;
}

/**
 * Function for shooting an enemy with a laser
 */
int fireLaser(Graphics *graphics, Turret *turret, Enemy *enemy) {
    enemy->health -= fOData[turret->type][turret->level].damage;
    switch (turret->level) {
    case 0: SDL_SetRenderDrawColor(graphics->renderer, 255, 0, 0, 255); break;
    case 1: SDL_SetRenderDrawColor(graphics->renderer, 144, 0, 255, 255); break;
    case 2: SDL_SetRenderDrawColor(graphics->renderer, 0, 194, 44, 255); break;
    case 3: SDL_SetRenderDrawColor(graphics->renderer, 50, 0, 220, 255); break;
    }
    SDL_RenderDrawLine(graphics->renderer, turret->x, turret->y, enemy->x, enemy->y);
    turret->rotation = atan2(enemy->y - turret->y, enemy->x - turret->x) * 180 / M_PI;
    return 0;
}

/**
 * Function to fire at the current position of an enemy
 *
 * sectorStart, sectorEnd: range of misfire (in degrees)
 * advance: fraction of frame the projectile is already travelling
 */
int fireInDirection(GameObject *game, Turret *turret, Enemy *enemy, double sectorStart, double sectorEnd, double advance) {
    double dx = enemy->x - turret->x;
    double dy = enemy->y - turret->y;
    turret->rotation = atan2(dy, dx) * 180 / M_PI;
    double alpha = (double) (rand() & 127) / 128;
    alpha *= sectorEnd - sectorStart;
    alpha += sectorStart;
    transformVector(&dx, &dy, alpha, (double) fOData[turret->type][turret->level].speed);
    double x = (double) turret->x + dx * advance;
    double y = (double) turret->y + dy * advance;
    return generateFO(game, x, y, dx, dy, turret->type, turret->level, fOData[turret->type][turret->level].maxLife);
}

/**
 * Function to transform a vector
 *
 * *x, *y: vector coordinates (gets changed)
 * alpha: rotation in degrees
 * length: length the vector should have after transformation
 */
void transformVector(double *x, double *y, double alpha, double length) {
    double sinAlpha = sin(alpha * M_PI / 180);
    double cosAlpha = cos(alpha * M_PI / 180);
    double scaling  = length / sqrt(SQUARE(*x) + SQUARE(*y));
    double nx = (cosAlpha * (*x) - sinAlpha * (*y)) * scaling;
    double ny = (sinAlpha * (*x) + cosAlpha * (*y)) * scaling;
    *x = nx;
    *y = ny;
}

/**
 * Function to shoot an enemy intelligently by predicting where
 * the enemy will be when the projectile reaches it
 */
int fireAtEnemy(GameObject *game, Turret *turret, Enemy *enemy) {
    double dx = enemy->x - turret->x;
    double dy = enemy->y - turret->y;
    double estimate = floor((sqrt(SQUARE(dx) + SQUARE(dy)) / ((double) fOData[turret->type][turret->level].speed) + 1) * ((double) enemyData[enemy->type].speed) + 0.5);
    Enemy traveller = *enemy;
    moveEnemy(&traveller, (int) estimate);
    dx = traveller.x - turret->x;
    dy = traveller.y - turret->y;
    turret->rotation = atan2(dy, dx) * 180 / M_PI;

    double scaling = sqrt(SQUARE(dx) + SQUARE(dy)) / (double) fOData[turret->type][turret->level].speed;
    if (turret->type != MORTAR) {
        ///hit with collision
        return generateFO(game, (double) turret->x, (double) turret->y, dx / scaling, dy / scaling,
                                  turret->type, turret->level, fOData[turret->type][turret->level].maxLife);
    } else {
        ///hit the pre-calculated point
        scaling = ceil(scaling);
        return generateFO(game, (double) turret->x, (double) turret->y, dx / scaling, dy / scaling,
                                  turret->type, turret->level, (int) scaling);
    }
}

/**
 * Function to create a new FireObject
 */
int generateFO(GameObject *game, double x, double y, double dx, double dy,
                       turretType type, int level, int lifetime) {
    ///allocate memory, return -1 if it fails
    FireObject *newFO = NULL;
    newFO = malloc(sizeof(FireObject));
    if (newFO == NULL) {
        printf("A pointer is Null where it should not be!\n(newFO)\n");
        return -1;
    }

    ///initialize values
    newFO->type = type;
    newFO->level = level;
    newFO->x = x;
    newFO->y = y;
    newFO->dx = dx;
    newFO->dy = dy;
    newFO->lifetime = lifetime;
    newFO->next = game->fireObjects;

    ///update the list
    game->fireObjects = newFO;
    return 0;
}

/**
 * Function for updating and rendering FireObjects for one frame
 */
void doFireObjects(Graphics *graphics, GameObject *game) {
    updateFireObjects(game);
    if(game->frame == game->speed) {
            renderFireObjects(graphics, game);
    }
    destroyDeadFireObjects(game);
}

/**
 * Function for rendering FireObjects
 */
void renderFireObjects(Graphics *graphics, GameObject *game) {
    SDL_Rect destRec;
    for (FireObject *fo = game->fireObjects; fo != NULL; fo = fo->next) {
        destRec.w = fOData[fo->type][fo->level].rect.w;
        destRec.h = fOData[fo->type][fo->level].rect.h;
        destRec.x = fo->x - destRec.w / 2;
        destRec.y = fo->y - destRec.h / 2;

        SDL_RenderCopyEx(graphics->renderer, graphics->fireObjectTX, &fOData[fo->type][fo->level].rect, &destRec,
                         atan2(fo->dy, fo->dx) * 180 / M_PI, NULL, SDL_FLIP_NONE);
    }
}

/**
 * Function for removing dead FireObjects from the L-List
 * Makes an explosion if necessary
 */
void destroyDeadFireObjects(GameObject *game) {
    FireObject *active = NULL;
    FireObject *prev = NULL;
    FireObject *nactive = NULL;

    for (active = game->fireObjects; active != NULL;) {
        if (active->lifetime) {
            ///still lifetime left
            prev = active;
            active = active->next;
        } else {
            ///to be destroyed
            if (active->type == MORTAR || active->type == ROCKET) {
                ///make an explosion
                Explosion *newExplosion = malloc(sizeof(Explosion));
                if (newExplosion == NULL) {
                    printf("A pointer is Null where it should not be!\n(newExplosion)\n");
                    continue;
                }
                newExplosion->x = active->x;
                newExplosion->y = active->y;
                newExplosion->damage = fOData[active->type][active->level].damage;
                newExplosion->radius = fOData[active->type][active->level].radius;
                newExplosion->next = game->explosions;
                game->explosions = newExplosion;

            }
            if (prev == NULL) {
                game->fireObjects = active->next;
            } else {
                prev->next = active->next;
            }
            nactive = active->next;
            free(active);
            active = nactive;
        }
    }
}

/**
 * Function for updating FireObjects
 */
void updateFireObjects(GameObject *game) {
    double nx;              ///new x-coordinate
    double ny;              ///new y-coordinate
    double firstDistSq;     ///distance between old position and enemy, squared
    double secDistSq;       ///distance between new position and enemy, squared
    double sp1;             ///scalar product of old position -> enemy and direction
    double sp2;             ///scalar product of new position -> enemy and direction
    double minDistSq;       ///minimal distance to enemy, squared
    int radius;             ///radius of projectile

    for (FireObject *active = game->fireObjects; active != NULL; active = active->next) {
        ///new position
        nx = active->x + active->dx;
        ny = active->y + active->dy;
        if (active->type == ROCKET) {
            radius = 4;
        } else {
            radius = fOData[active->type][active->level].radius;
        }

        ///loop through enemies: Is there a hit?
        ///A hit occurs when the minimal distance of the FireObject to the enemy
        ///along its trajectory during this frame is not larger than the sum of the radii
        ///of the enemy and the FireObject.
        if (active->type != MORTAR){
            for (Enemy *target = game->enemies; target != NULL; target = target->next) {
                firstDistSq = SQUARE(target->x - active->x) + SQUARE(target->y - active->y);
                if (SQUARE(enemyData[target->type].radius + radius + fOData[active->type][active->level].speed) < firstDistSq) {
                    continue;
                }
                secDistSq = SQUARE(target->x - nx) + SQUARE(target->y - ny);
                sp1 = SCALARPRODUCT(target->x - active->x, target->y - active->y, active->dx, active->dy);
                sp2 = SCALARPRODUCT(target->x - nx, target->y - ny, active->dx, active->dy);
                if (SAMESIGN(sp1, sp2)) {
                    ///no fly-by, use minimum of start- and end-distance
                    minDistSq = MIN(firstDistSq, secDistSq);
                } else {
                    ///fly-by, use projection & pythagoras
                    minDistSq = firstDistSq - SQUARE(sp1 / fOData[active->type][active->level].speed);
                }
                if (minDistSq <= SQUARE(enemyData[target->type].radius + radius)) {
                    ///hit - do damage
                    target->health -= fOData[active->type][active->level].damage;
                    if (active->type != GAUSS) {
                        ///move FO a bit
                        if (!SAMESIGN(sp1, sp2)) {
                            active->x += active->dx * (sp1 / SQUARE(fOData[active->type][active->level].speed));
                            active->y += active->dy * (sp1 / SQUARE(fOData[active->type][active->level].speed));
                        } else if (secDistSq < firstDistSq) {
                            active->x = nx;
                            active->y = ny;
                        }
                        ///mark FO for destruction
                        active->lifetime = 0;
                        break;
                    }
                }
            }
        }

        if (active->lifetime) {
            ///continue flying
            if (nx < 0 || nx > 1000 || ny < 0 || ny > 720) {
                ///out of bounds - mark FO for destruction
                active->lifetime = 0;
            } else {
                active->lifetime--;
            }
            ///move FO
            active->x = nx;
            active->y = ny;
        }
    }
}

/**
 * Function for executing and rendering explosions
 */
void doExplosions(Graphics *graphics, GameObject *game) {
    Explosion *nactive = NULL;
    double distSq;
    double radiusSq;
    int damageDealt;
    SDL_Rect dRect;
    for (; game->explosions != NULL; game->explosions = nactive) {
        radiusSq = SQUARE(game->explosions->radius);
        for (Enemy *enemy = game->enemies; enemy != NULL; enemy = enemy->next){
            distSq = SQUARE(enemy->x - game->explosions->x) + SQUARE(enemy->y - game->explosions->y);
            if (distSq <= radiusSq) {
                ///enemy is in range of explosion
                damageDealt = floor(game->explosions->damage * (1.0 - (distSq / radiusSq)) + 0.5);
                enemy->health -= damageDealt;
            }
        }
        dRect.w = game->explosions->radius * 2 + 1;
        dRect.h = game->explosions->radius * 2 + 1;
        dRect.x = game->explosions->x - game->explosions->radius;
        dRect.y = game->explosions->y - game->explosions->radius;
        SDL_RenderCopyEx(graphics->renderer, graphics->explosionTX, NULL, &dRect, rand(), NULL, SDL_FLIP_NONE);

        nactive = game->explosions->next;
        free(game->explosions);
    }
}

/**
 * Function for freeing all the memory allocated for turrets
 * Always call this when exiting the level via cleanupAll!
 */
 void cleanupTurrets(GameObject *game) {
    Turret *nactive = NULL;
    for (; game->turrets != NULL; game->turrets = nactive) {
        nactive = game->turrets->next;
        free(game->turrets);
    }
}

/**
 * Function for freeing all the memory allocated for FireObjects
 * Always call this when exiting the level via cleanupAll!
 */
 void cleanupFOs(GameObject *game) {
    FireObject *nactive = NULL;
    for (; game->fireObjects != NULL; game->fireObjects = nactive) {
        nactive = game->fireObjects->next;
        free(game->fireObjects);
    }
}

/**
 * Function for freeing all the memory allocated for Explosions
 * Always call this when exiting the level via cleanupAll!
 */
void cleanupExplosives(GameObject *game) {
    Explosion *nactive = NULL;
    for (; game->explosions != NULL; game->explosions = nactive) {
        nactive = game->explosions->next;
        free(game->explosions);
    }
}
