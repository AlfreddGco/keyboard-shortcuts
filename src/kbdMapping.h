#pragma once
#define _kbdMapping_H_
#include <map>
#include <string>
#include <ctype.h>

#include <X11/XKBlib.h>
#include <X11/Intrinsic.h>
#include <X11/extensions/XTest.h>


struct KeyInfo{
    int keycode = 0;
    int altGr = 0;
};

void LFCInit();
void LFCStringInit();
std::string LFCToString(unsigned int code);
KeyInfo StringToX11(Display *disp, char *string);
