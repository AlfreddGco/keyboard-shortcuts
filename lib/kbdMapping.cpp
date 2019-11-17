#include "kbdMapping.h"

std::map<std::string, int> X11Codes;

void LFCInit()
{
    X11Codes["."] = 60;
    X11Codes[" "] = 65;
}

void LFCToString(unsigned int code, char *string)
{
}

int StringToX11(Display *disp, char *string)
{
    KeySym keysym;
    KeyCode keycode;
    std::map<std::string, int>::iterator finder_iterator;

    finder_iterator = X11Codes.find(string);
    if (finder_iterator != X11Codes.end()){
        keycode = finder_iterator->second;
        printf("KEYCODE %d \n", keycode);
    }
    else{
        keysym = XStringToKeysym(string);
        keycode = XKeysymToKeycode(disp, keysym);
    }
    return keycode;
}
