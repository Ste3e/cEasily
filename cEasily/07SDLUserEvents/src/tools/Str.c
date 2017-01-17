#include "../../All.h"

//exceptions
extern void sj_handleException(bool fatal, const char* msg, const char* tech);

Str* sj_newStr(const char* src){
	size_t len = strlen(src);
		
	Str* toret = malloc(sizeof(Str));
	if(toret == NULL) return NULL;
	
	toret->len = len;
	toret->ptr = malloc((len + 1) * sizeof(char));
	if(toret->ptr == NULL) sj_handleException(true, "Failed to malloc Str struct.", NULL);
	
	if(len == 0){
		char empty[1];
		empty[0] = '\0';
		strcpy(toret->ptr, empty);
	}else{
		strcpy(toret->ptr, src);
	}
	assert(*(toret->ptr + toret->len) == '\0');
			
	return toret;		
}

Str* sj_copyStr(const Str* src){	
	Str* toret = malloc(sizeof(Str));
	if(toret == NULL) return NULL;
	
	toret->len = src->len;
	if(toret->len == 0) return NULL;
	
	toret->ptr = malloc((toret->len + 1) * sizeof(char));
	if(toret->ptr == NULL) return NULL;	
	
	strcpy(toret->ptr, src->ptr);	
	
	return toret;
}

void sj_replaceExact(Str* src, char* replacement){
	assert(strlen(replacement) <= src->len);
	
	strcpy(src->ptr, replacement);
}

Str* sj_chopBack(const Str* src, const char* toChop){
	Str* tmp = sj_copyStr(src);
	size_t newIdx = tmp->len - strlen(toChop);	
	tmp->ptr[newIdx] = '\0';
	
	Str* toret = sj_newStr(tmp->ptr);
	sj_freeStr(tmp);	
	
	return toret;
}

Str* sj_addBack(const Str* src, const char* toAdd){
	size_t len = src->len + strlen(toAdd);
	if(len == 0) return NULL;
	
	Str* toret = malloc(sizeof(Str));
	if(toret == NULL) return NULL;
	
	toret->len = len;
	toret->ptr = malloc((len + 1) * sizeof(char));
	if(toret->ptr == NULL) return NULL;
	
	strcpy(toret->ptr, src->ptr);
	strcat(toret->ptr, toAdd);
	
	return toret;
}


void sj_freeStr(Str* str){
	free(str->ptr);
	free(str);
}

bool sj_startsWith(const char* whole, const char* part){
	size_t partSize = strlen(part) * sizeof(char);
	int same = strncmp(whole, part, partSize);
	
	if(same == 0) return true;
	return false;
}
bool sj_endsWith(char* whole, const char* end){
	char* last = whole + (strlen(whole) - 1);
	if(*last == *end) return true;
	
	return false;
}

char* sj_remFrom(char* whole, const char* toRem){
	char* tmp = strtok(whole, "\n");//chop out the '\n' on whole if whole is got by fgets
	
	size_t size = strlen(tmp) - strlen(toRem);
	if(size == 0) return NULL;
	
	char* toret = malloc((size + 1) * sizeof(char));
	if(toret == NULL) return NULL;
	
	strcpy(toret, tmp + strlen(toRem));
	
	return toret;
}

