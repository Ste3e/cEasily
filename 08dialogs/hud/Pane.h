#ifndef PANE_H
#define PANE_H

#include "../All.h"
#include "Pane.h"


typedef struct Pane{
	int (*addButton)(char* text);
	
	void (*setSize)(int index, int w, int h);
	void (*setLoc)(int index, int x, int y, bool mainHud);
	
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
