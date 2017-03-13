#ifndef DIALOG_H
#define DIALOG_H

//widget ids for events and make them accesible to Pane.c
#define CLOSE 500001

struct Dialog;
typedef struct Dialog{
	GLuint map;
	int imgw, imgh, offx, offy;
	SDL_Rect innerRect, hitRect, textRect;
	bool hidden;
	
	void (*alert)(char* msg);
	
	void (*dlgEvent)(GuiEvent* e);
	void (*checkHit)(void);
}Dialog;

//constructor destructor
Dialog* sj_newDialog(GLuint map);
void sj_freeDialog(Dialog* this);

#endif
