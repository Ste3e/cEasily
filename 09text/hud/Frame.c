#include "../All.h"
#include "Frame.h"

//exceptions
extern void sj_handleException(bool fatal, bool display, const char* msg, const char* tech);//main.c

//struct functions
static void draw(Frame* this);

//helper functions
static void makeVao(void);
static void buildShader(void);
static void makeMap(void);

//variables
static Frame* this;
static GLuint vao, shad;
static int vcount;


Frame* sj_newFrame(void){
	if(this != NULL) return this;
	this = (Frame*)malloc(sizeof(Frame));
	if(this == NULL) sj_handleException(true, false, "Failed to malloc memory for Frame.", NULL);
		
	this->draw = draw;
	
	this->imgw = ini.width;
	this->imgh = ini.height;
	this->hidden = false;
	
	makeVao();
	buildShader();
	makeMap();
		
	return this;
}

void sj_freeFrame(Frame* frame){
	free(frame);
}

//helper functions
void makeVao(void){
	int floatCount;
	
	float verts[] = {-1, -1, -1, 1, -1, -1, 1, 1, -1, -1, -1, -1, 1, 1, -1, -1, 1, -1};
	float coords[] = {0, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0};
	floatCount = LEN(verts);
	vcount = floatCount / 3;	
	
	glGenVertexArrays(1, &vao);
	if(vao == 0) sj_handleException(true, false, "GL failed to allocate id for Frame vao.", NULL);
	glBindVertexArray(vao);
	
	GLuint vbo, cbo;
	glGenBuffers(1, &vbo);
	if(vbo == 0) sj_handleException(true, false, "GL failed to allocate id for Frame vbo.", NULL);
	glGenBuffers(1, &cbo);
	if(cbo == 0) sj_handleException(true, false, "GL failed to allocate id for Frame cbo.", NULL);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, floatCount * sizeof(float), &verts, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		
	glBindBuffer(GL_ARRAY_BUFFER, cbo);
	glBufferData(GL_ARRAY_BUFFER, floatCount * sizeof(float), &coords, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
		
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
void buildShader(void){
	const char* vert = 
"#version 330\n"
"layout(location = 0) in vec3 pos;\n"
"layout(location = 1) in vec2 coord;\n"
"out vec2 coords;\n"
"void main(void){\n"
"coords=coord.st;\n"
"gl_Position = vec4(pos.x, pos.y, pos.z, 1);\n"
"}\n";

	const char* frag =
"#version 330\n"
"uniform sampler2D cMap;\n"
"in vec2 coords;\n"
"void main(void){\n"
"gl_FragColor = texture2D(cMap, coords.st);\n"
"}\n";

	shad = glCreateProgram();
	if(shad == 0) sj_handleException(true, false, "GL failed to assign Frame shader id.", NULL);
		
	GLuint vshad = glCreateShader(GL_VERTEX_SHADER);
	if(vshad == 0) sj_handleException(true, false, "GL failed to assign Frame vertex shader id.", NULL);
	glShaderSource(vshad, 1, &vert, NULL);
	glCompileShader(vshad);
	int status, len;
	glGetShaderiv(vshad, GL_COMPILE_STATUS, &status);
	if(status == GL_FALSE){
		glGetShaderiv(vshad, GL_INFO_LOG_LENGTH, &len);
		char error[len];
		glGetShaderInfoLog(vshad, len, NULL, (char*)error);
		sj_handleException(true, false, "GL failed to compile Frame's vertex shader with ", error);
	}
	
	GLuint fshad = glCreateShader(GL_FRAGMENT_SHADER);
	if(fshad == 0) sj_handleException(true, false, "GL failed to assign Frame fragment shader id.", NULL);
	glShaderSource(fshad, 1, &frag, NULL);
	glCompileShader(fshad);
	glGetShaderiv(vshad, GL_COMPILE_STATUS, &status);
	if(status == GL_FALSE){
		glGetShaderiv(fshad, GL_INFO_LOG_LENGTH, &len);
		char error[len];
		glGetShaderInfoLog(fshad, len, NULL, (char*)error);
		sj_handleException(true, false, "GL failed to compile Frame's fragment shader with ", error);
	}
	
	glAttachShader(shad, vshad);
	glAttachShader(shad, fshad);
	glLinkProgram(shad);
	glGetShaderiv(shad, GL_LINK_STATUS, &status);
	if(status == GL_FALSE){
		glGetShaderiv(shad, GL_INFO_LOG_LENGTH, &len);
		char error[len];
		glGetShaderInfoLog(shad, len, NULL, (char*)error);
		sj_handleException(true, false, "GL failed to link Frame's shader with ", error);
	}
	
	//shader variable locations
	GLint cMapLoc = glGetUniformLocation(shad, "cMap");
	if(cMapLoc == -1) sj_handleException(false, false, "cMapLoc from Frame not got.", NULL);
		
	glUseProgram(shad);
	glUniform1i(cMapLoc, 0);
	glUseProgram(0);	
}

void makeMap(void){
	SDL_Surface* base = SDL_CreateRGBSurface(0, this->imgw, this->imgh, 32, ini.rmask, ini.gmask, ini.bmask, ini.amask);
	if(base == NULL) sj_handleException(true, false, "SDL failed to create Frame img surface with ", SDL_GetError());
		
	SDL_FillRect(base, NULL, SDL_MapRGBA(base->format, ini.bordColor[0], ini.bordColor[1], ini.bordColor[2], ini.bordColor[3]));
		
	SDL_Rect inner;
	inner.x = 1;
	inner.y = 1;
	inner.w = base->w - 2;
	inner.h = base->h - 2;
	SDL_FillRect(base, &inner, SDL_MapRGBA(base->format,  ini.bgColor[0], ini.bgColor[1], ini.bgColor[2], ini.bgColor[3]));
	
	glGenTextures(1, &this->map);
	if(this->map == 0){
		SDL_FreeSurface(base);
		sj_handleException(false, false, "GL failed to provide texture id for frame with ", SDL_GetError());
	}
	glBindTexture(GL_TEXTURE_2D, this->map);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, base->w, base->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, base->pixels);
	
	glBindTexture(GL_TEXTURE_2D, 0);
	SDL_FreeSurface(base);
}

//DRAW
static void draw(Frame* frame){
	this = frame;
	if(this->hidden) return;
	
	
	glUseProgram(shad);
	glBindVertexArray(vao);
			
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->map);
	
	glDrawArrays(GL_TRIANGLES, 0, vcount);
		
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);		
	glBindVertexArray(0);	
}
