#ifndef PANE_H
#define PANE_H

#include "../All.h"
#include "Pane.h"


typedef struct Pane{
	int (*addText)(char* str, bool isLabel);
	int (*addButton)(char* str);
	
	void (*setSize)(int index, int w, int h);
	void (*setLoc)(int index, int x, int y);
	
	void (*show)(void);
	void (*draw)(void);	
}Pane;

//dialog functions
void sj_alert(char* msg);
void sj_dlgEvent(GuiEvent* ge);

//constructor destructor
void sj_getPane(Pane* pane);
void sj_freePane(void);

#endif
