CXX = g++
CFLAGS = -std=c++11 -Wall -Wextra -Wshadow -g
LDFLAGS := -L/usr/local/lib/ -lpthread -pthread -lSDL2
INC = -Isrc -Itest

BIN = bin
SRC = src
OBJ = $(addprefix $(BIN)/, main.o z80.o rom.o gpu.o gb.o sdl_screen.o sdl_timer.o)

GTEST = test/googletest/make/gtest.a
GINC = -Itest/googletest/include

TST_OBJ = $(addprefix $(BIN)/, test_all.o z80.o rom.o gpu.o gb.o sdl_screen.o sdl_timer.o)
TST_SRC = test

all: arisaGB tests

arisaGB: $(OBJ)
	$(CXX) $(INC) $(CFLAGS) $(OBJ) -o $(BIN)/arisaGB $(LDFLAGS)

$(BIN)/%.o: $(SRC)/%.cpp    
	$(CXX) $(INC) $(GINC) $(CFLAGS) $< -c -o $@

tests: $(TST_OBJ)
	$(CXX) $(CFLAGS) $(TST_OBJ) $(GTEST) $(LDFLAGS) -o $(BIN)/tests

$(BIN)/%.o: $(TST_SRC)/%.cpp    
	$(CXX) $(INC) $(CFLAGS) $< -c -o $@

clean:
	rm -f $(BIN)/*.o
	rm -f $(BIN)/arisaGB
	rm -f $(BIN)/tests

.PHONY: clean
