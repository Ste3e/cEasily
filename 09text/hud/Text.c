#include "../All.h"
#include "Text.h"

//structs
typedef struct WordNode{
	int code, glLen;
	Str* word;
}WordNode;

//exceptions
extern void sj_handleException(bool fatal, bool display, const char* msg, const char* tech);//main.c

//struct functions
static void setSize(Text* this);

//forward declarations
static void splitToWords(void);
static void buildToLines(void);

//variables
static Text* this;


Text* sj_newText(char* str){
	this = malloc(sizeof(Text));
	if(this == NULL) sj_handleException(true, false, "malloc failed to allocate memory for Text object\n", NULL);
	
	this->setSize = setSize;
	
	this->listWords = sj_newList();
	this->listLines = sj_newList();
	this->original = sj_newStr(str);
	this->width = 0;
	this->height = 0;
	
	return this;
}
void sj_freeText(Text* t){
	sj_freeStr(t->original);
	int i;
	for(i = 0; i < t->listLines->count; i++){
		Str* tmp = (Str*)t->listLines->get(t->listLines, i);
		sj_freeStr(tmp);
	}
	sj_freeList(t->listLines);
	for(i = 0; i < t->listWords->count; i++){
		Str* tmp = (Str*)t->listWords->get(t->listWords, i);
		sj_freeStr(tmp);
	}
	sj_freeList(t->listWords);
	free(t);
}


//struct functions
static void setSize(Text* text){
	this = text;
	
	if(this->width == 0){//is label
		TTF_SizeText(ini.font, this->original->ptr, &this->width, &this->height);
		if(this->width > ini.width) sj_handleException(false, true, "Label is larger than gui screen.", NULL);
		if(this->height < ini.buttonHeight) this->height = ini.buttonHeight;
		this->listLines->add(this->listLines, this->original);
	}else{//is text area
		int tmp;
		TTF_SizeText(ini.font, "Zg", &tmp, &this->height);
		if(this->height < ini.buttonHeight) this->height = ini.buttonHeight;
		splitToWords();	
		buildToLines();
	}
}

static void splitToWords(void){
	int w, h;
	int i = 0, wptr = 0;
	int maxChar = 50;
	char word[maxChar];
	
	while(this->original->ptr[i] != '\0'){
		if(wptr == maxChar){
			sj_handleException(false, true, "The program assumes there are no words longer than 50 characters. Code is in splitToWords of Text.c", NULL);	
			break;
		}
				
		if(isalnum(this->original->ptr[i])){
			word[wptr++] = this->original->ptr[i];
		}else{
			if(wptr > 0){
				word[wptr] = '\0';
				WordNode* tmp = malloc(sizeof(WordNode));
				if(tmp == NULL) sj_handleException(true, false, "Failed to malloc WordNode in Text.c.", NULL);
						
				TTF_SizeText(ini.font, word, &w, &h);
				tmp->code = ALNUM;
				tmp->glLen = w;
				tmp->word = sj_newStr(word);
				
				this->listWords->add(this->listWords, tmp);
				wptr = 0;
			}
			
			if(ispunct(this->original->ptr[i])){
				WordNode* tmp = malloc(sizeof(WordNode));
				if(tmp == NULL) sj_handleException(true, false, "Failed to malloc WordNode in Text.c.", NULL);
				
				char punk[2];
				punk[0] = this->original->ptr[i];
				punk[1] = '\0';
				TTF_SizeText(ini.font, punk, &w, &h);
				tmp->code = PUNCT;
				tmp->glLen = w;
				tmp->word = sj_newStr(punk);
				
				this->listWords->add(this->listWords, tmp);
				wptr = 0;
			}
			
			if(isspace(this->original->ptr[i])){
				WordNode* tmp = malloc(sizeof(WordNode));
				if(tmp == NULL) sj_handleException(true, false, "Failed to malloc WordNode in Text.c.", NULL);

				if(this->original->ptr[i] == ' '){	
					TTF_SizeText(ini.font, " ", &w, &h);
					tmp->code = SPACE;
					tmp->glLen = w;
					tmp->word = sj_newStr(" ");
				}
				if(this->original->ptr[i] == '\t'){
					char tab[ini.tabSize];
					int j;
					for(j = 0; j < ini.tabSize; j++){
						tab[j] = ' ';
					}
					tab[j] = '\0';					
					
					TTF_SizeText(ini.font, tab, &w, &h);
					tmp->code = TAB;
					tmp->glLen = w;
					tmp->word = sj_newStr(tab);
				}
				if(this->original->ptr[i] == '\n'){
					tmp->code = NEWLINE;
					tmp->glLen = 0;
					tmp->word = sj_newStr("\n");
				}
				
				this->listWords->add(this->listWords, tmp);
				wptr = 0;
			}			
		}	
		i++;
	}	
	
	/*for(i = 0; i < this->listWords->count; i ++){
		WordNode* node = (WordNode*)this->listWords->get(this->listWords, i);
		Str* str = (Str*)node->word;
		puts(str->ptr);
	}*/
}

static void buildToLines(void){
	char line[this->width];	
	int len = 0;
	int lineLen = this->width - (ini.buttonHeight + 2 * ini.borderSize);
	int i, j;
	for(j = 0; j < this->width; j++){
		line[j] = '\0';
	}
		
	for(i = 0; i < this->listWords->count; i++){
		WordNode* tmp = (WordNode*)this->listWords->get(this->listWords, i);
		
		switch(tmp->code){
		case ALNUM:
			{
				Str* str = (Str*)tmp->word;
				if(len + tmp->glLen < lineLen){					
					strcat(line, str->ptr);
					len += tmp->glLen;					
				}else{
					this->listLines->add(this->listLines, sj_newStr(line));
					len = 0;
					for(j = 0; j < this->width; j++){
						line[j] = '\0';
					}
					strcat(line, str->ptr);
					len = tmp->glLen;
				}
			}
			break;
		case PUNCT:
			{
				Str* str = (Str*)tmp->word;
				strcat(line, str->ptr);
				len += tmp->glLen;					
				
				if(len >= lineLen){			
					this->listLines->add(this->listLines, sj_newStr(line));
					len = 0;
					for(j = 0; j < this->width; j++){
						line[j] = '\0';
					}
				}
			}
			break;
		case SPACE:
			{
				if(len + tmp->glLen < lineLen){
					strcat(line, " ");
					len += tmp->glLen;					
				}
			}
			break;
		case TAB:
			{
				Str* str = (Str*)tmp->word;
				if(len + tmp->glLen < lineLen){
					strcat(line, str->ptr);
					len += tmp->glLen;					
				}else{
					this->listLines->add(this->listLines, sj_newStr(line));
					len = 0;
					for(j = 0; j < this->width; j++){
						line[j] = '\0';
					}
					strcat(line, str->ptr);
					len = tmp->glLen;
				}
			}
			break;
		case NEWLINE:
			{
				if(len > 0){
					this->listLines->add(this->listLines, sj_newStr(line));
				}else{
					this->listLines->add(this->listLines, sj_newStr(" "));
				}
				len = 0;
				for(j = 0; j < this->width; j++){
					line[j] = '\0';
				}
			}
			break;
			
		default:
			break;
			
		}			
	}
	
	if(len > 0){
		this->listLines->add(this->listLines, sj_newStr(line));
	}
	
	
	/*for(i = 0; i < this->listLines->count; i++){
		Str* tmp = (Str*)this->listLines->get(this->listLines, i);
		printf("Line is: %s\n", tmp->ptr);
	}*/
}