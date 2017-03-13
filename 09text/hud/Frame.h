#ifndef FRAME_H
#define FRAME_H

struct Frame;
typedef struct Frame{
	GLuint map;
	float offsets[4];
	int imgw, imgh;
	bool hidden;
	
	void (*draw)(struct Frame* this);	
}Frame;

//constructor destructor
Frame* sj_newFrame(void);
void sj_freeFrame(Frame* this);

#endif
