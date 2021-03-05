#include <Arduino.h>
#include <config.h>
#include <XpressNetMaster.h>
#include <Bounce2.h>

#ifdef MCP
  #include <Wire.h>
  #include <Adafruit_MCP23017.h>
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

Bounce * buttons = new Bounce[sizeof(MegaInp)];

#ifdef MCP
  Adafruit_MCP23017 mcp;
#endif

void notifyXNetTrnt(uint16_t Address, uint8_t data);

void setup() {
  XpressNet.setup(Loco128, 9);                        //Initialisierung XNet Serial und Send/Receive-PIN
  mcp.begin();                                        // use default address 0

  for (byte i = 0; i < sizeof(MegaInp); i++) {
    buttons[i].attach(MegaInp[i], INPUT_PULLUP);      //setup the bounce instance for the current button
    buttons[i].interval(25);                          // interval in ms
  } 

  for (byte i = 0; i < sizeof(MegaOut); i++) {
    pinMode(MegaOut[i], OUTPUT);
    digitalWrite(MegaOut[i], LOW);
  }
 
  for (byte i = 0; i < sizeof(MCPOut); i++) {
    mcp.pinMode(MCPOut[i], OUTPUT);
    mcp.digitalWrite(MCPOut[i], LOW);
  }

}

void loop() {
  XpressNet.update();	                                 //call in every loop
  
  for (byte i = 0; i < sizeof(MegaInp); i++)  {
    buttons[i].update();                               // Update the Bounce instance
    if (buttons[i].fell()) {
      XpressNet.SetTrntPos(MegaInpAdd[i]-shift, MegaInpPos[i], 1);
      notifyXNetTrnt(MegaInpAdd[i]-shift, (MegaInpPos[i] | 0x08));
      if (MegaInpAdd2[i] != 0) {
        delay(250);
        XpressNet.update();	  
        XpressNet.SetTrntPos(MegaInpAdd2[i]-shift, MegaInpPos2[i], 1);
        notifyXNetTrnt(MegaInpAdd2[i]-shift, (MegaInpPos2[i] | 0x08));
      }
    }
  }

}

void notifyXNetTrnt(uint16_t Address, uint8_t data) {
  if (bitRead(data,3) == 0x01) {  //Weiche Aktiv == HIGH?
    for (byte i = 0; i < sizeof(MegaOut); i++) {
      if (Address == MegaOutAdd[i]-shift) {
        digitalWrite(MegaOut[i], MegaOutPos[i]^(data & 0x01));
      }
    }
    for (byte i = 0; i < sizeof(MCPOut); i++) {
      if (Address == MCPOutAdd[i]-shift) {
        mcp.digitalWrite(MCPOut[i], MCPOutPos[i]^(data & 0x01));
      }
    }
    // Serial.print("XNet TA:");
    // Serial.print(Address);
    // Serial.print(", P:");
    // Serial.println(data & 0x01);
  }
}