ifneq ($(PLATFORM), GCW)
ifneq ($(PLATFORM), WIN)
$(info *** Defaulting to PC Linux version. Specify PLATFORM=GCW for GCW build or WIN for Windows build. ***)
SDL_CONFIG=/usr/bin/sdl2-config
TOOLCHAIN=
LIB=-L/usr/lib
INC=-I/usr/include
EXE=tetrahedron
RM=/usr/bin/rm -rf
MKDIR=/usr/bin/mkdir -p
CP=/usr/bin/cp
else
$(info *** Making PC Windows build. Unspecify PLATFORM=WIN for PC build or specify GCW for GCW build. ***)
SDL_CONFIG=/mingw/bin/sdl2-config
TOOLCHAIN=/mingw/bin/mingw32-
LIB=-L/mingw/lib
INC=-I/mingw/include -static
WINLIBS=-lfreetype -lz -lpng -lbz2 -lz
CFLAGS+=-D_WIN_
EXE=tetrahedron.exe
RM=rm -rf
MKDIR=mkdir -p
CP=cp
endif
else
$(info *** Making GCW build. Unspecify PLATFORM=GCW for PC build or specify WIN for Windows build. ***)
PATH=/opt/gcw0-toolchain/usr/bin:$PATH
SDL_CONFIG=/opt/gcw0-toolchain/usr/mipsel-gcw0-linux-uclibc/sysroot/usr/bin/sdl2-config
TOOLCHAIN=/opt/gcw0-toolchain/usr/bin/mipsel-linux-
LIB=-L/opt/gcw0-toolchain/usr/mipsel-gcw0-linux-uclibc/sysroot/usr/lib
INC=-I/opt/gcw0-toolchain/usr/mipsel-gcw0-linux-uclibc/sysroot/usr/include
CFLAGS+=-D_GCW_
EXE=tetrahedron
RM=/usr/bin/rm -rf
MKDIR=/usr/bin/mkdir -p
CP=/usr/bin/cp
endif

OPK_DIR=opk_build
CC=$(TOOLCHAIN)gcc
CXX=$(TOOLCHAIN)g++
STRIP=$(TOOLCHAIN)strip
OPK =$(EXE).opk



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
	$(MKDIR) $(OPK_DIR)
#	$(STRIP) $(EXE)
	$(CP) $(EXE) $(OPK_DIR)/$(EXE)

opk : $(EXE)
	$(CP) ./*.wav $(OPK_DIR)
	mksquashfs $(OPK_DIR) $(EXE).opk -all-root -noappend -no-exports -no-xattrs

#upload : opk
#	scp ./$(OPK) $(REMOTE_USER)@$(REMOTE_IP):$(REMOTE_PATH)/$(OPK)

clean :
	$(RM) $(OBJS) $(EXE) $(OPK_DIR)/$(EXE) $(OPK)

.PHONY:opk clean install uninstall ALL
