$(info *** Making GNU/Linux build. Run `make -f Makefile.win` for MinGW/Windows or `make -f Makefile.gcw` for GCW-Zero. ***)

SDL_CONFIG=/usr/bin/sdl2-config

LIB=-L/usr/lib
INC=-I/usr/include

EXE=tetrahedron
RM=/usr/bin/rm -rf
MKDIR=/usr/bin/mkdir -p
CP=/usr/bin/cp

CC=gcc
CXX=g++
STRIP=strip

LDFLAGS += $(INC) -lSDL2 -lSDL2_ttf -lSDL2_mixer $(shell $(SDL_CONFIG) --libs)
CFLAGS += -O2 $(LIB) $(shell $(SDL_CONFIG) --cflags) -Wall -Wextra

SRCS=$(shell echo *.c)
OBJS=$(SRCS:%.c=%.o)

ALL : $(EXE)

.c.o:
	$(CC) $(CFLAGS) $(CXXFLAGS) -c $*.c -o $*.o 
.cpp.o:
	$(CXX) $(CFLAGS) $(CXXFLAGS) -c $*.cpp -o $*.o 

$(EXE) : $(OBJS)
	$(CXX) $(OBJS) -o $(EXE) $(LDFLAGS)
	$(STRIP) $(EXE)

clean :
	$(RM) $(OBJS) $(EXE)

.PHONY:opk clean install uninstall ALL
