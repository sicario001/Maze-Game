CC = g++
CFLAGS = -std=c++17 -Wall
LDFLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -lopenal -lalut -lenet -pthread
MEDIADIR := media
TARGET := myapp
RELEASE := release.zip
INCDIR := include
OBJDIR := obj
SRCS := $(wildcard src/*/*/*/*.cpp) $(wildcard src/*/*/*.cpp) $(wildcard src/*/*.cpp) $(wildcard src/*.cpp)
OBJS := $(patsubst src/%.cpp,obj/%.o,$(SRCS))

DEPS := $(OBJS:.o=.d)
CFLAGS += -MMD -MP

all: $(DIRS) $(TARGET)

debug: CFLAGS+= -g
debug: $(DIRS) $(TARGET)

release: $(RELEASE)
release: LDFLAGS:= -Bstatic $(LDFLAGS)

$(RELEASE):$(DIRS) $(TARGET)
	mkdir -p bin
	rm -f $(RELEASE)
	cp $(TARGET) bin/$(TARGET)
	cp -r $(MEDIADIR) bin/$(MEDIADIR)
	cd bin;	zip -r ../$(RELEASE) *
	rm -rf bin

$(DIRS):
	mkdir -p $(DIRS)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) -I $(INCDIR)

$(OBJDIR)/%.o: src/%.cpp
	@mkdir -p $(@D)
	$(CC) -c $< -o $@ $(CFLAGS) -I $(INCDIR)

clean:
	rm -rf $(TARGET) $(OBJDIR) $(DEPS) $(RELEASE)

-include $(DEPS)
