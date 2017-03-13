#ifndef SCROLL_H
#define SCROLL_H

struct Scroll;
typedef struct Scroll{
	GLuint map;
	bool up, down;
	int offx, offy, width, height;
	int halfH, scrollL;
	SDL_Rect uButton, dButton, sLine, hitRect;
	
	
	void (*build)(struct Scroll* this);
	void (*update)(struct Scroll* this);
}Scroll;

Scroll* sj_newScroll(GLuint map, int left, int top, int right, int height);
void sj_freeScroll(Scroll* this);

#endif

