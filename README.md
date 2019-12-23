# Keyboard-Shortcuts
C++ program for auto-writing with custom keyboard shortcuts in Linux systems

## Dependencies
The only dependency needed is libxtst-dev which you can install manually or using make file:
```
make install
```
## Compiling
```
make
```
## Running
```
sudo ./snippets
```
## Usage
Commands are readed through the config/commands.txt file where commands are written in like:
```
command|keyword
```
where "command" is the shortcut that will be listened while pressing super+ctrl keys and "keyword" is the word or the sentence to be written automatically

Super + ctrl will make program start listening for command and start autowriting

Program is closed just by listening "esc" key

## To do
- Configure service and config through scripts
- Support for most languages
