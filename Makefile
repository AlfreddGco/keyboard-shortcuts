CC=g++
CFLAGS = 
LDFLAGS = -lX11 -lXtst
src = $(wildcard src/*.cpp)
obj = $(src:.cpp=.o)
deps = $(obj:.o=.d) #one dependency file for each source
PREFIX = /usr/include/keyboard-shortcuts
.DEFAULT_GOAL := compile

install:
	@echo "Installing dependencies..."
	sudo apt install libxtst-dev -y
	ls /dev/input/by-path | grep kbd > config/kbd_name.txt

.PHONY: install_service
# $< is first prerequisite, in this case: snippets
install_service: snippets
	mkdir -p $(DESTDIR)$(PREFIX)
	cp $< $(DESTDIR)$(PREFIX)/
	cp -R config/ $(DESTDIR)$(PREFIX)/
	rm $(DESTDIR)$(PREFIX)/config/kbd-shortcuts.service
	xhost +
	cp config/kbd-shortcuts.service /etc/systemd/system/
	cp scripts/kbd-shortcuts /usr/bin/kbd-shortcuts
	systemctl start kbd-shortcuts.service

# @ variable containing target of rule
# ^ variable containing all deps of the rule
compile: $(obj)
	@echo "Compiling project..."
	$(CC) -o snippets $^ $(CFLAGS) $(LDFLAGS)

-include $(deps) #include all dep files in the makefile

# rule to generate a dep file from object file
%.d: %.cpp
	@$(CPP) $(CFLAGS) $< -MM -MT $(@:.d=.o) >$@


.PHONY: clean
clean:
	rm -f $(obj) snippets

.PHONY: cleandeps
cleandeps:
	rm -f $(deps)


# This make file was made with the great help of
# http://nuclear.mutantstargoat.com/articles/make/

