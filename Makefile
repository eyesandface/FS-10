CXX = g++
CXXFLAGS = -Wall -g

SRC = copy.cpp
OBJ = copy.o
TARGET = copy

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ)

$(OBJ): $(SRC)
	$(CXX) $(CXXFLAGS) -c $(SRC)

clean:
	rm -f $(OBJ) $(TARGET)
