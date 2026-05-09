CXX = c++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic

TARGET = metaprogramming
SRC = metaprogramming.cpp

.PHONY: all
all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

.PHONY: run
run: $(TARGET)
	@./$(TARGET)

.PHONY: clean
clean:
	@rm -f $(TARGET)

.PHONY: format
format:
	@clang-format -i *.cpp

.PHONY: edit
edit:
	@vim $(SRC)


