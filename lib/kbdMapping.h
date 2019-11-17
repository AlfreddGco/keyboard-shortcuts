#pragma once
#define _kbdMapping_H_
#include <map>
#include <string>
#include <ctype.h>

#include <X11/XKBlib.h>
#include <X11/Intrinsic.h>
#include <X11/extensions/XTest.h>

extern std::map<int, std::string> LFCString;

void LFCInit();
void LFCToString(unsigned int code, char* string);
int StringToX11(Display *disp, char *string);
