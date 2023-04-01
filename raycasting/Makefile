# The maze project Makefile

#OBJS specifies which files to compile as part of the project
OBJS = raycaster_flat.cpp quickcg.cpp
TEXTURE_OBJS = raycaster_textured.cpp quickcg.cpp
TEST_OBJS = main.cpp quickcg.cpp
FLOOR_OBJS = raycaster_floor.cpp quickcg.cpp
SPRITES_OBJS = raycaster_sprites.cpp quickcg.cpp

#CC specifies which compiler we're using
CC = g++

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS = -w

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lSDL

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = testfile

#This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)

texture : $(TEXTURE_OBJS)
	$(CC) $(TEXTURE_OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)

test : $(TEST_OBJS)
	$(CC) $(TEST_OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)

floor : $(FLOOR_OBJS)
	$(CC) $(FLOOR_OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)

sprites : $(SPRITES_OBJS)
	$(CC) $(SPRITES_OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)