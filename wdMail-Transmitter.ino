// File: wdMail-Client.ino
// Auth: Will Darnell
// Date: 19-DEC-2018

const int  sensorPin    =   2; // Number of pin to listen to
const long debouncingMs = 250; // Milliseconds to wait for debounce

volatile          int  sensorState = 0;
volatile unsigned long lastSensorFireMs;

void setup() {
  pinMode(sensorPin, INPUT);
  attachInterrupt(0, debounceInterrupt, CHANGE);
}

void loop() {
  // put your main code here, to run repeatedly:

}

void debounceInterrupt() {
  if ((long)(millis() - lastSensorFireMs) >= debouncingMs) {
    onSensorChange();
    lastSensorFireMs = millis();
  }
}

void onSensorChange() {
   
}
