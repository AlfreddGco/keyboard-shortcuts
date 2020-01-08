#include <stdio.h>
#include <bits/stdc++.h> //vector
#include <fcntl.h>       // open()
#include <unistd.h>      // close()
#include <stdlib.h>      // exit()
#include <ctype.h>       // isupper()
#include <linux/input.h> // EV_KEY

#include "kbdMapping.h"

#include <X11/XKBlib.h>
#include <X11/Intrinsic.h>
#include <X11/extensions/XTest.h>

int unsigned DEBUG = 0;

std::string exec(const char *cmd)
{
  std::array<char, 128> buffer;
  std::string result;
  std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
  if (!pipe)
  {
    throw std::runtime_error("popen() failed!");
  }
  while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
  {
    result += buffer.data();
  }
  return result;
}

static void getKbdPath(char *kbd_path)
{
  //Append known path
  strcat(kbd_path, "/dev/input/by-path/");

  // Read file and get keyboard name
  char kbd_name[666];
  FILE *fptr;
  if ((fptr = fopen("./config/kbd_name.txt", "r")) == NULL)
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

static void sendKey(Display *disp, KeyCode keycode, int isAltGr, int isUpper)
{
  KeyCode shiftModCode = XKeysymToKeycode(disp, XK_Shift_R);
  KeySym modsym = 0;

  //grab disp control
  XTestGrabControl(disp, True);
  
  if(isUpper != 0){
    XTestFakeKeyEvent(disp, shiftModCode, True, 0);
  }

  if(isAltGr != 0){
    XTestFakeKeyEvent(disp, 0xcb, True, 0);//mode_switch
    XTestFakeKeyEvent(disp, 0x32, True, 0);//shift_left
  }

  XTestFakeKeyEvent(disp, keycode, True, 0);
  XTestFakeKeyEvent(disp, keycode, False, 0);

  //altGr release
  XTestFakeKeyEvent(disp, 0xcb, False, 0);
  XTestFakeKeyEvent(disp, 0x32, False, 0);
  
  /* Generate shift release */
  XTestFakeKeyEvent(disp, shiftModCode, False, 0);

  //detach disp control
  XSync(disp, False);
  XTestGrabControl(disp, False);
}
//command is the keyboard shortcut send
static void sendWord(Display *disp, std::string command, size_t word_length, std::vector<std::string> commands, std::vector<std::string> writings)
{
  char toWrite[255] = "";
  char value[2];
  KeySym keysym;
  KeyCode keycode;
  KeyInfo key;
  value[1] = '\0';
  for (int i = 0; i < commands.size(); i++)
  {
    if (command == commands[i])
    {
      strcat(toWrite, writings[i].c_str());
      for (int i = 0; i < strlen(toWrite); i++)
      {
        value[0] = toWrite[i];
        key = StringToX11(disp, value);
        usleep(1000000/50);//50th of a second between each char
        sendKey(disp, key.keycode, key.altGr, isupper(value[0]));
      }
      toWrite[0] = '\0';
    }
  }
}

int main()
{
  if(DEBUG == 1){
  	printf("Initializing mapping variables\n");
  }
  //Initialize code change variables.
  LFCInit();
  LFCStringInit();

  //Obtain display name
  std::string disp_name = exec("who | grep -o ':.[[:space:]]'");
  unsigned int try_count = 0;
  while(disp_name.length() <= 1 && try_count < 5){
    usleep(1000000*60); //Sleep for 60 secs before trying again
    disp_name = exec("who | grep -o ':.[[:space:]]'");
    try_count++;
  }
  //Result has an \n and a space so we remove them
  disp_name.pop_back();
  disp_name.pop_back();

  if(DEBUG==1){
    //Add the | so we notice if there are blankspaces
    printf("Openning display %s|\n", disp_name.c_str());
  }

  Display *disp = XOpenDisplay(disp_name.c_str());

  if(disp == NULL){
    printf("Unable to open X11 display\n");
    return 1;
  }
  std::vector<std::string> commands, writings;
  getCommands(&commands, &writings);

  char kbd_path[666];
  getKbdPath(kbd_path);

  // The buffer path
  const char *dev = kbd_path;
  int fd;
  struct input_event ev;
  ssize_t n;
  std::string command = "";
  int super_key = 0;
  int ctrl_key = 0;

  // Open the buffer
  fd = open(dev, O_RDONLY);
  if (fd == -1){
    printf("Error reading keyboard buffer. Remember to run this script as root\n");
    fflush(stdout);
    return 0;
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
        command += LFCToString(ev.code);
		if(DEBUG == 1){
			printf("Actual command: %s\n", command.c_str());
		}
      }
      if (ev.code == 125 && (ev.value == 1 || ev.value == 2))
      {
        super_key = 1;
      }
      else if (ev.code == 29 && (ev.value == 1 || ev.value == 2))
      {
        ctrl_key = 1;
      }
      else if (ev.code == 125 && ev.value == 0)
      {
        super_key = 0;
      }
      else if (ev.code == 29 && ev.value == 0)
      {
        ctrl_key = 0;
      }
    }

    if (!super_key && !ctrl_key && command != "")
    {
      sendWord(disp, command, command.length(), commands, writings);
      command = "";
    }

    // Since we want a safe way to kill the loop safely (closing the stream and all)
    //if (ev.code == KEY_ESC)
    //{
    //  printf("Terminating program...\n");
    //  break;
    //}
  }

  // And don't forget to close the buffer and exit safely
  close(fd);
  fflush(stdout);
  return 0;
}
