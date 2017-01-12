#include "All.h"
#include "base.h"

static bool checkError(void);
static bool readIni(void);

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
	sj_running = ok;
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
	//set paths
	#ifdef _WIN32 
	Str* bin = sj_newStr("bin\\");
	Str* iniFile = sj_newStr("main.ini");
	Str* point = sj_newStr("rs\\images\\point.png");
	Str* text = sj_newStr("rs\\images\\text.png");
	Str* font = sj_newStr("rs\\fonts\\");
	#else
	Str* bin = sj_newStr("bin/");
	Str* iniFile = sj_newStr("main.ini");
	Str* point = sj_newStr("rs/images/point.png");
	Str* text = sj_newStr("rs/images/text.png");
	Str* font = sj_newStr("rs/fonts/");
	#endif	
		
	char* basePath = SDL_GetBasePath();
	ini.path = sj_chopBack(sj_newStr(basePath), bin->ptr);
	SDL_free(basePath);
	ini.iniPath = sj_addBack(ini.path, iniFile->ptr);
	free(iniFile);
	ini.cursorPoint = sj_addBack(ini.path, point->ptr);
	free(point);
	ini.cursorText = sj_addBack(ini.path, text->ptr);
	free(text);
	ini.fontName = sj_addBack(ini.path, font->ptr);
	free(font);	
	
	//check for files
	if((access(ini.iniPath->ptr, F_OK)) == -1){
		fprintf(stderr, "Cannot find main.ini");
		return false;	
	}
	if((access(ini.cursorPoint->ptr, F_OK)) == -1){
		fprintf(stderr, "Cannot find cursor's point.png");
		return false;	
	}
	if((access(ini.cursorText->ptr, F_OK)) == -1){
		fprintf(stderr, "Cannot find cursor's text.png");
		return false;	
	}
	if((access(ini.fontName->ptr, F_OK)) == -1){
		fprintf(stderr, "Cannot find font file");
		return false;	
	}
	
	//open and read ini file	
	FILE* in = fopen("main.ini", "r");
	if(in == NULL){
		fprintf(stderr, "Could not open main.ini");
		return false;
	}
	
	char line[1024];
	while(fgets(line, sizeof(line), in)){
		if(sj_startsWith(line, "width")) ini.width = atoi(sj_remFrom(line, "width "));
		if(sj_startsWith(line, "height")) ini.height = atoi(sj_remFrom(line, "height "));
	
		if(sj_startsWith(line, "bgColor")){
			char delim[] = " ";
			char* token;
			char* str = sj_remFrom(line, "bgColor ");
			int i = 0;
			for(token = strtok(str, delim); token; token = strtok(NULL, delim)){
				ini.bgColor[i++] = (GLubyte)(atoi(token));
			}
		}
		if(sj_startsWith(line, "bordColor")){
			char delim[] = " ";
			char* token;
			char* str = sj_remFrom(line, "bordColor ");
			int i = 0;
			for(token = strtok(str, delim); token; token = strtok(NULL, delim)){
				ini.bordColor[i++] = (GLubyte)(atoi(token));
			}
		}
		if(sj_startsWith(line, "widColor")){
			char delim[] = " ";
			char* token;
			char* str = sj_remFrom(line, "widColor ");
			int i = 0;
			for(token = strtok(str, delim); token; token = strtok(NULL, delim)){
				ini.widColor[i++] = (GLubyte)(atoi(token));
			}
		}
		if(sj_startsWith(line, "widOverColor")){
			char delim[] = " ";
			char* token;
			char* str = sj_remFrom(line, "widOverColor ");
			int i = 0;
			for(token = strtok(str, delim); token; token = strtok(NULL, delim)){
				ini.widOverColor[i++] = (GLubyte)(atoi(token));
			}
		}
		if(sj_startsWith(line, "widDisColor")){
			char delim[] = " ";
			char* token;
			char* str = sj_remFrom(line, "widDisColor ");
			int i = 0;
			for(token = strtok(str, delim); token; token = strtok(NULL, delim)){
				ini.widDisColor[i++] = (GLubyte)(atoi(token));
			}
		}
		if(sj_startsWith(line, "scrollColor")){
			char delim[] = " ";
			char* token;
			char* str = sj_remFrom(line, "scrollColor ");
			int i = 0;
			for(token = strtok(str, delim); token; token = strtok(NULL, delim)){
				ini.scrollColor[i++] = (GLubyte)(atoi(token));
			}
		}
		if(sj_startsWith(line, "fontColor")){
			char delim[] = " ";
			char* token;
			char* str = sj_remFrom(line, "fontColor ");
			int i = 0;
			for(token = strtok(str, delim); token; token = strtok(NULL, delim)){
				ini.fontColor[i++] = (GLubyte)(atoi(token));
			}
		}
		if(sj_startsWith(line, "areaColor")){
			char delim[] = " ";
			char* token;
			char* str = sj_remFrom(line, "areaColor ");
			int i = 0;
			for(token = strtok(str, delim); token; token = strtok(NULL, delim)){
				ini.areaColor[i++] = (GLubyte)(atoi(token));
			}
		}
	}

	fclose(in);	
	
	//SDL_PixelFormat
	if(SDL_BYTEORDER == SDL_BIG_ENDIAN){
		ini.rmask = 0xff000000;
		ini.gmask = 0x00ff0000;
		ini.bmask = 0x0000ff00;
		ini.amask = 0x000000ff;
	}else{
		ini.rmask = 0x000000ff;
		ini.gmask = 0x0000ff00;
		ini.bmask = 0x00ff0000;
		ini.amask = 0xff000000;
	}
	
	return true;
}

