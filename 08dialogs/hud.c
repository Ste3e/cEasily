#include "All.h"
#include "hud.h"
#include "hud/Pane.h"


static Pane p;

void sj_buildHud(void){
	sj_getPane(&p);
		
	w.button = p.addButton("Close");
	p.setSize(w.button, 75, 20);
	p.setLoc(w.button, 770, 500, true);
	
	p.show();
}
void sj_freeHud(void){
	sj_freePane();	
}

//DRAW
void sj_drawHud(void){
	p.draw();	
}

