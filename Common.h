#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define LOG_ERROR(format, ...) printf(format, ##__VA_ARGS__);

//#define LOCK_FREE_QUEUE
