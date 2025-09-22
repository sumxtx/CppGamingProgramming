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

OBJS := $(foreach file, $(SRC_FILES), $(addprefix $(OBJ_DIR), $(addsuffix .o, $(file))))
DEPS := $(foreach file, $(SRC_FILES), $(addprefix $(OBJ_DIR), $(addsuffix .d, $(file))))
INCS := $(foreach file, $(SRC_FILES), $(addprefix $(INC_DIR), $(addsuffix.hpp, $(file))))


all: $(OBJ_DIR) $(OBJS)
	$(CC) $(OBJS) $(SFML_LIB) $(LIB_FLAGS) $(LIB_FLAC) -o $(BIN_NAME)

$(OBJ_DIR):
	@mkdir $(OBJ_DIR)

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp Makefile $(INC_DIR)$(INCS)
	$(CC) $(DEP_FLAGS) $(SFML_INC) -I$(INC_DIR) -c $< -o $@
