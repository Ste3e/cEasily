#ifndef TEXTAREA_H
#define TEXTAREA_H

#include "Text.h"
#include "Scroll.h"

struct TextArea;
typedef struct TextArea{
	GLuint map;
	Text* text;
	Scroll* scroll;
	int offx, offy, width, height;
	GLubyte bgColor[4];
	int currentLine, maxLines;
	bool scrolling;
	SDL_Rect scrollLine, scrollBox;
	
	void (*setLoc)(struct TextArea* this, int offx, int offy);
	void (*setSize)(struct TextArea* this, int w, int h);
	void (*build)(struct TextArea* this);
	void (*update)(struct TextArea* this);
}TextArea;

TextArea* sj_newTextArea(Text* t, GLuint map, bool bgColor);
void sj_freeTextArea(TextArea* this);

#endif

