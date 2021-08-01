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

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(999);
  masterMCU.begin(Serial);
  delay(999);
}

void debug() {
  Serial.print("MASTER: ");
  Serial.print(millis());
  Serial.print("   Water: ");
  Serial.print(instructions.water);
  Serial.print(", Fan: ");
  Serial.print(instructions.fan);
  Serial.print(", LED: ");
  Serial.println(instructions.led);
}

void loop() {
  // put your main code here, to run repeatedly:
  toc = millis();
  if ((toc - tic) > DELTA) {
    masterMCU.txObj(instructions, sizeof(instructions));
    masterMCU.sendDatum(instructions), sizeof(instructions);
    debug();
    tic = toc;
  }

}