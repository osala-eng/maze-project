# The maze project Makefile

#OBJS specifies which files to compile as part of the project
OBJS = maze.cpp lib/quickcg.cpp

#CC specifies which compiler we're using
CC = g++
WCC = x86_64-w64-mingw32-g++

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS = -w

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lSDL

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = testfile
WIN_FILE = maze-1.0.exe
LIN_FILE = maze-1.0

#This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)

#This complies a sample executable for windows
# win : $(OBJS)
# $(WCC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(WIN_FILE)

#This compiles a sample executable for linux
lin : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(LIN_FILE)
