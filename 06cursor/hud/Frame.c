#include "../All.h"
#include "Frame.h"

//struct functions
static void draw(Frame* this);

//helper functions
static bool makeVao(void);
static bool buildShader(void);
static bool makeMap(void);

//variables
static Frame* this;
static bool initialized = false;
static GLuint vao, shad;
static int vcount;


Frame* sj_getFrame(void){
	this = (Frame*)malloc(sizeof(Frame));
	if(this == NULL){
		fprintf(stderr, "malloc failed to allocate memory for Frames\n");
		exit(EXIT_FAILURE);
	}
	
	this->draw = draw;
	
	this->offsets[0] = 1;
	this->offsets[1] = 1;
	this->offsets[2] = 1;
	this->offsets[3] = 1;
	this->imgw = ini.width;
	this->imgh = ini.height;
	this->hidden = false;
	
	if(!initialized){
		initialized = true;
		if(!makeVao()){
			fprintf(stderr, "Failed to create vao for Frames\n");
			exit(EXIT_FAILURE);
		}
		if(!buildShader()){
			fprintf(stderr, "Failed to create shader for Frames\n");
			exit(EXIT_FAILURE);
		}
	}
	if(!makeMap()){
			fprintf(stderr, "Failed to create texture map for Frames\n");
			exit(EXIT_FAILURE);
	}
	
	return this;
}

void sj_freeFrame(Frame* frame){
	free(frame);
}

//helper functions
bool makeVao(void){
	int floatCount;
	
	float verts[] = {-1, -1, -1, 1, -1, -1, 1, 1, -1, -1, -1, -1, 1, 1, -1, -1, 1, -1};
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
bool buildShader(void){
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
	GLint cMapLoc = glGetUniformLocation(shad, "cMap");
	if(cMapLoc == -1) puts("cMapLoc from Frames not got");
		
	glUseProgram(shad);
	glUniform1i(cMapLoc, 0);
	glUseProgram(0);	
	
	return true;
}

bool makeMap(void){
	SDL_Surface* base = SDL_CreateRGBSurface(0, this->imgw, this->imgh, 32, ini.rmask, ini.gmask, ini.bmask, ini.amask);
	if(base == NULL) return false;
		
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
		fprintf(stderr, "GL failed to provide texture id for frame");
	}
	glBindTexture(GL_TEXTURE_2D, this->map);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, base->w, base->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, base->pixels);
	
	glBindTexture(GL_TEXTURE_2D, 0);
	SDL_FreeSurface(base);
	
	return true;
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
