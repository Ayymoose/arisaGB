CXX = g++
CFLAGS = -std=c++11 -Wall -Wextra -Wshadow -g
LDFLAGS := -L/usr/local/lib/ -lpthread -pthread
INC = -Isrc -Itest

BIN = bin
SRC = src
OBJ = $(addprefix $(BIN)/, main.o z80.o rom.o gpu.o gb.o)

GTEST = test/googletest/make/gtest.a
GINC = -Itest/googletest/include

TST_OBJ = $(addprefix $(BIN)/, test_all.o z80.o rom.o gpu.o gb.o)
TST_SRC = test

all: arisaGB tests#  window#

arisaGB: $(OBJ)
	$(CXX) $(INC) $(CFLAGS) $(OBJ) -o $(BIN)/arisaGB

$(BIN)/%.o: $(SRC)/%.cpp    
	$(CXX) $(INC) $(GINC) $(CFLAGS) $< -c -o $@

tests: $(TST_OBJ)
	$(CXX) $(CFLAGS) $(TST_OBJ) $(GTEST) $(LDFLAGS) -o $(BIN)/tests

$(BIN)/%.o: $(TST_SRC)/%.cpp    
	$(CXX) $(INC) $(CFLAGS) $< -c -o $@

window: $(SRC)/window.cpp
	$(CXX) $(CFLAGS) $(SRC)/window.cpp -o $(BIN)/window -lGL -lGLU -lglut

clean:
	rm -f $(BIN)/*.o
	rm -f $(BIN)/arisaGB
	rm -f $(BIN)/tests
	rm -f $(BIN)/window

.PHONY: clean
