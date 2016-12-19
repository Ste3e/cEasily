del /F /Q bin\app.exe

set GCC=D:/cSDL/mingw64/bin/gcc.exe
set files=main.c 

set head=-Isrc/include/SDL2 -Isrc/include/GL
set lib=-lmingw32 -lSDL2main -lSDL2 -mwindows -lopengl32 -Lsrc/lib/ 

rem %GCC% %head% -c src/glew.c -o bin/glew.o
%GCC% %head% -Wall -o bin/app  %files% bin/glew.o %lib%

bin\app.exe

