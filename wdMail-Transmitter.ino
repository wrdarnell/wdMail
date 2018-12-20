// File: wdMail-Client.ino
// Auth: Will Darnell
// Date: 19-DEC-2018

const int  sensorPin      =  2; // Number of pin to listen to
const long debouncingTime = 15; // Milliseconds to wait for debounce

volatile int  sensorState = 0;
volatile long lastMicros;

void setup() {
  pinMode(sensorPin, INPUT);
  attachInterrupt(0, debounceInterrupt, CHANGE);
}

void loop() {
  // put your main code here, to run repeatedly:

}

void debounceInterrupt() {
  if ((long)(micros() - lastMicros) >= debouncingTime * 1000) {
    onSensorChange();
    lastMicros = micros();
  }
}

void onSensorChange() {
   
}
