CXX = c++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic

TARGET = metaprogramming
SRC = metaprogramming.cpp

all: $(TARGET)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

.PHONY = all

run: $(TARGET)
	@./$(TARGET)

.PHONY = run

clean:
	@rm -f $(TARGET)

.PHONY = clean

edit:
	@vim $(SRC)

.PHONY = edit

