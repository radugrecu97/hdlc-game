// System dependencies
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Third-party dependencies
#include "lib/yahdlc/yahdlc.h"

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


/**
 *  Reads a file completely and returns a heap-allocated char buffer and its size.
 *  !!! Remember to free the passed char buffer when done with it !!!
 * @param[in]  fileName Relative or absolute path to file containing HDLC buffers
 * @param[out] buffer Destination buffer (should be able to contain max frame size)
 * @param[out] bufferSize Destination buffer length
 * @retval >=0 Success
 * @retval <=0 Error
**/
int readFile(const char* fileName, char** buffer, unsigned int *bufferSize)
{
    FILE *filePtr;

    // Open a file in read mode
    filePtr = fopen(fileName, "r");
    if (filePtr == NULL) {
        printf("Error opening file.\n");
        return 0;
    }

    if (filePtr == NULL)
    {
        return 0;
    }

    // Get file size for string allocation
    fseek(filePtr, 0, SEEK_END);
    *bufferSize = ftell(filePtr);
    fseek(filePtr, 0, SEEK_SET);

    // Store the content of the file
    *buffer = (char*) malloc(sizeof(char) * (*bufferSize + 1));
    if (*buffer == NULL) {
        printf("Memory not allocated.\n");
        return 0;
    }

    // Read the content and store it inside myString
    if (fgets(*buffer, *bufferSize, filePtr) == NULL)
    {
        printf("Couldn't read file contents into buffer.\n");
        return 0;
    }

    fclose(filePtr);

    return 1;
}


/**
 * Retrieves list of movement actions from HDLC buffer
 *
 * @param[in]  bufferIn Input buffer of 1 byte commands
 * @param[in]  bufferInSize Size of buffer
 * @param[out] bufferOut Destination buffer (should be able to contain all actions)
 * @param[out] bufferOutSize Actual element count in buffer
 * @retval >=0 Success
 * @retval <=0 Error
 *
 */
int getActions(const char* bufferIn, unsigned int *bufferInSize, char* bufferOut, unsigned int *bufferOutSize)
{
    for (int i = 0; i < *bufferInSize; i++)
    {
        unsigned int hdlcSize = 10;
        char hdlcBuffer[10] = "";
        yahdlc_control_t yahdlcControl;
        yahdlc_state_t yahdlcState;
        int result = yahdlc_get_data_with_state(&yahdlcState, &yahdlcControl, (bufferIn + i), (*bufferInSize - i), hdlcBuffer, &hdlcSize);

        if (hdlcSize > 0)
        {
            // Each command is 1 byte long, so we're only interested in the first element
            bufferOut[(*bufferOutSize)++] = hdlcBuffer[0];
        }

        if (result < 0)
        {
            i += hdlcSize;
            continue;
        }

        i += result;
    }

    return 1;
}


int main(int argc, char *argv[])
{
    const char* fileName = "../assets/transmission.bin";
    char* fileBuffer;
    unsigned int fileSize = 0;

    int result = readFile(fileName, &fileBuffer, &fileSize);
    if (result < 0)
    {
        printf("Error reading file\n");
        return 0;
    }

    char actionBuffer[50] = {0};
    unsigned int actionCount = 0;
    getActions(fileBuffer, &fileSize, actionBuffer, &actionCount);

    for (int i = 0; i < actionCount; i++)
    {
        printf("%d ", actionBuffer[i]);
    }
    printf("\n");

    free(fileBuffer);

    const coords gridSize = {4, 4};
    coords currentPos = {0, 4};

    doGame(gridSize, &currentPos, actionBuffer, actionCount);

    printf("Final position ['%d', '%d']\n\n", currentPos.x, currentPos.y);

    return 1;
}
