#ifndef HUD_H
#define HUD_H

struct Widgets{
	int label, button, check, textArea, menu;
};
struct Widgets w;

void sj_buildHud(void);
void sj_drawHud(void);


#endif
