#include <stdio.h>
#include <bits/stdc++.h> //vector
#include <fstream>       //file stream
#include <fcntl.h>       // open()
#include <unistd.h>      // close()
#include <stdlib.h>      // exit()
#include <ctype.h>       // isupper()
#include <linux/input.h> // EV_KEY

#include "lib/kbdMapping.h"

#include <X11/XKBlib.h>
#include <X11/Intrinsic.h>
#include <X11/extensions/XTest.h>

static void getKbdPath(char *kbd_path)
{
  //Append known path
  strcat(kbd_path, "/dev/input/by-path/");

  // Read file and get keyboard name
  char kbd_name[666];
  FILE *fptr;
  if ((fptr = fopen("config/kbd_name.txt", "r")) == NULL)
  {
    printf("Error! opening config file (keyboard name)");
    exit(1);
  }
  // reads text until newline (just one line)
  fscanf(fptr, "%[^\n]", kbd_name);
  fclose(fptr);

  //append kbd buffer name into path
  strcat(kbd_path, kbd_name);
}

static void getCommands(std::vector<std::string> *commands, std::vector<std::string> *writings)
{
  std::string line;
  std::ifstream infile("./config/commands.txt");
  std::string delimiter = "|";

  while (std::getline(infile, line))
  {
    std::string command = line.substr(0, line.find(delimiter));
    std::string writing = line.substr(line.find(delimiter) + 1);
    commands->push_back(command);
    writings->push_back(writing);
  }
}

static void sendKey(Display *disp, KeyCode keycode, int altGr, int isUpper)
{
  KeyCode modcode = 0;
  KeySym modsym = 0;

  XTestGrabControl(disp, True);
  
  if(isUpper != 0){
    modsym = XK_Shift_R;
  }
  /* Generate modkey press */
  if (modsym != 0)
  {
    modcode = XKeysymToKeycode(disp, modsym);
    XTestFakeKeyEvent(disp, modcode, True, 0);
  }
  if(altGr != 0){
    XTestFakeKeyEvent(disp, 0xcb, True, 0);//mode_switch
    XTestFakeKeyEvent(disp, 0x32, True, 0);//shift_left
  }

  XTestFakeKeyEvent(disp, keycode, True, 0);
  XTestFakeKeyEvent(disp, keycode, False, 0);

  if (altGr != 0){
    XTestFakeKeyEvent(disp, 0xcb, False, 0);
    XTestFakeKeyEvent(disp, 0x32, False, 0);
  }
  /* Generate modkey release */
  if (modsym != 0){
    XTestFakeKeyEvent(disp, modcode, False, 0);
  }

  XSync(disp, False);
  XTestGrabControl(disp, False);
}

static void sendWord(Display *disp, char *command, size_t word_length, std::vector<std::string> commands, std::vector<std::string> writings)
{
  char toWrite[255] = "";
  char value[2];
  KeySym keysym;
  KeyCode keycode;
  KeyInfo key;
  value[1] = '\0';
  for (int i = 0; i < commands.size(); i++)
  {
    if (!strcmp(command, commands[i].c_str()))
    {
      strcat(toWrite, writings[i].c_str());
      for (int i = 0; i < strlen(toWrite); i++)
      {
        value[0] = toWrite[i];
        key = StringToX11(disp, value);
        sendKey(disp, key.keycode, key.altGr, isupper(value[0]));
      }
      toWrite[0] = '\0';
    }
  }
}

int main()
{
  LFCInit();
  Display *disp = XOpenDisplay(NULL);
  std::vector<std::string> commands, writings;
  getCommands(&commands, &writings);

  char kbd_path[666];
  getKbdPath(kbd_path);

  // The buffer path
  const char *dev = kbd_path;
  int fd;
  struct input_event ev;
  ssize_t n;
  char command[255] = "";
  int super_key = 0;
  int ctrl_key = 0;

  // Open the buffer
  fd = open(dev, O_RDONLY);
  if (fd == -1)
  {
    printf("Error reading keyboard buffer. Remember to run this script as root\n");
    return 1;
  }

  while (1)
  {
    n = read(fd, &ev, sizeof ev); // Read from the buffer
    //ev.value 1 pressed, ev.value 0 released, 2 hold
    if (ev.type == EV_KEY)
    {
      if (super_key == 1 && ctrl_key == 1 && ev.value == 1)
      {
        //printf("Key %d has been pressed\n", ev.code);
        //This codes correspond to linux framebuffer console (lfc)
        //keycodes for x11 server are different
        //For most values if we add 8 to the lfc the result is
        //the equivalent keycode in the x11 server
        KeySym tempSym = XkbKeycodeToKeysym(disp, ev.code + 8, 0, 0);
        strcat(command, XKeysymToString(tempSym));
      }
      if (ev.code == 29 && (ev.value == 1 || ev.value == 2))
      {
        super_key = 1;
      }
      else if (ev.code == 125 && (ev.value == 1 || ev.value == 2))
      {
        ctrl_key = 1;
      }
      else if (ev.code == 29 && ev.value == 0)
      {
        super_key = 0;
      }
      else if (ev.code == 125 && ev.value == 0)
      {
        ctrl_key = 0;
      }
    }

    if (!super_key && !ctrl_key && command[0] != '\0')
    {
      sendWord(disp, command, strlen(command), commands, writings);
      command[0] = '\0';
    }

    // Since we want a safe way to kill the loop safely (closing the stream and all)
    if (ev.code == KEY_ESC)
    {
      break;
    }
  }

  // And don't forget to close the buffer and exit safely
  close(fd);
  fflush(stdout);
  return 0;
}
