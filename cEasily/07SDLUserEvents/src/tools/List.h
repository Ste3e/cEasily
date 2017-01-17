#ifndef LIST_H
#define LIST_H

typedef struct ListNode{
  struct ListNode* next;
  
  void* item;
}ListNode;

struct List;
typedef struct List{
	int count;
	ListNode* head;
	ListNode* current;
	
	void (*add)(struct List* this, void* item);
	void* (*get)(struct List* this, int index);
	
}List;

List* sj_newList(void);
void sj_freeList(List* this);

#endif
