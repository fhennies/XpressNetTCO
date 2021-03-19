// A simple TCO for XpressNet / XBus 
// based on the XpressNetMaster library (Copyright (c) 08/2016 - 2020 Philipp Gahtow)
// The code in this main program is published under the GPL
// All definitions should be in config.h

#include <Arduino.h>              // for platformio in Visual Studio Code
#include <XpressNetMaster.h>      // XPressNet lib
#include <Bounce2.h>              // Debounce Atmega inputs
#include <config.h>               // All input / outputs definitioned in config.h
#ifdef MCP                        // Choice not fully implemented  
  #include <Wire.h>               // I2C library
  #include <Adafruit_MCP23017.h>  // MCP23017 library
#endif

XpressNetMasterClass XpressNet;   

const byte shift = SHIFTADDR;

const byte MegaInp[] = INATMEL;
const uint16_t MegaInpAdd[] = INATMELADD;
const byte MegaInpPos[] = INATMELPOS;
const int MegaInpAdd2[] = INATMELADD2;
const byte MegaInpPos2[] = INATMELPOS2;

const byte MegaOut[] = OUTATMEL;
const uint16_t MegaOutAdd[] = OUTATMELADD;
const byte MegaOutPos[] = OUTATMELPOS;

const byte MCPOut[] = OUTMCP;
const uint16_t MCPOutAdd[] = OUTMCPADD;
const byte MCPOutPos[] = OUTMCPPOS;

Bounce * buttons = new Bounce[sizeof(MegaInp)]; // one button obejct for each defined Atmega input

#ifdef MCP
  Adafruit_MCP23017 mcp;
#endif

void notifyXNetTrnt(uint16_t Address, uint8_t data);  //function defined at bottom, made known here

void setup() {
  XpressNet.setup(Loco128, 9);                        // Initialisierung XNet Serial und Send/Receive-PIN
  mcp.begin();                                        // Use default address 0

  for (byte i = 0; i < sizeof(MegaInp); i++) {        // Loop through all defined inputs on Atmega
    buttons[i].attach(MegaInp[i], INPUT_PULLUP);      // Setup the bounce instance for the current button
    buttons[i].interval(25);                          // interval in ms
  } 

  for (byte i = 0; i < sizeof(MegaOut); i++) {        // Loop through all defined outputs on Atmega
    pinMode(MegaOut[i], OUTPUT);
    digitalWrite(MegaOut[i], LOW);
  }
 
  for (byte i = 0; i < sizeof(MCPOut); i++) {         // Loop through all defined outputs on MCP23017
    mcp.pinMode(MCPOut[i], OUTPUT);                   // use MCP specific pin functions
    mcp.digitalWrite(MCPOut[i], LOW);
  }

}

void loop() {
  // XPressNet communication routine, call in every loop
  // Calls notifyXNetTrnt if there is any turnout notifocation send by master
  XpressNet.update();	                                 

// Loop through all Atmega input pins
  for (byte i = 0; i < sizeof(MegaInp); i++)  {
    buttons[i].update();                               // Update the Bounce instance
    if (buttons[i].fell()) {                           // Input changed to LOW (connected to ground)?
      XpressNet.SetTrntPos(MegaInpAdd[i]-shift, MegaInpPos[i], 1);  // Send turnout command
      notifyXNetTrnt(MegaInpAdd[i]-shift, (MegaInpPos[i] | 0x08));  // Call notifyXNetTrnt to update LED
      if (MegaInpAdd2[i] != 0) {                        // If second address defiend for input, repeat above
        XpressNet.update();	                            // Make sure first command is processed
        delay(250);                                     // Give XPressNet some time (not sure if necessary)
        XpressNet.SetTrntPos(MegaInpAdd2[i]-shift, MegaInpPos2[i], 1);
        notifyXNetTrnt(MegaInpAdd2[i]-shift, (MegaInpPos2[i] | 0x08));
      }
    }
  }
}                                                       // End of loop, outputs are handled in notifyXNetTrnt

void notifyXNetTrnt(uint16_t Address, uint8_t data) {   // Called by XpressNet.update in case 
  if (bitRead(data,3) == 0x01) {                        // Weiche Aktiv == HIGH?
    for (byte i = 0; i < sizeof(MegaOut); i++) {        // Loop through all Atmega outputs
      if (Address == MegaOutAdd[i]-shift) {             // Output right address?
        digitalWrite(MegaOut[i], MegaOutPos[i]^(data & 0x01));  // LED on/off dependend on pos 
      }
    }
    for (byte i = 0; i < sizeof(MCPOut); i++) {         // Loop through all MCP outputs
      if (Address == MCPOutAdd[i]-shift) {
        mcp.digitalWrite(MCPOut[i], MCPOutPos[i]^(data & 0x01));
      }
    }
  }
}