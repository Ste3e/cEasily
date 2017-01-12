#ifndef USREVENT_H
#define USREVENT_H

struct UsrEvent;
typedef struct UsrEvent{
	int idat;
	char* sdat;
}UsrEvent;

UsrEvent* sj_UsrEvent(void);
void sj_freeUsrEvent(UsrEvent* this);

#endif

