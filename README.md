# Keyboard-Shortcuts
C program for auto-writing with custom keyboard shortcuts in Linux systems

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
windows + ctrl will make program start listening for command and start autowriting

hardcoded commands:
- windows + ctrl + m = A01283966

## To do
- Read commands from .txt file
- Install as a service through make file
