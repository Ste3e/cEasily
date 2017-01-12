#include "All.h"
#include "hud.h"

void sj_buildHud(void){
	List* list = sj_newList();
	list->add(list, sj_newStr("one"));
	list->add(list, sj_newStr("two"));
	list->add(list, sj_newStr("three"));
	list->add(list, sj_newStr("four"));
	
	printf("List contains: ");
	Str* s = (Str*)list->get(list, 0);	
	printf("%s, ", s->ptr);
	s = (Str*)list->get(list, 1);	
	printf("%s, ", s->ptr);
	s = (Str*)list->get(list, 2);	
	printf("%s, ", s->ptr);
	s = (Str*)list->get(list, 3);	
	printf("%s.\n", s->ptr);
	
	sj_freeList(list);		
}