// System dependencies
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "src/gamelogic.h"

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
