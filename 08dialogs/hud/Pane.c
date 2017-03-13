#include "../All.h"
#include "Pane.h"
#include "Cursor.h"
#include "Frame.h"
#include "Dialog.h"
#include "Button.h"

//type hit
#define GUI 0
#define DLG 1

//widget type
#define BUTTON 1

//widget node
typedef struct Node{
	bool clean;
	int type;
	void* widget;
}Node;

//struct functions
static int addButton(char* text);
static void setSize(int index, int w, int h);
static void setLoc(int index, int x, int y, bool mainHud);
static void show(void);
static void draw(void);

//dialog functions
void sj_alert(char* msg);
void sj_dlgEvent(GuiEvent* ge);

//forward declarations
void unclean(void);

//variables
static Cursor* cursor;
static Frame* frame;
static Dialog* dlg;
static List* widList;
static int checkHit = GUI;

void sj_getPane(Pane* pane){
	cursor = sj_newCursor();
	frame = sj_newFrame();
	dlg = sj_newDialog(frame->map);
	widList = sj_newList();
	
	pane->addButton = addButton;
	pane->setSize = setSize;
	pane->setLoc = setLoc;
	pane->show = show;
	pane->draw = draw;
}
void sj_freePane(void){
	sj_freeFrame(frame);
	int i;
	for(i = 0; i < widList->count; i++){
		Node* tmp = (Node*)widList->get(widList, i);	
		switch(tmp->type){
			case BUTTON:
				{
					Button* b = ((Button*)tmp->widget);
					sj_freeButton(b);
				}
				break;	
		
			default:
				break;
		}
	}
	sj_freeList(widList);
}

//add widgets
static int addButton(char* text){
	Node* node = malloc(sizeof(Node));
	if(node == NULL){
		fprintf(stderr, "Failed to malloc Node.");
		exit(-1);
	}	
	int id = widList->count;
	node->clean = false;
	node->type = BUTTON;
	node->widget = sj_newButton(id, frame->map, sj_newStr(text));	
	widList->add(widList, node);	
	
	return id;
}

//alter widgets
static void setSize(int index, int w, int h){
	Node* tmp = (Node*)widList->get(widList, index);		
	
	switch(tmp->type){
	case BUTTON:
		{			
			Button* b = ((Button*)tmp->widget);
			b->setSize(b, w, h);
		}
		break;	
		
	default:
		break;
	
	}
}

static void setLoc(int index, int x, int y, bool mainHud){
	int xpos, ypos;
	if(mainHud){
		if(x < 0) xpos = ini.width + x; else xpos = x;
		if(y < 0) ypos = ini.height + y; else ypos = y;
	}else{
		//widget is on dialog Frame	
	}
		
	Node* tmp = (Node*)widList->get(widList, index);
		
	switch(tmp->type){
	case BUTTON:
		{
			Button* b = ((Button*)tmp->widget);
			b->setLoc(b, xpos, ypos);
		}
		break;	
	
	default:
		break;	
	}	
}


//dialogs
void unclean(void){
	int i;
	for(i = 0; i < widList->count; i++){
		Node* tmp = (Node*)widList->get(widList, i);
		tmp->clean = false;
	}
}
void sj_alert(char* msg){
	checkHit = DLG;
	unclean();
	dlg->alert(msg);	
}
void sj_dlgEvent(GuiEvent* ge){
	dlg->dlgEvent(ge);	
	show();
	checkHit = GUI;
}

//update and draw widgets
static void show(){
	int i;
	for(i = 0; i < widList->count; i++){
		Node* tmp = (Node*)widList->get(widList, i);
		if(tmp->clean) continue;
		
		switch(tmp->type){
		case BUTTON:
			{
				Button* b = ((Button*)tmp->widget);
				b->build(b, false);	
			}
			break;	
					
		default:
			break;
		}		
	}	
}

static void draw(void){
	switch(checkHit){
	case GUI:
		{
			int i;
			for(i = 0; i < widList->count; i++){
				Node* tmp = (Node*)widList ->get(widList, i);
				switch(tmp->type){
					case BUTTON:
						{
							Button* b = ((Button*)tmp->widget);
							b->checkHit(b);
						}
						break;
												
					default:
						break;
				}
			}
		}
		break;
	
	case(DLG):
		{
			dlg->checkHit();	
		}
		break;
		
	default:
		break;
	}
		
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	frame->draw(frame);
	cursor->draw();
	glDisable(GL_BLEND);
}




