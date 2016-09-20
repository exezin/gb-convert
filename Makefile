IDIR   =.
CC     =gcc -lm
CFLAGS =-lm -I$(IDIR)

ODIR   =obj
BDIR   =build

_DEPS  =
DEPS   = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ   = main.o
OBJ    = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

gb-convert: $(OBJ)
	$(CC) -o $(BDIR)/$@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ $(BDIR)/gb-convert
