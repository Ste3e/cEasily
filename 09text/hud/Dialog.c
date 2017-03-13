#include "../All.h"
#include "Dialog.h"
#include "Button.h"

#define NIL 0
#define ALERT 1

//exceptions
extern void sj_handleException(bool fatal, bool display, const char* msg, const char* tech);//main.c

//struct functions
static void alert(char* msg);
static void checkHit(void);
static void dlgEvent(GuiEvent* e);

//helper functions
static void background(bool paint);

//variables
static Dialog* this;
static bool initialized = false;
static int dlgType = NIL;

//widgets
static Button* ok = NULL; 

Dialog* sj_newDialog(GLuint map){
	if(initialized) return this;
	initialized = true;
	this = (Dialog*)malloc(sizeof(Dialog));
	if(this == NULL) sj_handleException(true, false, "Failed to malloc memory for Frame.", NULL);
	
	this->map = map;
	this->imgw = ini.width * ini.dialogFactor;
	this->imgh = ini.height * ini.dialogFactor;
	this->offx = (ini.width - this->imgw) / 2;
	this->offy = (ini.height - this->imgh) / 2;
	this->innerRect.x = ini.borderSize;
	this->innerRect.y = ini.borderSize;
	this->innerRect.w = this->imgw - (ini.borderSize * 2);
	this->innerRect.h = this->imgh - (ini.borderSize * 2); 
	
	this->alert = alert;
	this->checkHit = checkHit;
	this->dlgEvent = dlgEvent;
	
	return this;
}
void sj_freeDialog(Dialog* d){
		
}

//helper functions
static void background(bool paint){
	//make dialog image
	SDL_Surface* img = SDL_CreateRGBSurface(0, this->imgw, this->imgh, 32, ini.rmask, ini.gmask, ini.bmask, ini.amask);
	if(img == NULL) sj_handleException(true, false, "SDL failed to generate dialog alert image surface. ", SDL_GetError());
	
	if(paint){
		SDL_FillRect(img, NULL, SDL_MapRGBA(img->format, ini.bordColor[0], ini.bordColor[1], ini.bordColor[2], ini.bordColor[3]));
		SDL_FillRect(img, &this->innerRect, SDL_MapRGBA(img->format, ini.bgColor[0], ini.bgColor[1], ini.bgColor[2], ini.bgColor[3]));
	}else{
		SDL_FillRect(img, NULL, SDL_MapRGBA(img->format, ini.bgColor[0], ini.bgColor[1], ini.bgColor[2], ini.bgColor[3]));
	}
	
	//transfer dialog onto main hud image
	glBindTexture(GL_TEXTURE_2D, this->map);
	glTexSubImage2D(GL_TEXTURE_2D, 0, this->offx, this->offy, img->w, img->h, GL_RGBA, GL_UNSIGNED_BYTE, img->pixels);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	SDL_FreeSurface(img);	
}
//struct functions
static void alert(char* msg){
	background(true);
	
	//draw widgets above this image
	ok = sj_newButton(CLOSE, this->map, sj_newStr("OK"));
	ok->setSize(ok, 75, 20);
	ok->setLoc(ok, (this->offx + this->imgw) - 100, (this->offy + this->imgh) - 50);
	ok->build(ok, false);
	
	dlgType = ALERT;
}

//check hits
static void checkHit(void){
	switch(dlgType){
	case NIL:
		sj_handleException(false, true, "Oddly Dialog is checking for hits when no dialogs are open.", NULL);
		break;
		
	case ALERT:
		{
		ok->checkHit(ok);
		}
		break;
	}
}

//handle events
static void dlgEvent(GuiEvent* e){
	switch(e->idat){
	case CLOSE:
		{
			puts(e->sdat);
			sj_freeButton(ok);
			ok = NULL;
			background(false);
		}
	}
}

