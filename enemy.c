#include <stdio.h>
#include <stdlib.h>

#include "declarations.h"



/**
 * Function for spawning a new enemy
 * according to the information about the active wave
 */
int spawnEnemy(GameObject *game) {
    ///allocate memory, return -1 if it fails
    Enemy *newEnemy = NULL;
    newEnemy = malloc(sizeof(Enemy));
    if (newEnemy == NULL) {
        printf("A pointer is Null where it should not be!\n(newEnemy)\n");
        return -1;
    }
    if (game->waves == NULL) {
        ///something or someone really f*cked up
        printf("A pointer is Null where it should not be!\n(newEnemy -> waves)\n");
        return -1;
    }

    ///initialize values
    int index = rand() % game->path->startcount;
    PathStart *start = game->path->starts + index;
    newEnemy->type = game->waves->type;
    newEnemy->health = game->waves->health;
    newEnemy->x = start->x;
    newEnemy->y = start->y;
    newEnemy->dist = 0;
    newEnemy->current = start->elem;
    newEnemy->distToBase = start->remain;
    newEnemy->anilife = rand() % enemyData[newEnemy->type].aniCycleFrames;

    ///update the list
    ///Is list empty?
    if (game->enemies == NULL) {
        newEnemy->next = NULL;
        game->enemies = newEnemy;
        return 0;
    }

    ///otherwise: insert according to distToBase
    if (game->enemies->distToBase > newEnemy->distToBase) {
        ///insert at first position
        newEnemy->next = game->enemies;
        game->enemies = newEnemy;
    }
    else {
        ///active starts at the first enemie, goes through the list until:
        /// a) the last one is reached OR
        /// b) the next one has a longer remaining distance to the base
        Enemy *active = game->enemies;
        for (;active->next != NULL && active->next->distToBase < newEnemy->distToBase; active = active->next) {}
        newEnemy->next = active->next;
        active->next = newEnemy;
    }
    return 0;
}

/**
 * Function for updating and rendering enemies for one frame
 */
void doEnemies(Graphics *graphics, GameObject *game) {
    updateEnemies(game);
    sortEnemies(game);
    if(game->frame == game->speed) {
        renderEnemies(graphics, game);
    }
}

/**
 * Function for rendering the enemies
 */
void renderEnemies(Graphics *graphics, GameObject *game) {
    SDL_Rect sourceRec;
    SDL_Rect destRec;
    for (Enemy *enemy = game->enemies; enemy != NULL; enemy = enemy->next) {
        enemy->anilife = (enemy->anilife +1) % enemyData[enemy->type].aniCycleFrames;
        sourceRec = enemyData[enemy->type].rect;
        sourceRec.x = sourceRec.w * ((enemy->anilife * enemyData[enemy->type].aniCycleStates) / enemyData[enemy->type].aniCycleFrames);
        destRec = enemyData[enemy->type].rect;
        destRec.x = enemy->x - destRec.w / 2;
        destRec.y = enemy->y - destRec.h / 2;
        SDL_RenderCopyEx(graphics->renderer, graphics->enemyTX, &sourceRec, &destRec, 90 * enemy->current->dir, NULL, SDL_FLIP_NONE);
    }
}

/**
 * Function for moving an Enemy along its path
 * returns -1 if the enemy left the path, 0 otherwise
 */
int moveEnemy(Enemy *enemy, int distance) {
    ///Is there a path?
    if (enemy->current == NULL) {
        return -1;
    }

    ///calculate what remains of the movement
    ///after the next corner
    ///(negative, if the corner is not reached)
    int remain = enemy->dist + distance - enemy->current->length;
    if (remain >= 0) {
        distance -= remain;
    }

    ///moving, aka changing the position
    switch (enemy->current->dir) {
    case NORTH:
        enemy->y -= distance; break;
    case EAST:
        enemy->x += distance; break;
    case SOUTH:
        enemy->y += distance; break;
    case WEST:
        enemy->x -= distance; break;
    }
    enemy->dist += distance;

    ///Does the Enemy reach the corner?
    if (remain >= 0) {
        enemy->current = enemy->current->next;
        if (enemy->current == NULL){
            ///base reach event
            return -1;
        } else {
            enemy->dist = 0;
            return moveEnemy(enemy, remain);
        }
    }
    return 0;
}

/**
 * Function for updating enemies
 * Destroys dead enemies and moves the other ones.
 */
void updateEnemies(GameObject *game) {
    Enemy *active = NULL;
    Enemy *prev = NULL;
    Enemy *nactive = NULL;
    for (active = game->enemies; active != NULL;) {
        if (active->health <= 0) {
            ///Enemy shall be destroyed
            if (prev == NULL) {
                game->enemies = active->next;
            } else {
                prev->next = active->next;
            }
            nactive = active->next;
            free(active);
            active = nactive;
        } else {
            ///Enemy shall continue on its path
            if (moveEnemy(active, enemyData[active->type].speed)) {
                ///kill off enemy
                active->health = 0;
                ///attack base
                getPerson(game, 1);
                continue;
            }
            active->distToBase -= enemyData[active->type].speed;
            prev = active;
            active = active->next;
        }
    }
}

/**
 * Function for re-building the enemy-list,
 * sorted by distance to base
 */
void sortEnemies(GameObject *game) {
    if (game->enemies == NULL || game->enemies->next == NULL) {
        return;
    }

    int changed = 1;
    Enemy **continuation;
    Enemy *active;
    while (changed) {
        changed = 0;
        continuation = &(game->enemies);
        active = game->enemies;
        for (Enemy *follower = active->next; follower != NULL; follower = follower->next) {
            if (follower->distToBase < active->distToBase) {
                ///this has to get sorted!
                *continuation = follower;
                continuation = &(follower->next);
                changed = 1;
            } else {
                ///no sorting
                *continuation = active;
                continuation = &(active->next);
                active = follower;
            }
        }
    }
}

/**
 * Function for updating the wave information
 */
void doWaves(GameObject *game) {
    game->wavetime++;
    if (game->wavetime == 1200) {
        game->wavetime = 0;
        ///call in a new wave
        Wave *nactive = game->waves->next;
        free(game->waves);
        game->waves = nactive;

        if (game->waves != NULL) {
            ///selecting spawntimes
            selectTimes(game->spawntimes + 1, game->waves->amount, 0, 799);
            ///setting index
            game->spawntimes[0] = 1;
            ///clear next entry
            game->spawntimes[game->waves->amount + 1] = 0;
        }
    }
    if (game->spawntimes[game->spawntimes[0]] == game->wavetime) {
        ///spawn a new enemy
        spawnEnemy(game);
        ///update index
        game->spawntimes[0]++;
    }
}

/**
 * Helper function for selecting times when enemies should spawn
 */
void selectTimes(int *array, int positions, int start, int end) {
    ///basic idea:
    ///select a position somewhere in the middle of the
    ///given time-window and put a spawn there,
    ///then one half before and other half after
    int left = (positions - 1) / 2;
    int right = positions / 2;
    int range = (end - start) / 5 + 1;
    int position = rand() % range + start + 2 * range;
    while (position - start < left) {
        position++;
    }
    while (end - position < right) {
        position--;
    }
    array[left] = position;
    if (left) {
        selectTimes(array, left, start, position - 1);
    }
    if (right) {
        selectTimes(array + left + 1, right, position + 1, end);
    }
}

/**
 * Function for freeing all the memory allocated for enemies
 * Always call this when exiting the level via cleanupAll!
 */
void cleanupEnemies(GameObject *game) {
    Enemy *nactive = NULL;
    for (; game->enemies != NULL; game->enemies = nactive) {
        nactive = game->enemies->next;
        free(game->enemies);
    }
}

/**
 * Function for freeing all the memory allocated for waves
 * Always call this when exiting the level via cleanupAll!
 */
void cleanupWaves(GameObject *game) {
    Wave *nactive = NULL;
    for (; game->waves != NULL; game->waves = nactive) {
        nactive = game->waves->next;
        free(game->waves);
    }
}

/**
 * Function for freeing all the memory allocated for paths
 * Always call this when exiting the level via cleanupAll!
 */
void clenupPaths(GameObject *game) {
    free(game->path->starts);
    free(game->path->arrayStart);
}
