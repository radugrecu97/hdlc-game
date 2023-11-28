// Third-party dependencies
#include "gamelogic.h"

int executeAction(coords* position, action actionToExecute)
{
    switch(actionToExecute) {
        case UP:
            position->y--;
            break;
        case DOWN:
            position->y++;
            break;
        case RIGHT:
            position->x++;
            break;
        case LEFT:
            position->x--;
            break;
        default:
            printf("[ERROR] Tried to execute unrecognized '%d' action.\n", actionToExecute);
            break;
    }
    return 1;
}

int doCommandsRepeat(action currentAction, action* lastAction, int* actionRepeated)
{
    // Check if too many commands repeated
    if (currentAction == *lastAction)
    {
        *actionRepeated += 1;
        if (*actionRepeated >= 2)
        {
            return 1;
        }
    }
    else
    {
        *actionRepeated = 0;
    }

    *lastAction = currentAction;
    return 0;
}


int revertAction(coords* currentPos, action lastAction)
{
    // Revert last 3 moves
    switch (lastAction)
    {
        case UP:
            executeAction(currentPos, DOWN);
            break;
        case DOWN:
            executeAction(currentPos, UP);
            break;
        case RIGHT:
            executeAction(currentPos, LEFT);
            break;
        case LEFT:
            executeAction(currentPos, RIGHT);
            break;
        default:
            printf("We shouldn't be here");
            break;
    }
}


int isActionLegal(action currentAction)
{
    if(currentAction < UP || currentAction > LEFT)
    {
        return 0;
    }
    return 1;
}


int isOutOfBounds(coords* currentPos, const coords* gridSize)
{
    if (currentPos->x < 0 ||
        currentPos->x > gridSize->x ||
        currentPos->y < 0 ||
        currentPos->y > gridSize->y)
    {
        return 1;
    }

    return 0;
}


int announce(coords* position, action action)
{
    char actionStr[5] = {0};
    switch (action)
    {
        case UP:
            strncpy(actionStr,"UP", 5);
            break;
        case DOWN:
            strncpy(actionStr,"DOWN", 5);
            break;
        case RIGHT:
            strncpy(actionStr,"RIGHT", 5);
            break;
        case LEFT:
            strncpy(actionStr,"LEFT", 5);
            break;
        default:
            break;
        }
    printf("From [%d, %d], going '%s'\n", position->x, position->y, actionStr);
}


int doGame(const coords gridSize, coords* currentPos, char* actions, int actionCount)
{
    action lastAction = 0;
    int actionRepeated = 0;

    for (int i = 0; i < actionCount; i++)
    {
        action currentAction = (int) actions[i];
        if (!isActionLegal(currentAction))
        {
            printf("[ERROR] Unrecognized '%d' action.\n", currentAction);
            return 0;
        }

        announce(currentPos, currentAction);

        if (doCommandsRepeat(currentAction, &lastAction, &actionRepeated))
        {
            printf("[ERROR] Too many repeat actions. Reverting last 3 actions\n");
            for (int j = 0; j <= actionRepeated; j++)
            {
                revertAction(currentPos, lastAction);
            }
            actionRepeated = 0;
        }

        executeAction(currentPos, currentAction);

        if (isOutOfBounds(currentPos, &gridSize))
        {
            printf("[ERROR] Out of bounds. Reverting action\n");
            revertAction(currentPos, lastAction);
        }
    }
    return 1;
}

