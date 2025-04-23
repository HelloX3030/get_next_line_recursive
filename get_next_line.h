#pragma once

#include <unistd.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 4
#endif

// #define DEBUG 1
// #define PRINTS 1

char *get_next_line(int fd);
