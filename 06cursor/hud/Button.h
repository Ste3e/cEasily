#ifndef BUTTON_H
#define BUTTON_H

struct Button;
typedef struct Button{
	int id;
	GLuint mainMap;
	Str* text;
	bool virgin;
	int xloc, yloc;
	SDL_Rect baseRect, innerRect, hitRect, textRect;
	bool over;
	
	void (*setSize)(struct Button* this, int w, int h);
	void (*setLoc)(struct Button* this, int x, int y);
	void (*build)(struct Button* this, bool over);
	void (*checkHit)(struct Button* this);
}Button;

Button* sj_newButton(int id, GLuint mainMap, Str* text);
void sj_freeButton(Button* this);

#endif

