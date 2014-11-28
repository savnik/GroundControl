CC = gcc -pthread
CFLAGS = -I /usr/local/include/sdlio
LDFLAGS = -L /usr/local/include/sdlio/ -lm
LIBS = -lphidget21 -lmenu -lncurses

PROGS = main.o display.o phidget.o

all: $(PROGS)
	
main:	main.o phidget.o display.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)
	
phidget: phidget.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)
	
display: display.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)


	
	
	doc:	$(PROGS:=.ps)

%.ps:	%.c
	a2ps -Ma4 -l -o $@ $<
#	a2ps -Ma4 -l --sides=duplex -o $@ $<