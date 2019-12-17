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

    if(int(string[0]) == -61){ //Accute accent
        key.keycode = 34;
    }

    if(int(string[0]) == -95){//int(char) == á
        key.keycode = 38; //a keycode
    }
    else if (int(string[0]) == -87){//int(char) == é
        key.keycode = 26; //e keycode
    }
    else if (int(string[0]) == -83) //int(char) == í
    {
        key.keycode = 31; //i keycode
    }
    else if (int(string[0]) == -77) //int(char) == ó
    {
        key.keycode = 32; //o keycode
    }
    else if (int(string[0]) == -70) //int(char) == ú
    {
        key.keycode = 30; //u keycode
    }
    return key;
}
