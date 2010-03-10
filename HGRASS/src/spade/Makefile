CC = g++
PROGRAM = SPADE
CFLAGS = -g -p
#CFLAGS = -g
#CFLAGS = -g -p -ftest-coverage -fprofile-arcs
DEFAULT_MAR = -D PRINT_PROCESS
USER_MAR = 
MAR = $(DEFAULT_MAR) $(USER_MAR)
LIBES = `pkg-config --cflags --libs gtk+-2.0` -I/usr/include/libxml2 -lxml2
ROOTHOME = ../../
SRCDIR = src
SOURCE = $(wildcard $(SRCDIR)/*.cpp) $(wildcard $(SRCDIR)/*/*.cpp)

all:$(PROGRAM)

$(PROGRAM):$(SOURCE)
	$(CC) $(CFLAGS) $(MAR) -o $(PROGRAM) $(LIBES) $^

clean:
	rm -f $(PROGRAM)* $*.o
	
