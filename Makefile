CXX = g++
CFLAGS = -std=c++11 -Wall -Wextra -g
LDFLAGS := -L/usr/local/lib/ -lpthread -pthread
INC = -Isrc -Itest/googletest/include -Itest

BIN = bin
SRC = src
OBJ = $(addprefix $(BIN)/, main.o z80.o mmu.o rom.o)

#-fsanitize=signed-integer-overflow -fsanitize-undefined-trap-on-error  -lgtest_main -lpthread 
GTEST = test/googletest/make/gtest.a

TST_OBJ = $(addprefix $(BIN)/, test_all.o z80.o rom.o mmu.o)
TST_SRC = test

all: arisaGB tests

arisaGB: $(OBJ)
	$(CXX) $(INC) $(CFLAGS) $(OBJ) -o $(BIN)/arisaGB

$(BIN)/%.o: $(SRC)/%.cpp    
	$(CXX) $(INC) $(CFLAGS) $< -c -o $@

tests: $(TST_OBJ)
	$(CXX) $(CFLAGS) $(TST_OBJ) $(GTEST) $(LDFLAGS) -o $(BIN)/tests

$(BIN)/%.o: $(TST_SRC)/%.cpp    
	$(CXX) $(INC) $(CFLAGS) $< -c -o $@

clean:
	rm -f $(BIN)/*.o
	rm -f $(BIN)/arisaGB
	rm -f $(BIN)/tests

.PHONY: clean
