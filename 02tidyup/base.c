#include "All.h"
#include "base.h"

static bool checkError(void);
static bool readIni(void);
static bool startsWith(const char* whole, const char* part);
static char* remFrom(char* whole, const char* toRem);

static SDL_GLContext context;
static bool ok = true;

void sj_buildWindow(void){
	if(SDL_Init(SDL_INIT_VIDEO) == -1){
		printf("SDL_Init failed: %s\n", SDL_GetError());
		return;
	}
	
	if(!readIni()){
		fprintf(stderr, "Failed to read ini file\n");
		exit(EXIT_FAILURE);
	}
	
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 2);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	
	GLint flags = SDL_WINDOW_OPENGL | SDL_WINDOW_BORDERLESS | SDL_WINDOW_SHOWN;
	win = SDL_CreateWindow("cGuigle", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, ini.width, ini.height, flags);
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

	glViewport(0, 0, ini.width, ini.height);
	
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

static bool readIni(){
	FILE* in = fopen("main.ini", "r");
	if(in == NULL){
		fprintf(stderr, "Could not open Guigle.ini");
		return false;
	}
	
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