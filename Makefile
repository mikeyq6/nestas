CC=g++ -std=c++17
OBJS=nestas.cpp Emulator.cpp CPU.cpp Stack.cpp Display.cpp SDLDisplay.cpp SharedData.cpp
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
