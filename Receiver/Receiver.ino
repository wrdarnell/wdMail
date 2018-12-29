// File: Receiver.ino
// Auth: Will Darnell
// Date: 19-DEC-2018

#include <SPI.h>
#include <RF24.h>
#include "../../wdMail/Common/wdMail.h"
#include <jled.h>

const int  statusLedPin = 4;
const int   alertLedPin = 5;
const int   radioLedPin = 6;
const int alertResetPin = 9;

volatile unsigned long lastReset;
volatile           int healthStatus; 

auto  alertLed = JLed( alertLedPin);
auto  radioLed = JLed( radioLedPin);
auto statusLed = JLed(statusLedPin);

RF24 radio(7, 8); // CE, CSN

void setup() {
  // Init
  healthStatus = 0;

  // Setup pins
  pinMode(alertResetPin,   INPUT );

  // Warm up serial port
  Serial.begin(9600);
  while (!Serial) {;}
  
  // Setup Radio
  radio.begin();
  radio.openReadingPipe(0, pipeAddress);
  radio.setPALevel(radioPowerLevel);
  radio.setDataRate(radioDataRate);
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
  if (Serial) {
    updateStatusLed(1);
  } else {
    updateStatusLed(0);
  }
}

void updateStatusLed(int isHealthy) {
  if (isHealthy && !healthStatus) {
    statusLed.Blink(5, 5000).Forever();
    healthStatus = 1;
  } else if (!isHealthy && healthStatus) {
    statusLed.Stop();
    healthStatus = 0;
  }
}

void checkForRadioMessage() {
  if (radio.available()) {
    radioLed.Blink(25, 250).Repeat(1);
    char text[32] = "";
    radio.read(&text, sizeof(text));
    
    if (!strcmp(text, mailMessage)) {
      alertLed.Breathe(2500).DelayAfter(2000).Forever();
    }
    
    notifySerial(text);
  }
}

void notifySerial(char* message) {
  char buf[51];
  sprintf(buf, "%s\n", message);
  Serial.write(buf);
}
