#include "All.h"
#include "hud.h"
#include "hud/Pane.h"

static Pane p;

void sj_buildHud(void){
	sj_getPane(&p);	
}

void sj_drawHud(void){
	p.draw();	
}

