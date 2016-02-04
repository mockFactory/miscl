
hd = $(HOME)/lib
LIB = -lm 

CC = gcc
CFLAGS = -O2 
EXEC = convert2ascii

OBJS01 = convert2ascii.o nrutil.o
$(EXEC): $(OBJS01)
	$(CC) -o $@ $(OBJS01) $(LIB)

OBJS02 = combine_halocats.o nrutil.o
combine_halocats: $(OBJS02)
	$(CC) -o $@ $(OBJS02) $(LIB)


clean:
	rm -f *.o
