.DEFAULT_GOAL := compile

install:
	@echo "Installing dependencies..."
	sudo apt install libxtst-dev -y
	ls /dev/input/by-path | grep kbd > config/kbd_name.txt

compile: snippets.c
	@echo "Compiling program..."
	gcc snippets.c -lX11 -lXtst -o snippets 

clean:
	@echo "Removing files..."
	rm snippets
