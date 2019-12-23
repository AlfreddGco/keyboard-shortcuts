#include "kbdMapping.h"
#include <string>
#include <iostream>
#include <bits/stdc++.h> 

std::map<std::string, int> X11Codes;
std::map<int, std::string> LFCString;

void LFCInit()
{
    X11Codes["."] = 60;
    X11Codes[" "] = 65;
    X11Codes["@"] = 11;
}

void LFCStringInit()
{
    LFCString[1] = "Esc";
    LFCString[59] = "F1";
    LFCString[60] = "F2";
    LFCString[61] = "F3";
    LFCString[62] = "F4";
    LFCString[63] = "F5";
    LFCString[64] = "F6";
    LFCString[65] = "F7";
    LFCString[66] = "F8";
    LFCString[67] = "F9";
    LFCString[68] = "F10";
    LFCString[87] = "F11";
    LFCString[88] = "F12";
    LFCString[99] = "PrintScrn";
    LFCString[70] = "Scroll Lock";
    LFCString[119] = "Pause";
    LFCString[41] = "`";
    LFCString[2] = "1";
    LFCString[3] = "2";
    LFCString[4] = "3";
    LFCString[5] = "4";
    LFCString[6] = "5";
    LFCString[7] = "6";
    LFCString[8] = "7";
    LFCString[9] = "8";
    LFCString[10] = "9";
    LFCString[11] = "0";
    LFCString[12] = "-";
    LFCString[13] = "=";
    LFCString[14] = "Backspace";
    LFCString[110] = "Insert";
    LFCString[102] = "Home";
    LFCString[104] = "Page Up";
    LFCString[69] = "Num Lock";
    LFCString[98] = "KP /";
    LFCString[55] = "KP *";
    LFCString[74] = "KP -";
    LFCString[15] = "Tab";
    LFCString[16] = "Q";
    LFCString[17] = "W";
    LFCString[18] = "E";
    LFCString[19] = "R";
    LFCString[20] = "T";
    LFCString[21] = "Y";
    LFCString[22] = "U";
    LFCString[23] = "I";
    LFCString[24] = "O";
    LFCString[25] = "P";
    LFCString[26] = "[";
    LFCString[27] = "]";
    LFCString[28] = "Return";
    LFCString[111] = "Delete";
    LFCString[107] = "End";
    LFCString[109] = "Page Down";
    LFCString[71] = "KP 7";
    LFCString[72] = "KP 8";
    LFCString[73] = "KP 9";
    LFCString[78] = "KP +";
    LFCString[58] = "Caps Lock";
    LFCString[30] = "A";
    LFCString[31] = "S";
    LFCString[32] = "D";
    LFCString[33] = "F";
    LFCString[34] = "G";
    LFCString[35] = "H";
    LFCString[36] = "J";
    LFCString[37] = "K";
    LFCString[38] = "L";
    LFCString[39] = ";";
    LFCString[40] = "'";
    LFCString[75] = "KP 4";
    LFCString[76] = "KP 5";
    LFCString[77] = "KP 6";
    LFCString[42] = "Shift Left";
    LFCString[86] = "International";
    LFCString[44] = "Z";
    LFCString[45] = "X";
    LFCString[46] = "C";
    LFCString[47] = "V";
    LFCString[48] = "B";
    LFCString[49] = "N";
    LFCString[50] = "M";
    LFCString[51] = ",";
    LFCString[52] = ".";
    LFCString[53] = "/";
    LFCString[54] = "Shift Right";
    LFCString[103] = "Cursor Up";
    LFCString[79] = "KP 1";
    LFCString[80] = "KP 2";
    LFCString[81] = "KP 3";
    LFCString[96] = "KP Enter";
    LFCString[29] = "Ctrl Left";
    LFCString[125] = "Logo Left (-> Option)";
    LFCString[56] = "Alt Left (-> Command)";
    LFCString[57] = "Space";
    LFCString[100] = "Alt Right (-> Command)";
    LFCString[126] = "Logo Right (-> Option)";
    LFCString[97] = "Ctrl Right";
    LFCString[105] = "Cursor Left";
    LFCString[108] = "Cursor Down";
    LFCString[106] = "Cursor Right";
    LFCString[82] = "KP 0";
    LFCString[83] = "KP .";
}

std::string LFCToString(unsigned int code)
{
    std::string result;
    std::map<int, std::string>::iterator finder_iterator;
    finder_iterator = LFCString.find(code);
    if (finder_iterator != LFCString.end())
    {
        result = finder_iterator->second;
    }
    transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

KeyInfo StringToX11(Display *disp, char *string)
{
    KeyInfo key;
    KeySym keysym;
    std::map<std::string, int>::iterator finder_iterator;

    finder_iterator = X11Codes.find(string);
    if (finder_iterator != X11Codes.end())
    {
        key.keycode = finder_iterator->second;
    }
    else
    {
        keysym = XStringToKeysym(string);
        key.keycode = XKeysymToKeycode(disp, keysym);
    }

    if (strcmp(string, "@") == 0)
    {
        key.altGr = 1;
    }

    if (int(string[0]) == -61)
    { //Accute accent
        key.keycode = 34;
    }

    if (int(string[0]) == -95)
    {                     //int(char) == á
        key.keycode = 38; //a keycode
    }
    else if (int(string[0]) == -87)
    {                     //int(char) == é
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
