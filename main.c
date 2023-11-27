// System dependencies
#include <stdlib.h>
#include <stdio.h>

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

    for (int i = 0; i <= fileSize; i++)
    {
        printf("0x%x ",  fileBuffer[i]  & 0xff);
    }

    return 0;
}