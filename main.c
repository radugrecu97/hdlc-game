// System dependencies
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Third-party dependencies
#include "lib/yahdlc/yahdlc.h"


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
        return -1;
    }

    if (filePtr == NULL)
    {
        return -1;
    }

    // Get file size for string allocation
    fseek(filePtr, 0, SEEK_END);
    *bufferSize = ftell(filePtr);
    fseek(filePtr, 0, SEEK_SET);

    // Store the content of the file
    *buffer = (char*) malloc(sizeof(char) * (*bufferSize + 1));
    if (*buffer == NULL) {
        printf("Memory not allocated.\n");
        return -1;
    }

    // Read the content and store it inside myString
    if (fgets(*buffer, *bufferSize, filePtr) == NULL)
    {
        printf("Couldn't read file contents into buffer.\n");
        return -1;
    }

    fclose(filePtr);

    return 0;
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

    return 0;
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
        return -1;
    }

    char actionBuffer[50] = {0};
    unsigned int actionCount = 0;
    getActions(fileBuffer, &fileSize, actionBuffer, &actionCount);

    printf("\n");
    free(fileBuffer);

    return 0;
}
