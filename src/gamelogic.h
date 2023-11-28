#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Third-party dependencies
#include "../lib/yahdlc/yahdlc.h"

typedef enum {
    INACTIVE,
    UP,
    DOWN,
    RIGHT,
    LEFT
} action;

typedef struct {
    int x;
    int y;
} coords;

int executeAction(coords* position, action actionToExecute);

int doCommandsRepeat(action currentAction, action* lastAction, int* actionRepeated);

int revertAction(coords* currentPos, action lastAction);

int isActionLegal(action currentAction);

int isOutOfBounds(coords* currentPos, const coords* gridSize);

int announce(coords* position, action action);

int doGame(const coords gridSize, coords* currentPos, char* actions, int actionCount);

#endif // GAMELOGIC_H