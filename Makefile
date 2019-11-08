CC = g++
CFLAGS = -g -O2 -Wall
.DEFAULT_GOAL := compile


install:
	@echo "Installing dependencies..."
	sudo apt install libxtst-dev -y
	ls /dev/input/by-path | grep kbd > config/kbd_name.txt

compile: kbdMapping.o snippets.cpp
	@echo "Compiling program..."
	g++ -g -I./lib -o snippets snippets.cpp -lX11 -lXtst

clean:
	@echo "Removing files..."
	rm snippets

kbdMapping.o: lib/kbdMapping.cpp lib/kbdMapping.h
	$(CC) $(CFLAGS) -c lib/kbdMapping.cpp
