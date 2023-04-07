include ../rules.mk

SRC := $(wildcard ./src/*.cpp)
OBJ := $(patsubst %.cpp,%.o,$(SRC))
INC += -I ./include

all: $(SRC)
	$(CC) $(INC) -c $(SRC) -o $(OBJ) $(CFLAGS) 

.PHONY: clean
clean:
	rm -rf $(OBJ)