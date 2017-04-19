IDIR   =.
CC     =gcc -lm
CFLAGS =-lm -I$(IDIR)

ODIR   =obj
BDIR   =build

_DEPS  =
DEPS   = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ   = main.o
OBJ    = $(patsubst %,$(ODIR)/%,$(_OBJ))

all: files gb-convert

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

gb-convert: $(OBJ)
	$(CC) -o $(BDIR)/$@ $^ $(CFLAGS) $(LIBS)

files:
	mkdir -p obj
	mkdir -p build

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ $(BDIR)/gb-convert
