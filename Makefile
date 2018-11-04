CC=g++
CPPFLAGS=-std=c++11 -O2 -Wall
SDIR=src
ODIR=obj

_DEPS = Cell.h Grid.h Updater.h Pattern.h Automaton.h
DEPS = $(patsubst %,$(SDIR)/%,$(_DEPS))

_OBJ = Cell.o Grid.o Updater.o Pattern.o Automaton.o celtex.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SDIR)/%.cc $(DEPS)
	$(CC) -c -o $@ $< $(CPPFLAGS)

celtex: $(OBJ)
	$(CC) -o $@ $^ $(CPPFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ $(SDIR)/*~ celtex
