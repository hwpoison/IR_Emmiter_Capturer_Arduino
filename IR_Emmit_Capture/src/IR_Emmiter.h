/*

 * This library enables an Arduino board to send infrared commands modulated at a frequency
 * of 38 kHz. It is specifically designed to work with the NEC protocol, commonly used in remote controls
 * for televisions, audio devices, and other consumer electronics.
 *
 * The IR LED control is handled using an internal timer (Timer2), ensuring accurate pulse generation
 * without blocking the main program and improving a clean and efficient 38Hz modulation at 5v between 
 * each PWM pulse. This allows integration of IR transmission with other functionalities.

--IR Generic Led--
.......(H)........
.....|.....|......
... |.......|.....
...|.........|....
..|...............
..OUT........GND..

*/
#ifndef IR_EMMITER_H
#define IR_EMMITER_H

#include <Arduino.h> 

class IREmmiter {
  private:
    int  outputPin;
    static constexpr long pulseFrequency = 38000;
    byte pinRegisterBit = COM2B0;

    void turnOnIROut();
    void turnOffIROut();
    void mark(unsigned int microseconds);
    void space(unsigned int microseconds);
  public:
    IREmmiter();
    void setup(int outputPin, const int registerBit);
    void sendNEC(unsigned long data);
    void setupTimer();
};

#endif