.DEFAULT_GOAL := compile

install:
	@echo "Installing dependencies..."
	sudo apt install libxtst-dev -y

compile: snippets.c
	@echo "Compiling program..."
	gcc snippets.c -lX11 -lXtst -o snippets 

clean:
	@echo "Removing files..."
	rm snippets
