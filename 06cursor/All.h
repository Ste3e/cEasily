#ifndef ALL_H
#define ALL_H

#define LEN(x) (sizeof(x) / sizeof(*(x)))

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

#include "src/tools/List.h"
#include "src/tools/Str.h"

struct Ini{
	TTF_Font* font;
	TTF_Font* dings;
	Str* path;
	Str* iniPath;
	Str* cursorPoint;
	Str* cursorText;
	int width;
	int height;
	int buttonHeight;
	int borderSize;
	int fontSize;
	GLuint rmask, gmask, bmask, amask;
	GLubyte bgColor[4];
	GLubyte bordColor[4];
	GLubyte widColor[4];
	GLubyte widOverColor[4];
	GLubyte widDisColor[4];
	GLubyte scrollColor[4];
	GLubyte fontColor[4];
	GLubyte areaColor[4];
	int cursorSize;
	float mouseSpeed;
};
struct Ini ini;

bool sj_running;
int sj_x, sj_y;
float sj_xclip, sj_yclip;
bool sj_left, sj_right, sj_caps;
#endif
