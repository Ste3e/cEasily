#include "All.h"
#include "base.h"

//exceptions
extern void sj_handleException(bool fatal, bool display, const char* msg, const char* tech);

//forward declarations
static void checkError(void);
static bool readIni(void);

static SDL_GLContext context;

void sj_buildWindow(void){
	if(SDL_Init(SDL_INIT_VIDEO) == -1) sj_handleException(true, false, "SDL_ video init fail.", SDL_GetError());
	
	if(!readIni()) sj_handleException(true, false, "SDL create window fail.", SDL_GetError());
	
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 2);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	
	GLint flags = SDL_WINDOW_OPENGL | SDL_WINDOW_BORDERLESS | SDL_WINDOW_SHOWN;
	win = SDL_CreateWindow("cGuigle", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, ini.width, ini.height, flags);
	if(win == NULL) sj_handleException(true, false, "SDL create window fail.", SDL_GetError());
	
	context = SDL_GL_CreateContext(win);
	if(context == NULL) sj_handleException(true, false, "SDL create context fail.", SDL_GetError());
	
	int err=glewInit();
	if(err != GLEW_OK) sj_handleException(true, false, "Glew failed to initialize\n", NULL);
	
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
	sj_handleException(false, false, "OpenGL renderer: ", renderer);
	
	const char* version = (const char*) glGetString(GL_VERSION);
	sj_handleException(false, false, "OpenGL version: ", version); 
	
	char c = *version;
	if(c == '1' || c == '2'){
		sj_handleException(true, false, "Your openGL version is too low to run this program.", NULL);
	}
	
	GLenum error = glGetError();
	switch(error){
	 	case GL_NO_ERROR: break;
	 	case GL_INVALID_ENUM: sj_handleException(true, false, "invalid enum", NULL); break;
	 	case GL_INVALID_VALUE: sj_handleException(true, false, "invalid value", NULL); break;
	 	case GL_OUT_OF_MEMORY: sj_handleException(true, false, "out of memory", NULL); break;
	 	case GL_INVALID_FRAMEBUFFER_OPERATION: sj_handleException(true, false, "invalid framebuffer operation", NULL); break;
	 	default: break;	
	}
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
	Str* fontPath;
	Str* dingPath;
	Str* fontPre = sj_addBack(ini.path, font->ptr);
	free(font);	
	
	//check for files
	if((access(ini.iniPath->ptr, F_OK)) == -1) sj_handleException(true, false, "Cannot find main.ini, exiting.", NULL);
	
	if((access(ini.cursorPoint->ptr, F_OK)) == -1) sj_handleException(true,  false, "Cannot find cursor's point.png in rs/images, exiting.", NULL);
	
	if((access(ini.cursorText->ptr, F_OK)) == -1) sj_handleException(true,  false, "Cannot find cursor's text.png in rs/images, exiting.", NULL);	
	
	//open and read ini file	
	FILE* in = fopen("main.ini", "r");
	if(in == NULL) sj_handleException(true,  false, "Cannot find main.ini. Should be folder alongside main.c.", NULL);
	
	char line[1024];
	while(fgets(line, sizeof(line), in)){
		if(sj_startsWith(line, "width")) ini.width = atoi(sj_remFrom(line, "width "));
		if(sj_startsWith(line, "height")) ini.height = atoi(sj_remFrom(line, "height "));
		if(sj_startsWith(line, "cursorSize")) ini.cursorSize = atoi(sj_remFrom(line, "cursorSize "));
		if(sj_startsWith(line, "mouseSpeed")) ini.mouseSpeed = atof(sj_remFrom(line, "mouseSpeed "));
		if(sj_startsWith(line, "fontSize")) ini.fontSize = atoi(sj_remFrom(line, "fontSize "));
		if(sj_startsWith(line, "buttonHeight")) ini.buttonHeight = atoi(sj_remFrom(line, "buttonHeight "));
		if(sj_startsWith(line, "barSize")) ini.barSize = atoi(sj_remFrom(line, "barSize "));
		if(sj_startsWith(line, "borderSize")) ini.borderSize = atoi(sj_remFrom(line, "borderSize "));
		if(sj_startsWith(line, "dialogFactor")) ini.dialogFactor = atof(sj_remFrom(line, "dialogFactor "));
		if(sj_startsWith(line, "tabSize")) ini.tabSize = atoi(sj_remFrom(line, "tabSize "));
	
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
		//set up fonts
		if(sj_startsWith(line, "fontName")){
			char* tmp = sj_remFrom(line, "fontName ");
			fontPath = sj_addBack(fontPre, tmp);
			dingPath = sj_addBack(fontPre, "dings.ttf");
		}		
	}

	fclose(in);	
	
	//continue setting up fonts
	if(TTF_Init() == -1) sj_handleException(true, false, "Cannot initialize SDL_ttf", NULL);
	
	if((access(fontPath->ptr, F_OK)) == -1) sj_handleException(true, false, "Cannot find font ", fontPath->ptr);
	
	if((access(dingPath->ptr, F_OK)) == -1) sj_handleException(true, false, "Cannot find ding ttf in rs/fonts folder.", NULL);
		
	ini.font = TTF_OpenFont(fontPath->ptr, ini.fontSize);
	if(ini.font == NULL) sj_handleException(true, false, "Cannot open font ", fontPath->ptr);
	
	ini.dings = TTF_OpenFont(dingPath->ptr, ini.fontSize);
	if(ini.font == NULL) sj_handleException(true, false, "Cannot open ding ttf.", NULL);
	
	sj_freeStr(fontPath);
	sj_freeStr(dingPath);
	
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

