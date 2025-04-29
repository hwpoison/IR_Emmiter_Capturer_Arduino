#include "IR_Emmiter.h"

IREmmiter::IREmmiter() {}

// Setup output pin
void IREmmiter::setup(int outputPin, const int registerBit) {
  this->outputPin = outputPin;
  this->pinRegisterBit = registerBit;
  pinMode(outputPin, OUTPUT);
  setupTimer();
}

void IREmmiter::setupTimer() {
  /* Configure the timer so the pin can work
  in 38hz with 5v during each controlled pulse*/

  // Clean register
  TCCR2A = 0; 
  TCCR2B = 0;
  
  // Set CTC Mode
  TCCR2A |= (1 << WGM21);

  // Toggle Pin register
  TCCR2A |= (1 << pinRegisterBit);

  // Prescaler 8
  TCCR2B |= (1 << CS21); // CS21 = prescaler 8

  // Calculate OCR2A:
  // f = f_clk / (2 * N * (OCR2A + 1))
  // 38kHz => OCR2A ≈ (f_clk / (2 * N * f)) - 1
  OCR2A = (16000000 / (2 * 8 * pulseFrequency)) - 1;
  // Result: OCR2A ≈ 25
}

void IREmmiter::sendNEC(unsigned long data) {
  // According NEC protocol

  // 1 - Send the start signal
  mark(9000);  // 9ms ON
  // 2 - Send a silence
  space(4500); // 4.5ms OFF

  // 3 - Start the command sending
  for (int i = 0; i < 32; i++) {
    if (data & 0x80000000) {
      // Send bit 1
      mark(562);   // 562us ON
      space(1687); // 1.687ms OFF
    } else {
      // Send bit 0
      mark(562);   // 562us ON
      space(562);  // 562us OFF
    }
    data <<= 1;
  }

  // 4 - Send the end signal
  mark(562);   // 562us ON
  space(0);    // Remanente OFF
}

void IREmmiter::turnOnIROut() {
  TCCR2A |= (1 << pinRegisterBit);
}

void IREmmiter::turnOffIROut() {
  TCCR2A &= ~(1 << pinRegisterBit);
  digitalWrite(3, LOW); // force pin to low
}

void IREmmiter::mark(unsigned int microseconds) {
  turnOnIROut();
  delayMicroseconds(microseconds);
}

void IREmmiter::space(unsigned int microseconds) {
  turnOffIROut();
  delayMicroseconds(microseconds);
}


