#ifndef TEXT_H
#define TEXT_H

#define ALNUM 0
#define PUNCT 1
#define SPACE 2
#define TAB 3
#define NEWLINE 4

struct Text;
typedef struct Text{
	Str* original;
	List* listWords;
	List* listLines;
	int width, height;
	
	void (*setSize)(struct Text* this);
}Text;

Text* sj_newText(char* str);
void sj_freeText(Text* this);

#endif

