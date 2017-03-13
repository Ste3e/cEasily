#ifndef GUIEVENT_H
#define GUIEVENT_H

struct GuiEvent;
typedef struct GuiEvent{
	int idat;
	char* sdat;
}GuiEvent;

GuiEvent* sj_newGuiEvent(void);
void sj_freeGuiEvent(GuiEvent* this);

#endif

