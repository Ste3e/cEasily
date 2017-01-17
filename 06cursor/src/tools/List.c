#include "../../All.h"

//exceptions
extern void sj_handleException(bool fatal, const char* msg, const char* tech);

//struct functions
static void add(List* l, void* item);
static void* get(List* l, int index);

static List* this;

List* sj_newList(void){
	this = malloc(sizeof(List));
	if(this == NULL) sj_handleException(true, "Failed to malloc List.", NULL);
			
	this->count = 0;
	this->head = NULL;
	this->current = NULL;	
	
	this->add = add;
	this->get = get;
	
	return this;
}
void sj_freeList(List* l){
	this = l;
	ListNode* tmp;
	
	int i;
	for(i = 0; i < this->count; i ++){
		tmp = this->head;
		this->head = this->head->next;
		free(tmp);
	}
	free(l);
}

static void add(List* l, void* item){
	this = l;
	ListNode* tmp = malloc(sizeof(ListNode));
	if(this == NULL) sj_handleException(true, "Failed to malloc ListNode.", NULL);
	
	if(this->count == 0){		
		tmp->next = NULL;
		tmp->item = item;
		
		this->head = tmp;		
		this->current = tmp;
		this->count++;
	}else{
		tmp->next = NULL;
		tmp->item = item;
		
		this->current->next = tmp;//if current == head then head->next now == tmp
		this->current = tmp;//therefore current == head->next
		this->count ++;	
	}
	
}
static void* get(List* l, int index){
	this = l;
		
	ListNode* tmp = this->head;
	int i;
	int dx = index;
	if(dx >= this->count){
		dx = this->count -1;
		sj_handleException(false, "Some List has attempted to read unallocated memory. List has returned last valid entry.", NULL);	
	}
	
	for(i = 0; i < dx; i++){
		tmp = tmp->next;
	}
	
	return tmp->item;
}

