#include "../All.h"
#include "Button.h"

extern void sj_hudHandler(int id, int etc, char* data);

//static struct functions
static void build(struct Button* this, bool over);
static void setSize(struct Button* this, int w, int h);
static void setLoc(struct Button* this, int x, int y);

//static helper functions
static void calculateAreas(void);

static Button* this;

Button* sj_getButton(int id, GLuint mainMap, Str* text){
	this = malloc(sizeof(Button));
	if(this == NULL){
		fprintf(stderr, "malloc failed to allocate memory for Button\n");
		exit(EXIT_FAILURE);
	}
	
	this->id = id;
	this->mainMap = mainMap;
	this->text = text;
	this->virgin = true;
	this->dirty = true;
	this->xloc = this->yloc = 0;
	this->over = false;
	
	//preliminary sizing 
	this->textRect.x = this->textRect.y = 0;
	TTF_SizeText(ini.font, this->text->ptr, &this->textRect.w, &this->textRect.h);
	this->baseRect.w = this->textRect.w;
	this->baseRect.h = this->textRect.h;
		
	this->setSize = setSize;
	this->setLoc = setLoc;
	this->build = build;
	
	return this;
}
void sj_freeButton(Button* b){
	free(b->text);
	free(b);
}

//mutators
static void setSize(struct Button* button, int w, int h){
	this = button;
	
	if(w > this->baseRect.w){
		this->baseRect.w = w;	
	}
	if(h > this->baseRect.h){
		this->baseRect.h = h;	
	}
}

static void setLoc(struct Button* button, int x, int y){
	this = button;
	
	this->xloc = x;
	this->yloc = y;	
}

//build the button image and copy into main hud texture
static void build(struct Button* button, bool over){
	if(!this->dirty) return;
	this = button;
	if(this->virgin){
		this->virgin = false;
		calculateAreas();
	}
	
	//make button image
	SDL_Surface* img = SDL_CreateRGBSurface(0, this->baseRect.w, this->baseRect.h, 32, ini.rmask, ini.gmask, ini.bmask, ini.amask);
	SDL_assert(img != NULL);
	
	Uint8 color[4];
	if(over){
		color[0] = ini.widOverColor[0];
		color[1] = ini.widOverColor[1];
		color[2] = ini.widOverColor[2];
		color[3] = ini.widOverColor[3];
	}else{
		color[0] = ini.widColor[0];
		color[1] = ini.widColor[1];
		color[2] = ini.widColor[2];
		color[3] = ini.widColor[3];
	}
	if(ini.borderSize > 0){
		SDL_FillRect(img, NULL, SDL_MapRGBA(img->format, ini.bordColor[0], ini.bordColor[1], ini.bordColor[2], ini.bordColor[3]));
		SDL_FillRect(img, &this->innerRect, SDL_MapRGBA(img->format, color[0], color[1], color[2], color[3]));
	}else{
		SDL_FillRect(img, NULL, SDL_MapRGBA(img->format, color[0], color[1], color[2], color[3]));
	}
	
	//write text on button
	SDL_Color fontColor = {ini.fontColor[0], ini.fontColor[1], ini.fontColor[2], ini.fontColor[3]};
	SDL_Surface* stext;
	if(this->text->len < 1){
		stext = TTF_RenderText_Blended(ini.font, " ", fontColor);
	}else{
		stext = TTF_RenderText_Blended(ini.font, this->text->ptr, fontColor);
	}
	SDL_assert(stext != NULL);
	SDL_BlitSurface(stext, NULL, img, &this->textRect);	
	
	//transfer button onto main hud image
	glBindTexture(GL_TEXTURE_2D, this->mainMap);
	glTexSubImage2D(GL_TEXTURE_2D, 0, this->xloc, this->yloc, img->w, img->h, GL_RGBA, GL_UNSIGNED_BYTE, img->pixels);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	SDL_FreeSurface(stext);
	SDL_FreeSurface(img);
}

static void calculateAreas(void){
	this->baseRect.x = 0;
	this->baseRect.y = 0; 
	this->textRect.x = (int)((this->baseRect.w - this->textRect.w) / 2);
	this->innerRect.x = ini.borderSize;
	this->innerRect.y = ini.borderSize;
	this->innerRect.w = this->baseRect.w - (ini.borderSize * 2);
	this->innerRect.h = this->baseRect.h - (ini.borderSize * 2); 
}