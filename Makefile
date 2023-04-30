# Some variables
CC = g++
LANG_STD = -std=c++17
COMPILE_FLAGS = -Wall -Wfatal-errors
INCLUDE_PATH = -I"./libs/" -I"./libs/soloud/include/"
SRC_FILES = ./src/*.cpp \
			./src/Game/*.cpp \
			./src/Logger/*.cpp \
			./src/ECS/*.cpp \
			./src/AssetManager/*.cpp \
			./src/Utils/*.cpp \
			./src/Physics/*.cpp \
			./libs/imgui/*.cpp \
			./libs/soloud/audiosource/wav/*.cpp \
			./libs/soloud/backend/sdl2_static/*.cpp \
			./libs/soloud/core/*.cpp 
DEFINES = -DWITH_SDL2_STATIC
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -llua -lbox2d
OBJ_NAME = Cooky

# Some rules
build:
	$(CC) $(COMPILE_FLAGS) $(LANG_STD) $(INCLUDE_PATH) $(DEFINES) $(SRC_FILES) $(LINKER_FLAGS) -o $(OBJ_NAME)

run:
	./$(OBJ_NAME)

clean:
	rm $(OBJ_NAME)