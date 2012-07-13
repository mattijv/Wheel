CC=g++
CFLAGS=-O3 -std=c++11 -Iinclude `freetype-config --cflags` -I ../include
SOURCES=atlas.cpp font.cpp baserenderer.cpp wheel.cpp
OBJECTS=$(addprefix ../objects/, $(addsuffix .o, $(basename ${SOURCES})))
EXECUTABLE=../bin/a.out
LDFLAGS=-O3 -pthread
HEADERS=../include/*.h
LIBS=-lGL -lGLEW -lglfw

all: $(OBJECTS) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) $(LIBS) `freetype-config --libs` -o $@

.cpp.o: HEADERS
	$(CC) $(CFLAGS) $< -c -o $@

../objects/base/string.o: base/string.cpp $(HEADERS)
	$(CC) $(CFLAGS) $< -c -o $@

../objects/atlas.o: atlas.cpp  $(HEADERS)
	$(CC) $(CFLAGS) $< -c -o $@

../objects/font.o: font.cpp  $(HEADERS)
	$(CC) $(CFLAGS) $< -c -o $@

../objects/baserenderer.o: baserenderer.cpp $(HEADERS)
	$(CC) $(CFLAGS) $< -c -o $@

../objects/wheel.o: wheel.cpp  $(HEADERS)
	$(CC) $(CFLAGS) $< -c -o $@

clean:
	-rm -f ${EXECUTABLE} ${OBJECTS}

.PHONY: all clean
