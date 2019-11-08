#ifndef _kbdMapping_H_
#define _kbdMapping_H_

namespace kbdMapping {

    void LFCToString(unsigned int code, char* string);
    int StringToLFC(char* string);
    void X11ToString(unsigned int code, char* string);
    int StringToX11(char* string);

}
#endif
