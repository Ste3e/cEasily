#include "../All.h"
#include "Pane.h"
#include "Frame.h"

//struct functions
static void draw(void);

//variables
static Frame* guiFrame;

void sj_getPane(Pane* pane){
	pane->draw = draw;
	
	guiFrame = sj_getFrame();
}

static void draw(void){
	guiFrame->draw(guiFrame);
}




