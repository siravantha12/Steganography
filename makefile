CC = g++
CFLAGS = -Wall

all: Crypto 

Crypto: Crypto.cpp support.h support.o
	$(CC) $(CFLAGS) -o $@ Crypto.cpp support.cpp

.cpp.o:
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f Crypto support.o
	rm -f *_wmsg*.ppm
