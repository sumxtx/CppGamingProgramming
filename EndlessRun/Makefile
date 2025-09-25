CC = g++
CFLAGS = 
LIB_FLAGS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
BIN_NAME = EndlessRun-app
DEP_FLAGS = -MMD

INC_DIR  = ./include/
SRC_DIR  = ./src/
OBJ_DIR  = ./obj/
SFML_LIB = -L../../SFML-2.6.0/lib
SFML_INC = -I../../SFML-2.6.0/include
LIB_FLAC = ../../SFML-2.6.0/lib/libFLAC.so.12

-include ./src/source_list
-include $(DEPS)

SRCS := $(foreach file, $(SRC_FILES), $(addprefix $(OBJ_DIR), $(addsuffix .cpp, $(file))))
OBJS := $(SRCS:.cpp=.o)
DEPS := $(foreach file, $(SRC_FILES), $(addprefix $(OBJ_DIR), $(addsuffix .d, $(file))))
INCS := $(foreach file, $(SRC_FILES), $(addprefix $(INC_DIR), $(addsuffix.hpp, $(file))))


all: $(OBJS)
	$(CC) $(SFML_LIB) $(LIB_FLAGS) $(LIB_FLAC) -o $(BIN_NAME) $^

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp Makefile $(INC_DIR)%.hpp
	$(CC) $(DEP_FLAGS) $(SFML_INC) -I$(INC_DIR) -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)
