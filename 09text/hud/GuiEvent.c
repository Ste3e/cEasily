#include "../All.h"
#include "GuiEvent.h"

//exceptions
extern void sj_handleException(bool fatal, bool display, const char* msg, const char* tech);//main.c

static GuiEvent* this;

GuiEvent* sj_newGuiEvent(void){
	this = malloc(sizeof(GuiEvent));
	if(this == NULL) sj_handleException(true, false, "malloc failed to allocate memory for GuiEvent\n", NULL);
	
	this->sdat = NULL;
	
	return this;
}

void sj_freeGuiEvent(GuiEvent* ge){
	free(ge);
}