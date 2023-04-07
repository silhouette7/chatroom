include ../rules.mk

COMMON_DIR := ../common/src
COMMON_SRC := $(wildcard $(COMMON_DIR)/*.cpp)
COMMON_OBJ := $(patsubst %.cpp,%.o,$(COMMON_SRC))

TARGET := server

SRC_DIR := ./src
SRC := $(wildcard $(SRC_DIR)/*.cpp)
OBJ := $(patsubst %.cpp,%.o,$(SRC))
INC += -I ./include
INC += -I ../common/include

all: $(OBJ) $(COMMON_OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(COMMON_OBJ) -o $(TARGET) 

$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(INC) -c $< -o $@ $(CFLAGS)

.PHONY: clean
clean:
	rm -rf $(OBJ)