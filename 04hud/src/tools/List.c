#include "../../All.h"

static void reset(List* l);
static void add(List* l, void* item);
static void* get(List* l, int index);

static List* this;

List* sj_newList(void){
	this = malloc(sizeof(List));
	if(this == NULL){
		fprintf(stderr, "Failed to malloc List.");
		exit(-1);
	}
		
	this->count = 0;
	this->head = NULL;
	this->current = NULL;	
	
	this->reset = reset;
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

static void reset(struct List* l){
	this = l;
	
	ListNode* tmp = this->head;
	int i;
	
	for(i = 0; i < this->count; i++){
		tmp = tmp->next;
	}
	
	this->head = NULL;
	this->current = NULL;
	this->count = 0;	
}

static void add(List* l, void* item){
	this = l;
	ListNode* tmp = malloc(sizeof(ListNode));
		if(this == NULL){
		fprintf(stderr, "Failed to malloc List.");
		exit(-1);
	}
	
	if(this->count == 0){		
		tmp->next = NULL;
		tmp->item = item;
		this->head = tmp;		
		this->current = this->head;
		this->count++;
	}else{
		tmp->next = NULL;
		tmp->item = item;
		
		this->current->next = tmp;
		this->current = tmp;
		this->count ++;	
	}
	
}
static void* get(List* l, int index){
	this = l;
	
	ListNode* tmp = this->head;
	int i;
	
	for(i = 0; i < index; i++){
		tmp = tmp->next;
	}
	
	return tmp->item;
}

