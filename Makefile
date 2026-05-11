CXX = c++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic

TARGET = tests
SRC = tests.cpp 
HDR = metafunctions.hpp

.PHONY: all
all: $(TARGET)

$(TARGET): $(SRC) $(HDR)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

.PHONY: run
run: $(TARGET)
	@./$(TARGET)

.PHONY: clean
clean:
	@rm -f $(TARGET)

.PHONY: format
format:
	@clang-format -i *.cpp *.hpp

