#include "..\All.h"
#include "Cursor.h"

//exceptions
extern void sj_handleException(bool fatal, bool display, const char* msg, const char* tech);//main.c

//static struct functions
static void draw(void);

//static helper functions
static void makeVao(void);
static GLuint makeMap(Str* texPath);
static void buildShader(void);

//variable list
static Cursor* this;
static bool initialized = false;
static GLuint img, pointimg, textimg;
static GLuint vao;
static GLuint shad, cPosLoc;
static int vcount;

Cursor* sj_newCursor(void){
	if(initialized) return this;
	initialized = true;
	this = malloc(sizeof(Cursor));
	if(this == NULL) sj_handleException(true, false, "malloc failed to allocate memory for Cursor.", NULL);
	
	this->draw = draw;
	
	makeVao();
	pointimg = makeMap(ini.cursorPoint);
	textimg = makeMap(ini.cursorText);
	img = pointimg;
	buildShader();
		
	return this;
}

void sj_freeCursor(Cursor* cursor){
	free(cursor);
}

//helper functions
static void makeVao(void){
	float cw = (float)ini.cursorSize / (float)ini.width;
	float ch = (float)ini.cursorSize / (float)ini.height;
	int floatCount;
	
	float verts[] = {-cw, -ch, -1, cw, -ch, -1, cw, ch, -1, -cw, -ch, -1, cw, ch, -1, -cw, ch, -1};
	float coords[] = {0, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0};
	floatCount = LEN(verts);
	vcount = floatCount / 3;	
	
	glGenVertexArrays(1, &vao);
	if(vao == 0) sj_handleException(true, false, "SDL failed to assign vao for Cursor. ", SDL_GetError());
	glBindVertexArray(vao);
	
	GLuint vbo, cbo;
	glGenBuffers(1, &vbo);
	if(vbo == 0) sj_handleException(true, false, "SDL failed to assign vbo for Cursor. ", SDL_GetError());
	glGenBuffers(1, &cbo);
	if(cbo == 0) sj_handleException(true, false, "SDL failed to assign cbo for Cursor. ", SDL_GetError());
	
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

static GLuint makeMap(Str* texPath){
	SDL_Surface* base = IMG_Load(texPath->ptr);
	if(base == NULL) sj_handleException(true, false, "SDL failed to create image Surface for Cursor. ", SDL_GetError());
	
	GLuint toret;
	glGenTextures(1, &toret);
	if(toret == 0) sj_handleException(true, false, "GL failed to assign id for Cursor texture.", NULL);
	
	glBindTexture(GL_TEXTURE_2D, toret);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, base->w, base->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, base->pixels);
		
	glBindTexture(GL_TEXTURE_2D, 0);
	SDL_FreeSurface(base);
		
	return toret;
}

static void buildShader(void){
	const char* vert = 
"#version 330\n"
"layout(location = 0) in vec3 pos;\n"
"layout(location = 1) in vec2 coord;\n"
"uniform vec2 cPos;\n"
"out vec2 coords;\n"
"void main(void){\n"
"coords=coord.st;\n"
"gl_Position = vec4(pos.x + cPos.x, pos.y + cPos.y, pos.z, 1.0);\n"
"}\n";

	const char* frag =
"#version 330\n"
"uniform sampler2D cMap;\n"
"in vec2 coords;\n"
"void main(void){\n"
"gl_FragColor = texture2D(cMap, coords.st);\n"
"}\n";

	shad = glCreateProgram();
	if(shad == 0) sj_handleException(true, false, "GL failed to assign id for Cursor shader program.", NULL);
		
	GLuint vshad = glCreateShader(GL_VERTEX_SHADER);
	if(vshad == 0) sj_handleException(true, false, "GL failed to assign id for Cursor vertex shader.", NULL);
	glShaderSource(vshad, 1, &vert, NULL);
	glCompileShader(vshad);
	int status, len;
	glGetShaderiv(vshad, GL_COMPILE_STATUS, &status);
	if(status == GL_FALSE){
		glGetShaderiv(vshad, GL_INFO_LOG_LENGTH, &len);
		char error[len];
		glGetShaderInfoLog(vshad, len, NULL, (char*)error);
		sj_handleException(true, false, "GL failed to compile Cursor vertex shader with: ", error);
	}
	
	GLuint fshad = glCreateShader(GL_FRAGMENT_SHADER);
	if(fshad == 0) sj_handleException(true, false, "GL failed to assign id for Cursor fragment shader.", NULL);
	glShaderSource(fshad, 1, &frag, NULL);
	glCompileShader(fshad);
	glGetShaderiv(vshad, GL_COMPILE_STATUS, &status);
	if(status == GL_FALSE){
		glGetShaderiv(fshad, GL_INFO_LOG_LENGTH, &len);
		char error[len];
		glGetShaderInfoLog(fshad, len, NULL, (char*)error);
		sj_handleException(true, false, "GL failed to compile Cursor fragment shader with: ", error);
	}
	
	glAttachShader(shad, vshad);
	glAttachShader(shad, fshad);
	glLinkProgram(shad);
	glGetShaderiv(shad, GL_LINK_STATUS, &status);
	if(status == GL_FALSE){
		glGetShaderiv(shad, GL_INFO_LOG_LENGTH, &len);
		char error[len];
		glGetShaderInfoLog(shad, len, NULL, (char*)error);
		sj_handleException(true, false, "GL failed to link Cursor vertex and fragment shaders with: ", error);
	}
	
	//shader variable locations
	cPosLoc = glGetUniformLocation(shad, "cPos");
	if(cPosLoc == -1) sj_handleException(false, false, "cPosLoc not got in Cursor.c", NULL);
	
	GLint cMapLoc = glGetUniformLocation(shad, "cMap");
	if(cMapLoc == -1) sj_handleException(false, false, "cMapLoc not got in Cursor.c", NULL);
		
	glUseProgram(shad);
	glUniform1i(cMapLoc, 0);
	glUseProgram(0);
}

//struct functions
static void draw(){	
	glUseProgram(shad);
	glBindVertexArray(vao);
	glUniform2f(cPosLoc, sj_xclip, sj_yclip);
		
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, img);
	
	glDrawArrays(GL_TRIANGLES, 0, vcount);
		
	glBindTexture(GL_TEXTURE_2D, 0);		
	glBindVertexArray(0);
	glUseProgram(0);	
}
