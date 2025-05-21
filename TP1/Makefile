CC = g++
CFLAGS = -Wall -std=c++11
LDFLAGS = -pg -lm

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
EXECUTABLE = $(BIN_DIR)/main.out

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ_FILES)
	$(CC) $(LDFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

mem: $(EXECUTABLE)
	valgrind --tool=memcheck --leak-check=yes --track-origins=yes --show-leak-kinds=all --verbose -s ./$(EXECUTABLE)

run: $(EXECUTABLE)
	./$(EXECUTABLE)

clean:
	rm -f $(OBJ_DIR)/*.o $(EXECUTABLE)
