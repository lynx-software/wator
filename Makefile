.PHONY: all clean

OBJS =  wator

CXXFLAGS = -g -O3 -Wpedantic -Wall -Wextra -Wmisleading-indentation -Wunused -Wuninitialized -Wshadow -Wconversion -D_GLIBCXX_ASSERTIONS -std=c++17

all: ${OBJS}

ControllerT.o: ControllerT.h MapT.h DisplayT.h
MapT.o: MapT.h CreatureT.h ControllerT.h FishT.h SharkT.h
DisplayT.o: DisplayT.h MapT.h
CreatureT.o: CreatureT.h
FishT.o: FishT.h CreatureT.h
SharkT.o: SharkT.h CreatureT.h
wator: ControllerT.o CoordinateT.o MapT.o DisplayT.o CreatureT.o FishT.o SharkT.o

clean:
	rm -f *.o ${OBJS}
	clear

clear:
	clear
	clear
	make