all:
	g++ *.cpp -o Snakegame.exe -mwindows -Isrc/include -Lsrc/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf
	./Snakegame.exe