# Makefile 
# Andrew Turpin
# April 2014

POBJ     = plotter.o grid.o 
COBJ     = coder.o grid.o bitfile.o
MOBJ     = motherShip.o grid.o bitfile.o 

CC    = gcc 
CFLAGS = -Wall -O2

all: plotter coder motherShip

plotter: $(POBJ) Makefile
	$(CC) $(CFLAGS) -o plotter $(POBJ) -lm

coder: $(COBJ) Makefile
	$(CC) $(CFLAGS) -o coder $(COBJ) -lm

motherShip: $(MOBJ) Makefile
	$(CC) $(CFLAGS) -o motherShip $(MOBJ) -lm


clean:
	rm -f $(POBJ) $(COBJ) $(MOBJ) plotter coder motherShip

clobber: clean
	rm -f plotter*.txt *.bit    # CARE! removes temporary compression files

test:
	@for test in 1 2 3 4 6 ; do \
	    echo "********* test $$test" ;\
	    ./plotter sonar$$test.txt > plotter$$test.txt ;\
	    ./coder sonar$$test.txt actual$$test.txt plotter$$test.txt ;\
	    ./motherShip sonar$$test.txt plotter$$test.txt actual$$test.txt.bit | cmp - actual$$test.txt ;\
	done
    
plotter.o:  Makefile grid.h
coder.o:  Makefile grid.h bitfile.h 
motherShip.o:  Makefile grid.h bitfile.h
grid.o: Makefile grid.h
bitfile.o: Makefile bitfile.h 
