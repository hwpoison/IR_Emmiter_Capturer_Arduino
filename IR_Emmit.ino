/* IR Generic Led.
.......(H)........
.....|.....|......
... |.......|.....
...|.........|....
..|...............
..OUT........GND..
*/

// Output Pin
const int IR_LED_PIN = 3;

// COM2B0 refer to pin 3
byte IR_PIN = COM2B0; 

// IR default frequency is of 38Hz for NEC
const long IR_Pulse_Frequency = 38000; 

void setup() {
  pinMode(IR_LED_PIN, OUTPUT);
  Serial.begin(9600);
  setupTimer();
}

void loop() {
  unsigned long data = 0xFD6897 // example code
  
  // Use serial for send custom commands (Hex codes)
  if (Serial.available()) {
    String inputString = Serial.readStringUntil('\n');
    inputString.trim();
    if (inputString.length() > 0) {
      data = strtoul(inputString.c_str(), NULL, 16);
      Serial.print("Sending command: ");
      Serial.println(data, HEX);
      sendNEC(data);
    }
  }
}

void sendNEC(unsigned long data) {
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

void mark(unsigned int microseconds) {
  turnOnIROut();
  delayMicroseconds(microseconds);
}

void space(unsigned int microseconds) {
  turnOffOROut();
  delayMicroseconds(microseconds);
}

void setupTimer(){
  /* This will made that the IR pin works
  in 38hz with 5v during each controled 
  pulses of mark or space */

  TCCR2A = 0; // Clean register
  TCCR2B = 0;
  
  // CTC Mode
  TCCR2A |= (1 << WGM21);

  // Toggle IR_PIN
  TCCR2A |= (1 << IR_PIN);

  // Prescaler 8
  TCCR2B |= (1 << CS21); // CS21 = prescaler 8

  // Calculate OCR2A:
  // f = f_clk / (2 * N * (OCR2A + 1))
  // 38kHz => OCR2A ≈ (f_clk / (2 * N * f)) - 1
  OCR2A = (16000000 / (2 * 8 * IR_Pulse_Frequency)) - 1;
  // Result: OCR2A ≈ 25
}

void turnOnIROut() {
  TCCR2A |= (1 << IR_PIN);
}

void turnOffOROut() {
  TCCR2A &= ~(1 << IR_PIN);
  digitalWrite(3, LOW); // force pin to low
}