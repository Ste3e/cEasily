#ifndef ALL_H
#define ALL_H

#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <io.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <math.h>

#include <gl/glew.h>
#include "SDL.h"
#include "SDL_image.h" 
#include "SDL_ttf.h"

struct Ini{
	int width;
	int height;
};
struct Ini ini;

bool sj_running;

#endif
