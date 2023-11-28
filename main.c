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
 * @retval >=0 Success (size of returned value should be discarded from source buffer)
**/
int readFile(const char* fileName, char** buffer, unsigned int *bufferSize)
{
    FILE *filePtr;

    // Open a file in read mode
    filePtr = fopen(fileName, "r");
    if (filePtr == NULL) {
        printf("Error opening file.\n");
        exit(-1);
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
        exit(-1);
    }

    // Read the content and store it inside myString
    if (fgets(*buffer, *bufferSize, filePtr) == NULL)
    {
        printf("Couldn't read file contents into buffer.\n");
        exit(-1);
    }

    fclose(filePtr);

    return 0;
}


/**
 * Retrieves data from specified buffer containing the HDLC frame. Frames can be
 * parsed from multiple buffers e.g. when received via UART.
 *
 * @param[in]  fileName Relative or absolute path to file containing HDLC buffers
 * @param[out] buffer Destination buffer (should be able to contain max frame size)
 * @param[out] bufferSize Destination buffer length
 * @retval >=0 Success (size of returned value should be discarded from source buffer)
 *
 */
int getHDLC(const char* bufferIn, unsigned int bufferInSize, char* bufferOut, unsigned int* bufferOutSize)
{
    yahdlc_control_t temp;
    int result = yahdlc_get_data(&temp, bufferIn, bufferInSize, bufferOut, bufferOutSize);
    return result;
}

int main(int argc, char *argv[])
{
    const char* fileName = "../assets/transmission.bin";
    char* fileBuffer;
    unsigned int fileSize;

    int result = readFile(fileName, &fileBuffer, &fileSize);
    if (result < 0)
    {
        printf("Error reading file\n");
        return -1;
    }

    for (int i = 0; i < fileSize; i++)
    {
        unsigned int hdlcSize = 50;
        char hdlcBuffer[50] = "";
        yahdlc_control_t yahdlcControl;
        yahdlc_state_t yahdlcState;
        result = yahdlc_get_data_with_state(&yahdlcState, &yahdlcControl, fileBuffer+i, fileSize-i, hdlcBuffer, &hdlcSize);
        // printf("Result code = '%d'\n", result);
        // getHDLC(fileBuffer + i, fileSize, hdlcBuffer, &hdlcSize);

        for (int j = 0; j < hdlcSize; j++)
        {
            printf("### '%d' ",  hdlcBuffer[j]);
        }
        if (hdlcSize != 0)
        {
            printf("| ");
        }

        if (result < 0)
        {
            i += hdlcSize;
            continue;
        }


        i += result;
        // printf("0x%x ",  fileBuffer[i]  & 0xff);
    }

    // unsigned int hdlcSize = 800;
    // char hdlcBuffer[800] = "";


    // getHDLC(fileBuffer, fileSize, &hdlcBuffer, &hdlcSize);
    // printf("HDLC size '%u' HDLC buffer '%s'", hdlcSize, hdlcBuffer);

    // // Clean-up
    // free(fileBuffer);

    return 0;
}