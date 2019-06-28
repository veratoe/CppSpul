INCLUDES = -I/c/msys32/SFML-2.5.1/include

LIBS = -L/c/msys32/SFML-2.5.1/lib -L/c/msys32/mingw32/i686-w64-mingw32/lib

CC = i686-w64-mingw32-g++

CFLAGS = -DSFML_STATIC

#LDFLAGS = -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -static-libgcc -static-libstdc++ -lopengl32 -lwinmm -lgdi32 -static -lpthread #-Wl,-subsystem,windows 
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system # -lopengl32 -lwinmm -lgdi32 

TARGET = main

SOURCES := $(wildcard *.cpp)
OBJECTS := $(patsubst %.cpp, %.o, $(SOURCES))

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ $(LDFLAGS) $(LIBS)

%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $<

clean: 
	rm -rf $(TARGET).exe *.o


