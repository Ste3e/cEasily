#include "../../All.h"
#include "UsrEvent.h"

static UsrEvent* this;

UsrEvent* sj_UsrEvent(void){
	this = malloc(sizeof(UsrEvent));
	if(this == NULL){
		fprintf(stderr, "malloc failed to allocate memory for UsrEvent\n");
		exit(EXIT_FAILURE);
	}
	
	this->idat = 0;
	this->sdat = NULL;
	
	return this;
}

void sj_freeUsrEvent(UsrEvent* ue){
	free(ue);	
}










