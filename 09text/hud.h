#ifndef HUD_H
#define HUD_H

struct Widgets{
	int button, label, textArea;
};
struct Widgets w;

void sj_buildHud(void);
void sj_drawHud(void);
void sj_freeHud(void);

#endif
