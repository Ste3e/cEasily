#include "../All.h"
#include "Scroll.h"

//exceptions
extern void sj_handleException(bool fatal, bool display, const char* msg, const char* tech);//main.c

//struct functions
static void build(Scroll* this);
static void update(Scroll* this);

//variables
static Scroll* this;

Scroll* sj_newScroll(GLuint map, int left, int top, int right, int height){
	this = malloc(sizeof(Scroll));
	if(this == NULL) sj_handleException(true, false, "malloc failed to allocate memory for Scroll object\n", NULL);
	
	this->build = build;
	this->update = update;
	
	this->map = map;
	this->up = this->down = false;
	this->offx = right - ini.barSize;
	this->offy = top;
	this->width = ini.barSize;
	this->height = height;
	this->uButton.x = 0;
	this->uButton.y = 0;
	this->uButton.w = ini.barSize;
	this->uButton.h = ini.barSize;
	this->dButton.x = 0;
	this->dButton.y = height - ini.barSize;
	this->dButton.w = ini.barSize;
	this->dButton.h = ini.barSize;
	this->sLine.x = 0;
	this->sLine.y = 0;
	this->sLine.w = 2;
	this->sLine.h = height;	
	this->hitRect.x = left;
	this->hitRect.y = top;
	this->hitRect.w = right;
	this->hitRect.h = top + height;
	this->halfH = top + (height * 0.5);
	this->scrollL = right - ini.buttonHeight;
	
	return this;
}

static void build(Scroll* s){
	this = s;

	//make button image
	SDL_Surface* img = SDL_CreateRGBSurface(0, this->width, this->height, 32, ini.rmask, ini.gmask, ini.bmask, ini.amask);
	if(img == NULL) sj_handleException(true, false, "SDL failed to generate Scroll bar image surface. ", SDL_GetError());
	
	SDL_FillRect(img, NULL, SDL_MapRGBA(img->format, ini.areaColor[0], ini.areaColor[1], ini.areaColor[2], ini.areaColor[3]));
	SDL_FillRect(img, &this->uButton, SDL_MapRGBA(img->format, ini.widColor[0], ini.widColor[1], ini.widColor[2], ini.widColor[3]));
	SDL_FillRect(img, &this->dButton, SDL_MapRGBA(img->format, ini.widColor[0], ini.widColor[1], ini.widColor[2], ini.widColor[3]));
	SDL_FillRect(img, &this->sLine, SDL_MapRGBA(img->format, ini.widColor[0], ini.widColor[1], ini.widColor[2], ini.widColor[3]));	
		
	//transfer button onto main hud image
	glBindTexture(GL_TEXTURE_2D, this->map);
	glTexSubImage2D(GL_TEXTURE_2D, 0, this->offx, this->offy, img->w, img->h, GL_RGBA, GL_UNSIGNED_BYTE, img->pixels);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	SDL_FreeSurface(img);	
}

static void update(Scroll* s){
	this = s;
		
	if(sj_x > this->hitRect.x && sj_x < this->hitRect.w && sj_y > this->hitRect.y && sj_y < this->hitRect.h){
		this->up = sj_up;
		this->down = sj_down;
		if(sj_x > this->scrollL && sj_left){
			if(sj_y > this->halfH) this->down = true;
			else this->up = true;
		}
	}
}


