// File: Receiver.ino
// Auth: Will Darnell
// Date: 19-DEC-2018

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "../../wdMail/Common/wdMail.h"
#include <jled.h>

const int  statusLedPin = 5;
const int   alertLedPin = 4;
const int   radioLedPin = 6;
const int alertResetPin = 9;

volatile unsigned long lastReset;

auto  alertLed = JLed( alertLedPin);
auto  radioLed = JLed( radioLedPin);
auto statusLed = JLed(statusLedPin);

RF24 radio(7, 8); // CE, CSN

void setup() {
  // Setup pins
  pinMode(alertResetPin,   INPUT );

  // Warm up serial port
  Serial.begin(9600);
  while (!Serial) {;}
  
  // Setup Radio
  radio.begin();
  radio.openReadingPipe(0, pipeAddress);
  radio.setPALevel(radioPowerLevel);
  radio.startListening();
}

void loop() {
  healthCheck();
  watchResetButton();
  checkForRadioMessage();

    // LEDs
  alertLed.Update();
  radioLed.Update();
  statusLed.Update();
}

void watchResetButton() {
  // Watches the reset button.  If pushed, clear alert status
  if ((((long)millis() - lastReset) > 2000) && digitalRead(alertResetPin)) {
    notifySerial((char*)"Reset Button Pressed");
    alertLed.Stop();
    lastReset = millis();
  }
}

void healthCheck() {

}

void checkForRadioMessage() {
  if (radio.available()) {
    radioLed.Blink(25, 250).Repeat(1);
    char text[32] = "";
    radio.read(&text, sizeof(text));
    
    if (!strcmp(text, mailMessage)) {
      alertLed.Breathe(1500).DelayAfter(500).Forever();
    }
    
    notifySerial(text);
  }
}

void notifySerial(char* message) {
  char buf[51];
  sprintf(buf, "%s\n", message);
  Serial.write(buf);
}
