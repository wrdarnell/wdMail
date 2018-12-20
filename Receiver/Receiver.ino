// File: Receiver.ino
// Auth: Will Darnell
// Date: 19-DEC-2018

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "../../wdMail/Common/wdMail.h"

// TEST
const int ledPin = LED_BUILTIN;
// END TEST

RF24 radio(7, 8); // CE, CSN

void setup() {
  // Test Code
  pinMode(ledPin, OUTPUT); // Setup the on-board LED
  // End Test Code

  Serial.begin(9600); // Why? / is needed?
  radio.begin();
  radio.openReadingPipe(0, pipeAddress);
  radio.setPALevel(radioPowerLevel);
  radio.startListening();
}

void loop() {
  if (radio.available()) {
    char text[50] = "";
    radio.read(&text, sizeof(text));
    if (!strcmp(text, mailMessage)) {
      HelloLED();
    }
  }
}

void HelloLED() {
  int i;
  for (i=0;i<5;i++) {
    digitalWrite(ledPin, HIGH);
    delay(100);
    digitalWrite(ledPin, LOW);
    delay(100);
  }
}
