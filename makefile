CC = g++
CFLAGS = -std=c++17 -Wall -g
LDFLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -lopenal -lalut -lenet -pthread
TARGET := myapp
INCDIR := include
OBJDIR := obj
SRCS := $(wildcard src/*/*/*/*.cpp) $(wildcard src/*/*/*.cpp) $(wildcard src/*/*.cpp) $(wildcard src/*.cpp)
OBJS := $(patsubst src/%.cpp,obj/%.o,$(SRCS))

DEPS := $(OBJS:.o=.d)
CFLAGS += -MMD -MP

all: $(DIRS) $(TARGET)

$(DIRS):
	mkdir -p $(DIRS)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) -I $(INCDIR)

$(OBJDIR)/%.o: src/%.cpp
	@mkdir -p $(@D)
	$(CC) -c $< -o $@ $(CFLAGS) -I $(INCDIR)

clean:
	rm -rf $(TARGET) $(OBJDIR) $(DEPS)

-include $(DEPS)