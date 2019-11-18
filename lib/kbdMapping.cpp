#include "kbdMapping.h"

std::map<std::string, int> X11Codes;


void LFCInit()
{
    X11Codes["."] = 60;
    X11Codes[" "] = 65;
    X11Codes["@"] = 11;
}

void LFCToString(unsigned int code, char *string)
{
}

KeyInfo StringToX11(Display *disp, char *string)
{
    KeyInfo key;
    KeySym keysym;
    std::map<std::string, int>::iterator finder_iterator;

    finder_iterator = X11Codes.find(string);
    if (finder_iterator != X11Codes.end()){
        key.keycode = finder_iterator->second;
    }
    else{
        keysym = XStringToKeysym(string);
        key.keycode = XKeysymToKeycode(disp, keysym);
    }

    if(strcmp(string, "@") == 0){
        key.altGr = 1;
    }
    return key;
}
