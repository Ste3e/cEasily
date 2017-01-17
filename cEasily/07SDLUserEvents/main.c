#include "All.h"
#include "base.h"
#include "hud.h"
#include "hud/Pane.h"
#include "scene/Scene.h"

static bool logCreated = false;

int main(int argc, char *argv[]){
	sj_buildWindow();	
	sj_buildHud();
	Scene* scene = sj_getScene();
		
	SDL_SetRelativeMouseMode(SDL_TRUE);	
	float cursorHalf = ini.cursorSize / 2;
	float xpos = 0, ypos = 0;
	sj_x = sj_y = 0;
	SDL_Event e;
	while(sj_running){
		sj_left = sj_right = sj_caps = false;
		while(SDL_PollEvent(&e)){
			switch(e.type){
			case SDL_KEYDOWN:
				if(e.key.keysym.sym == SDLK_ESCAPE) sj_running = false;
				break;
				
			case SDL_MOUSEMOTION:
				xpos =  sj_xclip + e.motion.xrel * ini.mouseSpeed;
				if(xpos > -1.05 && xpos < 1.05){
					sj_xclip = xpos;
					sj_x = (int)(((sj_xclip + 1) * ini.width) * 0.5); 
				}
				ypos = sj_yclip - e.motion.yrel * ini.mouseSpeed;
				if(ypos > -1.05 && ypos < 1.05){
					sj_yclip = ypos;
					sj_y = (int)-((((sj_yclip + 1) * ini.height) * 0.5) + cursorHalf) + ini.height; 
				}						
				break;
				
			case SDL_MOUSEBUTTONDOWN:
				if(e.button.button == SDL_BUTTON_LEFT) sj_left = true;
				if(e.button.button == SDL_BUTTON_RIGHT) sj_right = true;
				break;
				
			case SDL_USEREVENT:
				if(e.user.code == w.button){
					GuiEvent* ge = (GuiEvent*)e.user.data1; 
					puts(ge->sdat);
					sj_freeGuiEvent(ge);	
				}
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

//exceptions
void sj_handleException(bool fatal, const char* msg, const char* tech){
	FILE *log;
	
	if(!logCreated){
		log = fopen("log.txt", "w");
		logCreated = true;
	}else{
		log = fopen("log.txt", "a");
	}
		
	if(log == NULL){
		fprintf(stderr, "Failed to read log file\n");
	}else{
		fputs(msg, log);
		if(tech != NULL){
			fputs(tech, log);	
		}
		fputs("\n", log);
		fclose(log);
	}
 
	if(fatal){
		sj_running = false;
	}
}