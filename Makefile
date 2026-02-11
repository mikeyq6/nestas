CC=g++ -std=c++20
OBJS=nestas.cpp Emulator.cpp CPU.cpp Display.cpp DisplayFactory.cpp SDLDisplay.cpp SharedData.cpp PPU.cpp
LFLAGS=-lSDL2 -lSDL2_image -lSDL2_ttf
SUPPRESS=
EXENAME=nestas
DEBUGVARIABLES=-DDEBUG=1
CFLAGS= -Wall -Werror -pthread

debug:
	make clean
	${CC} -g ${CFLAGS} ${SUPPRESS} ${DEBUGVARIABLES} ${LFLAGS} -o ${EXENAME} ${OBJS}

release:
	make clean
	${CC} ${CFLAGS} ${SUPPRESS} ${LFLAGS} -o ${EXENAME} ${OBJS}

clean:
	rm -rf ${EXENAME} *.o
