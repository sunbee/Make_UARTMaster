#include <Arduino.h>

#include <Wire.h>
#include <SerialTransfer.h>

SerialTransfer masterMCU;
unsigned long tic = millis();
unsigned long toc = tic;

#define DELTA 1000

struct PAYMASTER {
  /*
  water: instruction to switch pump on or off. Note the float sensor in pump's circuit will prevent overflow.
  fan: instruction to control fan speed - LO, MED, HIGH. Note PC fan requires an int between 0 and 255.
  led: instruction to control LED brightness. Note that the FastLED library requires an int between 0 and 255.
  */
  bool water;
  uint8_t fan; 
  uint8_t led;
} instructions = {
  true,
  201,
  60
};

struct PAYSLAVE {
  /*
  fan: the fan speed read off the pin no. 3 (yellow wire) of a PC fan.
  */
  uint8_t fan;
} status;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(999);
  masterMCU.begin(Serial);
  delay(999);
}

void debugTx() {
  Serial.print("MASTER TX: ");
  Serial.print(millis());
  Serial.print("   Water: ");
  Serial.print(instructions.water);
  Serial.print(", Fan: ");
  Serial.print(instructions.fan);
  Serial.print(", LED: ");
  Serial.println(instructions.led);
}

void debugRx() {
  Serial.print("MASTER RX: ");
  Serial.print(millis());
  Serial.print(", Fan: ");
  Serial.println(status.fan);
}

void loop() {
  // put your main code here, to run repeatedly:
  toc = millis();
  if ((toc - tic) > DELTA) {
    masterMCU.txObj(instructions, sizeof(instructions));
    masterMCU.sendDatum(instructions), sizeof(instructions);
    debugTx();
    tic = toc;
  }
  if (masterMCU.available()) {
    masterMCU.rxObj(status);
    debugRx();
  } else if (masterMCU.status < 0) {
    Serial.print("ERROR: ");

    if(masterMCU.status == -1)
      Serial.println(F("CRC_ERROR"));
    else if(masterMCU.status == -2)
      Serial.println(F("PAYLOAD_ERROR"));
    else if(masterMCU.status == -3)
      Serial.println(F("STOP_BYTE_ERROR"));
  }
}