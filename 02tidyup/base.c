#include "All.h"
#include "base.h"

//exception handler
extern void sj_handleException(bool fatal, const char* msg, const char* tech);//main.c

static void checkError(void);
static bool readIni(void);
static bool startsWith(const char* whole, const char* part);
static char* remFrom(char* whole, const char* toRem);

static SDL_GLContext context;

void sj_buildWindow(void){
	if(SDL_Init(SDL_INIT_VIDEO) == -1) sj_handleException(true, "SDL_ video init fail.", SDL_GetError());
	
	if(!readIni()) sj_handleException(true, "SDL create window fail.", SDL_GetError());
	
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 2);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	
	GLint flags = SDL_WINDOW_OPENGL | SDL_WINDOW_BORDERLESS | SDL_WINDOW_SHOWN;
	win = SDL_CreateWindow("cGuigle", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, ini.width, ini.height, flags);
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

	glViewport(0, 0, ini.width, ini.height);
	
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

static bool readIni(){
	FILE* in = fopen("main.ini", "r");
	if(in == NULL) sj_handleException(true, "Could not open main.ini.", NULL);
		
	char line[1024];
	while(fgets(line, sizeof(line), in)){
		if(startsWith(line, "width")) ini.width = atoi(remFrom(line, "width "));
		if(startsWith(line, "height")) ini.height = atoi(remFrom(line, "height "));
	}

	fclose(in);	
	
	return true;
}

static bool startsWith(const char* whole, const char* part){
	size_t partSize = strlen(part) * sizeof(char);
	int same = strncmp(whole, part, partSize);
	
	if(same == 0) return true;
	return false;
}

static char* remFrom(char* whole, const char* toRem){
	char* tmp = strtok(whole, "\n");//chop out the '\n' on whole if whole is got by fgets
	
	size_t size = strlen(tmp) - strlen(toRem);
	if(size == 0) return NULL;
	
	char* toret = malloc((size + 1) * sizeof(char));
	if(toret == NULL) return NULL;
	
	strcpy(toret, tmp + strlen(toRem));
	
	return toret;
}