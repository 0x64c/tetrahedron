ifneq ($(PLATFORM), GCW)
ifneq ($(PLATFORM), WIN)
$(info *** Defaulting to PC Linux version. Specify PLATFORM=GCW for GCW build or WIN for Windows build. ***)
SDL_CONFIG=/usr/bin/sdl2-config
TOOLCHAIN=
LIB=-L/usr/lib
INC=-I/usr/include
EXE = tetrahedron
else
$(info *** Making PC Windows build. Unspecify PLATFORM=WIN for PC build or specify GCW for GCW build. ***)
SDL_CONFIG=/usr/i686-w64-mingw32/bin/sdl2-config
TOOLCHAIN=/usr/bin/i686-w64-mingw32-
LIB=-L/usr/i686-w64-mingw32/lib
INC=-I/usr/i686-w64-mingw32/include -static
WINLIBS=-lfreetype -lbz2
CFLAGS+= -D_WIN_
EXE = tetrahedron.exe
endif
else
$(info *** Making GCW build. Unspecify PLATFORM=GCW for PC build or specify WIN for Windows build. ***)
PATH=/opt/gcw0-toolchain/usr/bin:$PATH
SDL_CONFIG=/opt/gcw0-toolchain/usr/mipsel-gcw0-linux-uclibc/sysroot/usr/bin/sdl2-config
TOOLCHAIN=/opt/gcw0-toolchain/usr/bin/mipsel-linux-
LIB=-L/opt/gcw0-toolchain/usr/mipsel-gcw0-linux-uclibc/sysroot/usr/lib
INC=-I/opt/gcw0-toolchain/usr/mipsel-gcw0-linux-uclibc/sysroot/usr/include
CFLAGS+= -D_GCW_
EXE = tetrahedron
endif

OPK_DIR=opk_build
CC=$(TOOLCHAIN)gcc
CXX=$(TOOLCHAIN)g++
STRIP=$(TOOLCHAIN)strip
OPK = $(EXE).opk
RM = /usr/bin/rm -f

#LDFLAGS += -lshake -lSDL2 -lSDL2_ttf -lSDL2_mixer $(shell $(SDL_CONFIG) --libs)
LDFLAGS += $(INC) -lSDL2 -lSDL2_ttf -lSDL2_mixer $(WINLIBS) $(shell $(SDL_CONFIG) --static-libs)
CFLAGS += -g3 $(LIB) $(shell $(SDL_CONFIG) --cflags) -Wall -Wextra

#REMOTE_USER=root
#REMOTE_IP=192.168.0.156
#REMOTE_PATH=/media/GCW_EXT/apps

SRCS=$(shell echo *.c)
OBJS=$(SRCS:%.c=%.o)

ALL : $(EXE)

.c.o:
	$(CC) $(CFLAGS) $(CXXFLAGS) -c $*.c -o $*.o 
.cpp.o:
	$(CXX) $(CFLAGS) $(CXXFLAGS) -c $*.cpp -o $*.o 

$(EXE) : $(OBJS)
	$(CXX) $(OBJS) -o $(EXE) $(LDFLAGS)
	/usr/bin/mkdir -p $(OPK_DIR)
#	$(STRIP) $(EXE)
	/usr/bin/cp $(EXE) $(OPK_DIR)/$(EXE)

opk : $(EXE)
	/usr/bin/cp ./*.wav $(OPK_DIR)
	mksquashfs $(OPK_DIR) $(EXE).opk -all-root -noappend -no-exports -no-xattrs

#upload : opk
#	scp ./$(OPK) $(REMOTE_USER)@$(REMOTE_IP):$(REMOTE_PATH)/$(OPK)

clean :
	$(RM) $(OBJS) $(EXE) $(OPK_DIR)/$(EXE) $(OPK)

.PHONY:opk clean install uninstall ALL
