#include "Scene.h"
#include "../All.h"

static Scene* this;
static bool initialized = false;

//constructor destructor
Scene* sj_getScene(void){
	if(initialized) return this;
	initialized = true;
	
	this = malloc(sizeof(Scene));
	if(this == NULL){
		fprintf(stderr, "malloc failed to allocate memory for Scene\n");
		exit(EXIT_FAILURE);
	}
	
	return this;
}

void sj_freeScene(Scene* this){
	free(this);
}
