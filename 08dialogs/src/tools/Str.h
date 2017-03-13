#ifndef STR_H
#define STR_H

typedef struct Str{
	size_t len;
	char* ptr;
}Str;

Str* sj_newStr(const char* src);
Str* sj_copyStr(const Str* src);
void sj_replaceExact(Str* src, char* replacement);
Str* sj_chopBack(const Str* src, const char* toChop);
Str* sj_addBack(const Str* src, const char* toAdd);

void sj_freeStr(Str* str);

//char* functions
bool sj_startsWith(const char* whole, const char* part);
bool sj_endsWith(char* whole, const char* end); 
char* sj_remFrom(char* whole, const char* toRem);



#endif

