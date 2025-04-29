#include "IR_Recept.h"

IRRecept::IRRecept() {}

void IRRecept::setup(int inputPin){
  this->inputPin = inputPin;
  pinMode(inputPin, INPUT);
}

unsigned long IRRecept::readNEC() {
  unsigned long result = 0x0;
  /// 1 - Receive the start signal

  // NEC = Start pulse: 9 ms HIGH + 4.5 ms LOW.
  // PIN HIGH = Withouth signal
  // PIN LOW  = Active signal

  // Waits while the ping isn't receiving anything
  while (digitalRead(inputPin) == HIGH);

  // We receive a signal, starts the count while the signal is active
  unsigned long startTime = micros();
  while (digitalRead(inputPin) == LOW);

  // Now the signal is off, calculate the time diff and get the first timestamp
  // Here we'll determinate if the protocol is NEC (9ms) or SONY (2.4ms)
  unsigned long firstTime = micros() - startTime;

  if (firstTime  < 8500) return false;

  // Starts again the count while signal is not active
  startTime = micros();
  while (digitalRead(inputPin) == HIGH);

  // 2 - Recive the second signal
  // Now that the inputPin received the second signal (that contains the data)
  // We will calculate the time diff so we get the second timestamp
  if (micros() - startTime < 4000) return false;

  // 3 - Start receving data
  // Now NEC start is confirmed
  // Start 32bit data read
  for (int i = 0; i < 32; i++) {
    while (digitalRead(inputPin) == LOW);  // Waits until HIGH (off)
    unsigned long pulseDuration = micros();
    while (digitalRead(inputPin) == HIGH); // Waits until LOW (on)
    pulseDuration = micros() - pulseDuration;

    // If is 1 or 0
    if (pulseDuration > 1000) {  // 1 is a long pulse
      result |= (1UL << (31 - i));
    }
  }
  
  return result;
}