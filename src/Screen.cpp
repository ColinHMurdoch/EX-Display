
#include <Arduino.h>
#include <MCUFRIEND_kbv.h>
#include <Adafruit_GFX.h>

#include "config.h"
#include "Screen.h"
#include "Defines.h"

MCUFRIEND_kbv tft;

#include "Arial9pt7b.h"
//#include "FreeSans12pt7b.h"

void TFT_Startup(byte screen)
{

    uint16_t ID = tft.readID();
    Serial.print("TFT ID = 0x");
    Serial.println(ID, HEX);
    // #ifdef DEBUG
    // SERIAL.println("Calibrate for your Touch Panel");
    // #endif
    //if (ID == 0xD3D3) ID = 0x9486; // write-only shield

    tft.begin(ID);
  
    // if (ARDUINO_NUCLEO_F446RE){
    //     if(ID = 0x9488){
    //       tft.invertDisplay(1);
    //     }
    //   else {
    //     //tft.invertDisplay(0);
    //   }
    // }
  
    tft.setRotation(1);           
    tft.setTextColor(0xFFFF); 
    tft.fillScreen(BLACK);

    // create a string of blanks for the display.
    // This does not seem necessary as we draw the whole screen.
    // for (byte x=0; x<=MAX_LINE_LENGTH; x++){
    //   blankmsg[x]=' ';
    // }
    // blankmsg[MAX_LINE_LENGTH+1]='\0';

    SCREEN::StartScreenPrint(screen);

}

void showmsgXY(int x, int y, int sz, const char *msg)
{
    tft.setFont();
    tft.setFont(&Arial9pt7b);
    tft.setCursor(x, y); 
    tft.setTextSize(sz);
    tft.print(msg);
       
}

void TFT_DrawHeader(byte screen) {

    char header[HDDR_SIZE] = {""};
    sprintf(header, "DCC-EX   SCREEN %d\n", screen);
    tft.setTextColor(YELLOW);
    //printf("Header to show = %s", header);
    showmsgXY(1, 20, 1, header);
    tft.drawFastHLine(0, 25, tft.width(), WHITE);
    tft.setTextColor(WHITE);
    
}



void testprint(byte lines){

  char message[30];
  int vpos=0;

  for (byte no=1; no<(lines+1); no++)
  {
    vpos = (no * 21) + 22;

    #ifdef DEBUG
      printf(message, "Line : %d Pos %d", no, vpos);
      Serial.println(message);
    #endif

    showmsgXY(1, vpos, 1, message);
    
    }

}


  // if (ScreenDrawn){
  //   printf("Printing single line %d", screenRow);
  //   PrintSingleLine(screenNo, screenRow);
  // }


void StartScreenPrint(byte screen) {
    
    Serial.println("New Page");
    tft.fillScreen(BLACK);

    TFT_DrawHeader(screen);

    Serial.println("Drawn Header\n");
    // force the redraw of the screen
    ScreenChanged[screen]=true;
    PrintInProgress=true;
    NextRowToPrint=0;
    NextScreenLine=0;
    SCREEN::DisplayScreen(screen);  // debug output only
    

}

void PrintSingleLine(byte screenNo, byte screenRow) {
    printf("Printing lines %d\n", screenRow);
    // Find which screen row to print on
    byte Row = 0;
    for (byte x=0; x<=screenRow; x++) {
        if (DisplayLines[screenNo][x].inuse==true) {
          Row=Row+1;
        }
      
    }
    printf("Row found - %d\n", Row);
    //NextRowToPrint=Row;
    byte vpos = (Row * 21) + 44;
    //showmsgXY(1, vpos, 1, blankmsg);
    //tft.fillRect(1,(vpos-21),320, 20, BLACK);

    showmsgXY(1, vpos, 1, DisplayLines[screenNo][Row].text);
    
}

void PrintALine(byte screen) {
  
  int vpos=0;
  //printf("Printing a line %d MAX %d\n", NextRowToPrint, MAX_ROWS);
  //delay(50);
  if (DisplayLines[screen][NextRowToPrint].inuse==true) {
    printf("Print row %d screen %d\n", NextScreenLine, screen);
    vpos = (NextScreenLine * 21) + 44;
    
    //showmsgXY(1, vpos, 1, blankmsg);

    showmsgXY(1, vpos, 1, DisplayLines[screen][NextRowToPrint].text);

    printf("Printing Row %d - %s\n", NextScreenLine, DisplayLines[screen][NextRowToPrint].text);
    // increment the screen line count
    NextScreenLine++;
  }
  // increment the data line count
  NextRowToPrint++;

  if (NextRowToPrint >= MAX_ROWS) {
    //We've reached the end of this page
    PrintInProgress=false;
    ScreenChanged[screen]=false;
    
    // Any blank lines needed?
    while (NextScreenLine<MAX_ROWS){
      vpos = (NextScreenLine * 21) + 44;
      //showmsgXY(1, vpos, 1, blankmsg);
      NextScreenLine++;
    }
  NextRowToPrint = 0;
  NextScreenLine = 0;
  }
  
}


void DisplayScreen(byte screen){
  //if (screenNo == 0){
   // byte vpos = (screenRow * 21) + 44;
  //   tft.fillRect(1,vpos,320, 20, BLACK);
   //  showmsgXY(1, vpos, 1, msg);
  //}

  for (byte x=0;x<10;x++){
    printf("Line %d - Use - %d - %s\n", x, DisplayLines[screen][x].inuse, DisplayLines[screen][x].text);
  }

}

void SetDisplayLine(byte screenId, byte screenRow, char * text) {

  DisplayLines[screenId][screenRow].inuse = true;
  DisplayLines[screenId][screenRow].changed = true;
  DisplayLines[screenId][screenRow].displayRow = NULL;
  strcpy (DisplayLines[screenId][screenRow].text, text);
  // Set a flag so the screen driver knows something has changed.
  ScreenChanged[screenId]=true;

    // If this is the current screen we could call a row update line directly from here
    // but do we know which screen row to use?
    // PrintThisLine(screenId, screenRow, text)
}

void displayAllRows() {
  for (byte x=0; x<MAX_SCREENS; x++) {
    Serial.print(F("\n\nDisplay: "));
    Serial.print(x);
    Serial.println(F("|"));
    for (byte y=0; y<MAX_ROWS; y++) {
      Serial.print(DisplayLines[x][y].displayRow);
      Serial.print(F("|"));
      Serial.println(DisplayLines[x][y].text);
    }
  }
}

