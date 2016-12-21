#include "All.h"
#include "base.h"

int main(int argc, char *argv[]){
	sj_buildWindow();
	
	SDL_Event e;
	while(cg_running){
		while(SDL_PollEvent(&e)){
			switch(e.type){
			case SDL_KEYDOWN:
				if(e.key.keysym.sym == SDLK_ESCAPE) cg_running = false;
				break;
				
			default:
				break;
			}
		}
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);				
		SDL_GL_SwapWindow(win);
	}
	
	SDL_DestroyWindow(win);
	SDL_Quit();
	
	return 0;
}

