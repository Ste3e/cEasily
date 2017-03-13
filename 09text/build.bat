del /F /Q bin\app.exe

set GCC=D:/cSDL/mingw64/bin/gcc.exe
set files=main.c base.c hud.c
set scene=scene/Scene.c
set tools=src/tools/List.c src/tools/Str.c
set hud=hud/Pane.c hud/Cursor.c hud/GuiEvent.c hud/Frame.c hud/Dialog.c hud/Scroll.c
set wids=hud/Text.c hud/TextArea.c hud/Button.c

set head=-Isrc/include/SDL2 -Isrc/include/GL 
set lib=-lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -mwindows -lopengl32 -Lsrc/lib/ 

rem %GCC% %head% -c src/glew.c -o bin/glew.o
%GCC% %head% -Wall -o bin/app  %files% %tools% %scene% %hud% %wids% bin/glew.o %lib%

bin\app.exe

