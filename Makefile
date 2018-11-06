CC=g++
CPPFLAGS=-std=c++11 -O2 -Wall `sdl2-config --cflags`
SDIR=src
ODIR=obj
LIBS+=`sdl2-config --libs`

ifeq ($(OS),Windows_NT)
	LIBS+= -lopengl32 -lglew32 
else
	LIBS+= -framework OpenGL -lglew
endif

_DEPS = Cell.h Grid.h Updater.h Pattern.h Shader.h Texture.h Automaton.h
DEPS = $(patsubst %,$(SDIR)/%,$(_DEPS))

_OBJ = Cell.o Grid.o Updater.o Pattern.o Shader.o Automaton.o celtex.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SDIR)/%.cc $(DEPS)
	$(CC) -c -o $@ $< $(CPPFLAGS) $(LIBS)

celtex: $(OBJ)
	$(CC) -o $@ $^ $(CPPFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ $(SDIR)/*~ celtex
