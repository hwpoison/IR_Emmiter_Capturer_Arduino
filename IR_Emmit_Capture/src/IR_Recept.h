/*

 * This library enables Arduino board to receive infrared
 * commands working with NEC protocol using a VS1838B sensor

......(H).......
....|..|..|.....
.. |...|...|....
..|....|....|...
..|....|....|...
.IN..GND..VCC...

IN = PWM Pin 11
VCC= 2.7V-5.5V

Datasheet = https://www.alldatasheet.es/datasheet-pdf/pdf/1132465/ETC2/VS1838B.html
NEC Specifications = https://www.sbprojects.net/knowledge/ir/nec.php

*/

#ifndef IR_RECEPT_H
#define IR_RECEPT_H

#include <Arduino.h> 

class IRRecept {
  private:
    int  inputPin;
  public:
    IRRecept();
    unsigned long readNEC();
    void setup(int inputPin);
};

#endif