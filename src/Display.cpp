/*
 *  © 2024, Matt Bidwell, Paul Antoine, Colin Murdoch, Chris Harlow
 *  All rights reserved.
 *
 *
 *  This is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  It is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with CommandStation.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <Arduino.h>
#include <MCUFRIEND_kbv.h>
#include <Adafruit_GFX.h>

#include "AtFinder.h"
#include "config.h"
#include "Display.h"
#include "Screen.h"
#include "Defines.h"
#include "DisplayFunctions.h"


#ifdef DEBUG
  #if defined(ARDUINO_AVR_UNO)
    // for the Uno include this so we can use printf
    #include "LibPrintf.h"
  #endif
#endif


int ScreenLines;

//char blankmsg[MAX_LINE_LENGTH+1];


void setup() {
  Serial.begin(115200); // Start the serial communication
  CS_LISTEN.begin(115200);
  //SERIAL1.begin(115200); // Start Serial1 for listening to messages

  //DCCEXInbound::setup(10);

  // Initialize the display
  currentScreen = INITIAL_SCREEN;
  SCREEN::TFT_Startup(currentScreen); 

  // Tell AtFinder our maximum supported text length,
  // and how to call back when found.
  AtFinder::setup(30, updateEXDisplayRow);   // currently limit to 30 lines

  Serial.println("End of Setup");
  delay(1000);

  timestamp = millis();

}

void loop() {

  // Check if we are in the startup phase 
  // This phase inhibits screen drawing until the startup messages are
  // issued by the CS
  if (StartupPhase) {
    if ((millis() - timestamp) >= STARTUP_TIME){
            StartupPhase=false;
            screencount=millis();
    }
  }

  
  // Process any data in the serial buffer
  // each byte received form serial is passed to the parse
  if (CS_LISTEN.available()) {
    AtFinder::processInputChar(CS_LISTEN.read());
  }
  // you can display all rows by sending <@ 255 0 "">
  
  // Display the time on the startup phase 
  if (StartupPhase) 
    {
      int timelapse = millis()-timestamp;
      printf("Time ms = %d\n", timelapse);
    }
      
  // No data incoming so see if we need to display anything
  if (StartupPhase==false){
    if (ScreenChanged[currentScreen]==true) {
        printf("Time to draw a screen line\n");
        if (PrintInProgress==true) { 
            printf("Pinting a line\n");
            SCREEN::PrintALine(currentScreen);
        }
        else {
          printf("Redraw Screen\n");
          SCREEN::StartScreenPrint(currentScreen);

        }
    }
  

    //Check Page Time to see if we need to scroll
    if((millis()-screencount) > SCROLLTIME) {

      if (currentScreen >= MAX_SCREENS-1) {
        currentScreen=0;
      }
      else {
        currentScreen++;
        
      }
      screencount=millis();
      SCREEN::StartScreenPrint(currentScreen);

    }
  }
  //Serial.println("End of Loop");
  // delay(1000);

}

// const uint8_t maxBufferSize = 100;
// char serialInputBytes[maxBufferSize];
// bool newSerialData = false;

// void processSerialInput() {
//   static bool serialInProgress = false;
//   static uint8_t serialIndex = 0;
//   char startMarker = '<';
//   char endMarker = '>';
//   char serialByte;
//   while (CS_LISTEN.available() > 0 && newSerialData == false) {   // doesn't this need to listen to Serial now also?
//     serialByte = CS_LISTEN.read();    // doesn't this need to listen to Serial now also?
//     if (serialInProgress == true) {
//       if (serialByte != endMarker) {
//         serialInputBytes[serialIndex++] = serialByte;
//         if (serialIndex >= maxBufferSize) { // If we're at the buffer size, need to reduce to capture endMarker
//           serialIndex = maxBufferSize - 1;
//         }
//       } else {
//         if (serialIndex >= maxBufferSize - 1) { // If we're at the buffer size - 1, need room to add endMarker and null terminator
//           serialIndex = maxBufferSize - 2;
//         }
//         serialInputBytes[serialIndex++] = endMarker;
//         serialInputBytes[serialIndex] = '\0';
//         serialInProgress = false;
//         serialIndex = 0;
//         newSerialData = true;
//       }
//     } else if (serialByte == startMarker) {
//       serialInProgress = true;
//       serialInputBytes[serialIndex++] = startMarker;
//     }
//   }
//   if (newSerialData == true) {
//     Serial.println(F("Got serial command "));
//     for (uint8_t i = 0; i < maxBufferSize; i++) {
//       if (serialInputBytes[i] == '\0') break;
//       Serial.print((char)serialInputBytes[i]);
//     }
//     Serial.println(F(""));
//     newSerialData = false;
//     parseData(serialInputBytes);
//   } 
// }

