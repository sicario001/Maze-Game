CC = g++
CFLAGS = -std=c++17 -Wall -g
LDFLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
TARGET := myapp
INCDIR = src/include
SRCS := $(wildcard src/*/*.cpp) $(wildcard src/*.cpp)
OBJS := $(patsubst src/%.cpp,obj/%.o,$(SRCS))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) -I $(INCDIR)

obj/%.o: src/%.cpp
	$(CC) -c $< -o $@ $(CFLAGS) -I $(INCDIR)

clean:
	rm -f $(TARGET) $(OBJS)