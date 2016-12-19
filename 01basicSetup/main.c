#include "All.h"

static void buildWindow();
static bool checkError(void);

static SDL_Window* win;
static SDL_GLContext context;
static int width = 1024, height = 768;
static bool ok = true;

int main(int argc, char *argv[]){
	buildWindow();
	
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

static void buildWindow(void){
	if(SDL_Init(SDL_INIT_VIDEO) == -1){
		printf("SDL_Init failed: %s\n", SDL_GetError());
		return;
	}
	
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 2);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	
	GLint flags = SDL_WINDOW_OPENGL | SDL_WINDOW_BORDERLESS | SDL_WINDOW_SHOWN;
	win = SDL_CreateWindow("cGuigle", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
	if(win == NULL){
		printf("SDL_CreateWindow failed: %s\n", SDL_GetError());
		return;
	}
	
	context = SDL_GL_CreateContext(win);
	if(context == NULL){
		printf("SDL_GL_CreateContext failed: %s\n", SDL_GetError());
		return;
	}
	
	int err=glewInit();
	if(err != GLEW_OK){
		printf("GlewInit failed: \n");
		return;
	}
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	
	glDepthFunc(GL_LEQUAL);

	glClearColor(0.5, 0.0, 0.0, 1.0);
	glClearDepth(1.0);

	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);	

	glViewport(0, 0, width, height);
	
	ok = checkError();
	cg_running = ok;
}

static bool checkError(void){	
	const char* renderer = (const char*) glGetString(GL_RENDERER);
	printf("Render is %s\n", renderer);
	
	const char* version = (const char*) glGetString(GL_VERSION);
	printf("OpenGL version is %s\n", version); 
	char c = *version;
	if(c == '1' || c == '2'){
		puts("Your openGL version is too low to run this program.");
		return false;
	}
	
	bool toret = true;
	GLenum error = glGetError();
	switch(error){
	 	case GL_NO_ERROR: break;
	 	case GL_INVALID_ENUM: puts("invalid enum"); toret = false; break;
	 	case GL_INVALID_VALUE: puts("invalid value"); toret = false; break;
	 	case GL_OUT_OF_MEMORY: puts("out of memory"); toret = false; break;
	 	case GL_INVALID_FRAMEBUFFER_OPERATION: puts("invalid framebuffer operation"); toret = false; break;
	 	default: break;	
	}
	
	return toret;
}