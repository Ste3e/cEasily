#ifndef CURSOR_H
#define CURSOR_H

struct Cursor;
typedef struct Cursor{
	void (*draw)(void);
}Cursor;

//constructor destructor
Cursor* sj_newCursor(void);
void sj_freeCursor(Cursor* this);

#endif
