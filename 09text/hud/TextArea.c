#include "../All.h"
#include "TextArea.h"
#include "Text.h"
#include "Scroll.h"

//exceptions
extern void sj_handleException(bool fatal, bool display, const char* msg, const char* tech);//main.c

//struct functions
static void setLoc(TextArea* this, int offx, int offy);
static void setSize(TextArea* this, int w, int h);
static void build(TextArea* this);
static void update(TextArea* this);

//variables
static TextArea* this;
static Scroll* scroll;

TextArea* sj_newTextArea(Text* text, GLuint map, bool isLabel){
	this = malloc(sizeof(TextArea));
	if(this == NULL) sj_handleException(true, false, "malloc failed to allocate memory for TextArea object\n", NULL);
	
	this->setLoc = setLoc;
	this->setSize = setSize;
	this->build = build;
	this->update = update;
	
	this->map = map;
	this->text = text;
	this->scroll = NULL;
	this->currentLine = 0;
	this->maxLines = 1;
	this->scrolling = false;
	this->offx = 0;
	this->offy = 0;
	if(isLabel){
		this->bgColor[0] = ini.bgColor[0];
		this->bgColor[1] = ini.bgColor[1];
		this->bgColor[2] = ini.bgColor[2];
		this->bgColor[3] = ini.bgColor[3];
		this->text->setSize(this->text);
		this->width = this->text->width;
		this->height = this->text->height;
	}else{
		this->bgColor[0] = ini.areaColor[0];
		this->bgColor[1] = ini.areaColor[1];
		this->bgColor[2] = ini.areaColor[2];
		this->bgColor[3] = ini.areaColor[3];
		//default silly values in case setSize is not called
		this->width = 10;
		this->height = 10;
	}
	
	return this;
}

void sj_freeTextArea(TextArea* ta){
	sj_freeText(ta->text);
	free(ta);
}

//struct functions
static void setLoc(TextArea* ta, int offx, int offy){
	this = ta;
	
	this->offx = offx;
	this->offy = offy;
}
static void setSize(TextArea* ta, int w, int h){
	this = ta;
	
	this->width = w;
	this->height = h;
	this->text->width = w;
	this->text->setSize(this->text);
	if(this->height < this->text->height * this->text->listLines->count){
		this->scrolling = true;
	}
	this->maxLines = h / this->text->height;	
}


//build the button image and copy into main hud texture
static void build(TextArea* ta){
	this = ta;
	
	//make button image
	SDL_Surface* img = SDL_CreateRGBSurface(0, this->width, this->height, 32, ini.rmask, ini.gmask, ini.bmask, ini.amask);
	if(img == NULL) sj_handleException(true, false, "SDL failed to generate TextArea image surface. ", SDL_GetError());	
	
	SDL_FillRect(img, NULL, SDL_MapRGBA(img->format, this->bgColor[0], this->bgColor[1], this->bgColor[2], this->bgColor[3]));
	
	//write text on label
	SDL_Color fontColor = {ini.fontColor[0], ini.fontColor[1], ini.fontColor[2], ini.fontColor[3]};
	Str* tmp = NULL;	
	SDL_Rect pos = {.x = ini.tabSize * ini.borderSize, .y = 0, .w = 0, .h = 0};
	int i;
	int j = this->currentLine; 
	
	for(i = 0; i < this->maxLines; i ++){
		if(j > this->text->listLines->count) break;
		tmp = (Str*)this->text->listLines->get(this->text->listLines, j++);
		SDL_Surface* stext = NULL;
		if(tmp->len < 1){
			stext = TTF_RenderText_Blended(ini.font, " ", fontColor);
		}else{
			stext = TTF_RenderText_Blended(ini.font, tmp->ptr, fontColor);
		}
		if(stext == NULL) sj_handleException(true, false, "SDL failed to generate button's text image surface. ", SDL_GetError());
		SDL_BlitSurface(stext, NULL, img, &pos);	
		SDL_FreeSurface(stext);
		pos.y += this->text->height;
	}
		
	//transfer label onto main hud image
	glBindTexture(GL_TEXTURE_2D, this->map);
	glTexSubImage2D(GL_TEXTURE_2D, 0, this->offx, this->offy, img->w, img->h, GL_RGBA, GL_UNSIGNED_BYTE, img->pixels);
	glBindTexture(GL_TEXTURE_2D, 0);
		
	SDL_FreeSurface(img);
	
	if(this->scrolling){
		this->scroll = sj_newScroll(this->map, this->offx, this->offy, this->offx + this->width, this->height);
		this->scroll->build(this->scroll);	
	}
}

//update scroll
static void update(TextArea* ta){
	this = ta;
	
	if(!this->scrolling) return;
	this->scroll->update(this->scroll);
	if(this->scroll->up){
		this->scroll->up = false;	
		if(this->currentLine > 0){
			this->currentLine--;	
		}
		build(this);
	}
	if(this->scroll->down){
		this->scroll->down = false;
		if(this->currentLine + this->maxLines >= this->text->listLines->count){
			this->currentLine = this->text->listLines->count - this->maxLines;	
		}else{
			this->currentLine++;	
		}
		build(this);
	}
}
