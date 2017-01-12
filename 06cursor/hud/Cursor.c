#include "..\All.h"
#include "Cursor.h"

//static struct plugins
static void draw(void);

//static helper functions
static bool makeVao(void);
static GLuint makeMap(Str* texPath);
static bool buildShader(void);

//variable list
static Cursor* this;
static bool initialized = false;
static GLuint img, pointimg, textimg;
static GLuint vao;
static GLuint shad, cPosLoc;
static int vcount;

Cursor* sj_getCursor(void){
	if(initialized) return this;
	initialized = true;
	this = malloc(sizeof(Cursor));
	if(this == NULL){
		fprintf(stderr, "malloc failed to allocate memory for Cursor\n");
		exit(EXIT_FAILURE);
	}
	
	this->draw = draw;
	
	if(!makeVao()){
		fprintf(stderr, "Failed to build vao for Cursor\n");
		exit(EXIT_FAILURE);
	}
	pointimg = makeMap(ini.cursorPoint);
	if(pointimg == 0){
		fprintf(stderr, "Failed to build texture for Cursor pointer\n");
		exit(EXIT_FAILURE);
	}
	textimg = makeMap(ini.cursorText);
	if(textimg == 0){
		fprintf(stderr, "Failed to build texture for Cursor text\n");
		exit(EXIT_FAILURE);
	}
	img = pointimg;
	if(!buildShader()){
		fprintf(stderr, "Failed to build shader for Cursor\n");
		exit(EXIT_FAILURE);
	}
	
	return this;
}

void sj_freeCursor(Cursor* this){
	
}

//helper functions
static bool makeVao(void){
	float cw = (float)ini.cursorSize / (float)ini.width;
	float ch = (float)ini.cursorSize / (float)ini.height;
	int floatCount;
	
	float verts[] = {-cw, -ch, -1, cw, -ch, -1, cw, ch, -1, -cw, -ch, -1, cw, ch, -1, -cw, ch, -1};
	float coords[] = {0, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0};
	floatCount = LEN(verts);
	vcount = floatCount / 3;	
	
	glGenVertexArrays(1, &vao);
	if(vao == 0) return false;
	glBindVertexArray(vao);
	
	GLuint vbo, cbo;
	glGenBuffers(1, &vbo);
	if(vbo == 0) return false;
	glGenBuffers(1, &cbo);
	if(cbo == 0) return false;	
	
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
	
	return true;
}

static GLuint makeMap(Str* texPath){
	SDL_Surface* base = IMG_Load(texPath->ptr);
	if(base == NULL) return 0;
	
	GLuint toret;
	glGenTextures(1, &toret);
	if(toret == 0) return toret;
	
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

static bool buildShader(void){
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
	if(shad == 0){
		fprintf(stderr, "GL failed to assign cursor program shader id\n");
		return false;	
	}
	
	GLuint vshad = glCreateShader(GL_VERTEX_SHADER);
	if(vshad == 0){
		fprintf(stderr, "GL failed to assign cursor vertex shader id\n");
		return false;	
	}
	glShaderSource(vshad, 1, &vert, NULL);
	glCompileShader(vshad);
	int status, len;
	glGetShaderiv(vshad, GL_COMPILE_STATUS, &status);
	if(status == GL_FALSE){
		glGetShaderiv(vshad, GL_INFO_LOG_LENGTH, &len);
		char error[len];
		glGetShaderInfoLog(vshad, len, NULL, (char*)error);
		fprintf(stderr, error);
		return false;
	}
	
	GLuint fshad = glCreateShader(GL_FRAGMENT_SHADER);
	if(fshad == 0){
		fprintf(stderr, "GL failed to assign cursor fragment shader id\n");
		return false;	
	}
	glShaderSource(fshad, 1, &frag, NULL);
	glCompileShader(fshad);
	glGetShaderiv(vshad, GL_COMPILE_STATUS, &status);
	if(status == GL_FALSE){
		glGetShaderiv(fshad, GL_INFO_LOG_LENGTH, &len);
		char error[len];
		glGetShaderInfoLog(fshad, len, NULL, (char*)error);
		fprintf(stderr, error);
		return false;
	}
	
	glAttachShader(shad, vshad);
	glAttachShader(shad, fshad);
	glLinkProgram(shad);
	glGetShaderiv(shad, GL_LINK_STATUS, &status);
	if(status == GL_FALSE){
		glGetShaderiv(shad, GL_INFO_LOG_LENGTH, &len);
		char error[len];
		glGetShaderInfoLog(shad, len, NULL, (char*)error);
		fprintf(stderr, error);
		return false;
	}
	
	//shader variable locations
	cPosLoc = glGetUniformLocation(shad, "cPos");
	if(cPosLoc == -1) puts("cPosLoc not got");
	
	GLint cMapLoc = glGetUniformLocation(shad, "cMap");
	if(cMapLoc == -1) puts("cMapLoc from cursor not got");
		
	glUseProgram(shad);
	glUniform1i(cMapLoc, 0);
	glUseProgram(0);	
	
	return true;	
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
