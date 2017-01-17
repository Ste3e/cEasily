#include "All.h"

//forward declarations
void sj_handleException(bool fatal, const char* msg, const char* tech);
static void buildWindow();
static void checkError(void);

static SDL_Window* win;
static SDL_GLContext context;
static int width = 1024, height = 768;
static bool logCreated = false;

int main(int argc, char *argv[]){
	buildWindow();
	
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
		SDL_GL_SwapWindow(win);
	}
	
	SDL_DestroyWindow(win);
	SDL_Quit();
	
	return 0;
}

static void buildWindow(void){
	if(SDL_Init(SDL_INIT_VIDEO) == -1) sj_handleException(true, "SDL_ video init fail.", SDL_GetError());
		
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 2);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	
	GLint flags = SDL_WINDOW_OPENGL | SDL_WINDOW_BORDERLESS | SDL_WINDOW_SHOWN;
	win = SDL_CreateWindow("cGuigle", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
	if(win == NULL) sj_handleException(true, "SDL create window fail.", SDL_GetError());
	
	context = SDL_GL_CreateContext(win);
	if(context == NULL) sj_handleException(true, "SDL create context fail.", SDL_GetError());
	
	int err=glewInit();
	if(err != GLEW_OK) sj_handleException(true, "Glew failed to initialize\n", NULL);
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	
	glDepthFunc(GL_LEQUAL);

	glClearColor(0.5, 0.0, 0.0, 1.0);
	glClearDepth(1.0);

	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);	

	glViewport(0, 0, width, height);
	
	checkError();
	sj_running = true;
}

static void checkError(void){	
	const char* renderer = (const char*) glGetString(GL_RENDERER);
	sj_handleException(false, "OpenGL renderer: ", renderer);
	
	const char* version = (const char*) glGetString(GL_VERSION);
	sj_handleException(false, "OpenGL version: ", version); 
	
	char c = *version;
	if(c == '1' || c == '2'){
		sj_handleException(true, "Your openGL version is too low to run this program.", NULL);
	}
	
	GLenum error = glGetError();
	switch(error){
	 	case GL_NO_ERROR: break;
	 	case GL_INVALID_ENUM: sj_handleException(true, "invalid enum", NULL); break;
	 	case GL_INVALID_VALUE: sj_handleException(true, "invalid value", NULL); break;
	 	case GL_OUT_OF_MEMORY: sj_handleException(true, "out of memory", NULL); break;
	 	case GL_INVALID_FRAMEBUFFER_OPERATION: sj_handleException(true, "invalid framebuffer operation", NULL); break;
	 	default: break;	
	}
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
	
	