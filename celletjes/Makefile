INCLUDES = -I/c/Users/wubma/Desktop/of_v0.10.1_msys2_release/libs/openFrameworks \
	-I/c/Users/wubma/Desktop/of_v0.10.1_msys2_release/libs/openFrameworks \
	-I/c/Users/wubma/Desktop/of_v0.10.1_msys2_release/libs/openFrameworks/events \
	-I/c/Users/wubma/Desktop/of_v0.10.1_msys2_release/libs/openFrameworks/types \
	-I/c/Users/wubma/Desktop/of_v0.10.1_msys2_release/libs/openFrameworks/math \
	-I/c/Users/wubma/Desktop/of_v0.10.1_msys2_release/libs/openFrameworks/utils \
	-I/c/Users/wubma/Desktop/of_v0.10.1_msys2_release/libs/openFrameworks/graphics \
	-I/c/Users/wubma/Desktop/of_v0.10.1_msys2_release/libs/openFrameworks/communication \
	-I/c/Users/wubma/Desktop/of_v0.10.1_msys2_release/libs/openFrameworks/gl \
	-I/c/Users/wubma/Desktop/of_v0.10.1_msys2_release/libs/openFrameworks/app \
	-I/c/Users/wubma/Desktop/of_v0.10.1_msys2_release/libs/openFrameworks/3d \
	-I/c/Users/wubma/Desktop/of_v0.10.1_msys2_release/libs/openFrameworks/sound \
	-I/c/Users/wubma/Desktop/of_v0.10.1_msys2_release/libs/openFrameworks/video \
	-I/c/Users/wubma/Desktop/of_v0.10.1_msys2_release/libs/tess2/include \
	-I/c/Users/wubma/Desktop/of_v0.10.1_msys2_release/libs/utf8/include \
	-I/c/Users/wubma/Desktop/of_v0.10.1_msys2_release/libs/glm/include  \
	-I/c/Users/wubma/Desktop/of_v0.10.1_msys2_release/libs/fmodex/include \
	-I/c/Users/wubma/Desktop/of_v0.10.1_msys2_release/libs/json/include \
	-I/c/Users/wubma/Desktop/of_v0.10.1_msys2_release/libs/pugixml/include \
	-I/c/Users/wubma/Desktop/of_v0.10.1_msys2_release/libs/videoInput/include \
	-I/c/msys64/mingw32/include/libs/fmodex/include \
	-I/c/msys64/mingw32/include/cairo \
	-I/c/msys64/mingw32/include/glew/include

LDFLAGS = -L/c/Users/wubma/Desktop/of_v0.10.1_msys2_release/libs/openFrameworksCompiled/lib/msys2 \
	  -L/c/Users/wubma/Desktop/of_v0.10.1_msys2_release/libs/uriparser/lib/msys2 \
	  -L/c/Users/wubma/Desktop/of_v0.10.1_msys2_release/libs/tess2/lib/msys2 \
	  -L/c/Users/wubma/Desktop/of_v0.10.1_msys2_release/libs/fmodex/lib/msys2 \
	  -L/c/msys64/mingw32/lib \

LIBS = -lopenFrameworks -lcairo -lz -lfreetype -lfontconfig -lglfw3 -lssl -lcrypto -lglew32 -lfreeimage -lboost_filesystem-mt -lcurl -luriparser -lopengl32 -lglut -ltess2 -lfmodex
CC=g++
CFLAGS= -g -Wno-conversion-null 
TARGET = main

SOURCES := $(wildcard *.cpp)
OBJECTS := $(patsubst %.cpp, %.o, $(SOURCES))

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ $(LDFLAGS) $(LIBS)  

%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $<

run: $(TARGET).exe
	./$(TARGET).exe

.PHONY: clean

clean:
	rm -rf $(TARGET).exe *.o
