#include <stdio.h>
#include <stdbool.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include <stdlib.h>
#include <ctype.h>
#include <linux/input.h>
 
#include <X11/Xlib.h>
#include <X11/XKBlib.h>
#include <X11/Intrinsic.h>
#include <X11/extensions/XTest.h>

static void sendKey (Display * disp, KeySym keysym, KeySym modsym, int isUpper){
 KeyCode keycode = 0, modcode = 0;
 keycode = XKeysymToKeycode (disp, keysym);

 if (keycode == 0) return;
 XTestGrabControl (disp, True);
 /* Generate modkey press */
 if (modsym != 0) {
  modcode = XKeysymToKeycode(disp, modsym);
  XTestFakeKeyEvent (disp, modcode, True, 0);
 }
 /* Generate regular key press and release */
 if(isUpper != 0){
    XTestFakeKeyEvent (disp, 50, True, 0);
 }
 XTestFakeKeyEvent (disp, keycode, True, 0);
 if(isUpper != 0){
    XTestFakeKeyEvent (disp, 50, False, 0);
 }
 XTestFakeKeyEvent (disp, keycode, False, 0); 
 
 /* Generate modkey release */
 if (modsym != 0)
  XTestFakeKeyEvent (disp, modcode, False, 0);
 
 XSync (disp, False);
 XTestGrabControl (disp, False);
}

static void sendWord(Display *disp, char* word, size_t word_length){
    char toWrite[255] = "";
    char value[2];
    KeySym keysym;
    value[1] = '\0';
    if(!strcmp(word, "m")){
        strcat(toWrite, "A01283966"); 
    }
    if(toWrite[0] != '\0'){
      for(int i=0; i<strlen(toWrite); i++){
        value[0] = toWrite[i];
        keysym = XStringToKeysym(value);
        sendKey(disp, keysym, 0, isupper(value[0]));
      }
    }
}

int main(void) {
    Display *disp = XOpenDisplay(NULL);
    // The buffer path
    const char *dev = "/dev/input/by-path/pci-0000:00:14.0-usb-0:5:1.0-event-kbd";
    int fd;
    struct input_event ev;
    ssize_t n;
    char command[255] = "";
    bool super_key = false;
    bool ctrl_key = false;

    fd = open(dev, O_RDONLY); // Open the buffer
    if (fd == -1){
        printf("Error reading keyboard buffer. Remember to run this script as root\n");
        return EXIT_FAILURE;
    }
 
    while (1) {
        n = read(fd, &ev, sizeof ev); // Read from the buffer
        //ev.value 1 pressed, ev.value 0 released, 2 hold
        if (ev.type == EV_KEY){
            if(super_key==true && ctrl_key==true && ev.value == 1){
                //printf("Key %d has been pressed\n", ev.code);
                //For some reason here I have to add 8 to the keycode
                //I guess it's because of my spanish keyboard 
                KeySym tempSym = XkbKeycodeToKeysym(disp, ev.code+8, 0, 0);
                strcat(command, XKeysymToString(tempSym));
            }
            if(ev.code == 29 && (ev.value == 1 || ev.value == 2)){
                super_key = true;
            } else if(ev.code == 125 && (ev.value == 1 || ev.value == 2)){
                ctrl_key = true;
            } else if(ev.code == 29 && ev.value == 0){
                super_key = false;
            } else if(ev.code == 125 && ev.value == 0){
                ctrl_key = false;
            }
        }


        if(!super_key && !ctrl_key && command[0] != '\0'){
            sendWord(disp, command, strlen(command));
            command[0] = '\0';
        }
     
        // Since we want a safe way to kill the loop safely (closing the stream and all)
        if (ev.code == KEY_ESC){
            break; 
        }
    }
    // And don't forget to close the buffer and exit safely
    close(fd);
    fflush(stdout);
    return EXIT_FAILURE;
}
