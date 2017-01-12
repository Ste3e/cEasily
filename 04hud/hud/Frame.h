#ifndef FRAME_H
#define FRAME_H

struct Frame;
typedef struct Frame{
	GLuint map, trimLoc;
	float offsets[4];
	int imgw, imgh;
	bool hidden;
	
	void (*draw)(struct Frame* this);	
}Frame;

//constructor destructor
Frame* sj_getFrame(void);
void sj_destroyFrame(Frame* this);

#endif
