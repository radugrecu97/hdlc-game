#ifndef UTIL_H
#define UTIL_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../lib/yahdlc/yahdlc.h"

int readFile(const char* fileName, char** buffer, unsigned int *bufferSize);

int getActions(const char* bufferIn, unsigned int *bufferInSize, char* bufferOut, unsigned int *bufferOutSize);

#endif // UTIL_H
