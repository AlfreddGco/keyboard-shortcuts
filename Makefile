.DEFAULT_GOAL := compile

install:
	@echo "Installing dependencies..."
	sudo apt install libxtst-dev -y
	ls /dev/input/by-path | grep kbd > config/kbd_name.txt

compile:  kbdMapping.o snippets.cpp
	@echo "Compiling program..."
	g++ -g snippets.cpp lib/kbdMapping.o -lX11 -lXtst -o snippets

kbdMapping.o: lib/kbdMapping.cpp lib/kbdMapping.h
	g++ -c lib/kbdMapping.cpp -o lib/kbdMapping.o

