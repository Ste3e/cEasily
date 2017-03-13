#include "All.h"
#include "hud.h"
#include "hud/Pane.h"



static Pane p;

void sj_buildHud(void){
	sj_getPane(&p);
		
	w.button = p.addButton("Close");
	p.setSize(w.button, 75, 20);
	p.setLoc(w.button, 770, 500);
	
	w.label = p.addText("This is a label.", true);
	p.setLoc(w.label, 50, 50);
	
	char* str = 
	"Incorporating text into a gui library is difficult. The user of the "
	"library should be able to create a string of any length and define an "
	"area of any size and the library should display that text "
	"in that area. The problem is that the string might have to wrap because "
	"it is wider than the area. And if it wraps it might need to scroll "
	"because its wrapping makes the string higher than the area. To compound "
	"the problem the size of the text depends upon the font used and not the "
	"number of characters in the string."; 
	w.textArea = p.addText(str, false);
	p.setSize(w.textArea, 600, 80);
	p.setLoc(w.textArea, 200, 100);
	
	
	p.show();
}
void sj_freeHud(void){
	sj_freePane();	
}

//DRAW
void sj_drawHud(void){
	p.draw();	
}

