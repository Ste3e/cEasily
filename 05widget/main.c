#include "All.h"
#include "base.h"
#include "hud.h"
#include "hud/Pane.h"
#include "scene/Scene.h"


int main(int argc, char *argv[]){
	sj_buildWindow();	
	sj_buildHud();
	Scene* scene = sj_getScene();
		
	SDL_Event e;
	while(sj_running){
		while(SDL_PollEvent(&e)){
			switch(e.type){
			case SDL_KEYDOWN:
				if(e.key.keysym.sym == SDLK_ESCAPE) sj_running = false;
				break;
				
			default:
				break;
			}
		}
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
		
		sj_drawHud();
		
		SDL_GL_SwapWindow(win);
	}
	
	SDL_DestroyWindow(win);
	SDL_Quit();
	
	return 0;
}

