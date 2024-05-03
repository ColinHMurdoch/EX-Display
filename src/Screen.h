#ifndef SCREEN_H
#define SCREEN_H

#include "config.h"
//#include "Defines.h"

// Screen.h

// ALL Touch panels and wiring is DIFFERENT.  The touch screen needs to be calibrated
// See the README files for how to run the calibration routine and
// copy-paste results from TouchScreen_Calibr_native.ino into the lines below.
// These settings are for the McuFriend 2.8" shield in Portrait tft.screenRotaion(0)

//const byte XP = 7, XM = A1, YP = A2, YM = 6;
//const int TS_LEFT=899,TS_RT=122,TS_TOP=100,TS_BOT=898;
//const int TS_LEFT=122,TS_RT=899,TS_TOP=100,TS_BOT=898;

// Define some colours for the display

#define BLACK   0x0000U
#define RED     0xF800U
#define GREEN   0x07E0U
#define CYAN    0x07FFU
#define MAGENTA 0xF81FU
#define YELLOW  0xFFE0U
#define WHITE   0xFFFFU

// A structure to store the screen lines.
struct DisplayStruct
{
  bool inuse=false;
  bool changed=false;
  byte displayRow=0;
  char text[MAX_LINE_LENGTH]=" ";
} DisplayLines[MAX_SCREENS][MAX_ROWS];

// variables to indicate what needs doing to display the screen
bool ScreenChanged[MAX_SCREENS];
//bool ScreenDrawn=false
bool PrintInProgress=false;
byte NextRowToPrint=0;
byte NextScreenLine=0;




// Set this following string to the header you require.  This is displayed at the top of the screen
#define HDDR_SIZE  25


// Functions appearing in the code
namespace SCREEN {

void TFT_Startup(byte screen);
void showmsgXY(int x, int y, int sz, const char *msg);
void TFT_DrawHeader(byte screen);
void testprint(byte lines);
void StartScreenPrint(byte screen);
//void PrintNextLine();
void PrintSingleLine(byte screenNo, byte screenRow);
void PrintALine(byte screen);
void DisplayScreen(byte screen);
void SetDisplayLine(byte screenId, byte screenRow, char * text);
void displayAllRows();
}

#endif