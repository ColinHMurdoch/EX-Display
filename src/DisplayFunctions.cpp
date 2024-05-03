//#include "Defines.h"
#include "DisplayFunctions.h"
#include "Screen.h"


#ifdef DEBUG
bool debug = true;
#else
bool debug = false;
#endif


// This function is called from AtFinder when a
// <@ screenId row "text"> message is discovered.

void updateEXDisplayRow(uint8_t screenId, uint8_t screenRow, char *text) {

  // special diagnostic to dump buffers on request
  if (screenId == 255) {
    SCREEN::displayAllRows();
    return;
  }

  SCREEN::SetDisplayLine(screenId, screenRow, text);
 
    Serial.print(F("\nCallback activated for screen|row|text: "));
    Serial.print(screenId);
    Serial.print(F("|"));
    Serial.print(screenRow);
    Serial.print(F("|"));
    Serial.println(text);
    
  //} 
  // else {
  //   Serial.print("\nCallback ignored for screen ");
  //   Serial.println(screenId);
  // }
}


