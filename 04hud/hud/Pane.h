#ifndef PANE_H
#define PANE_H

#include "../All.h"

typedef struct Pane{

	void (*draw)(void);	
}Pane;

void sj_getPane(Pane* pane);


#endif
