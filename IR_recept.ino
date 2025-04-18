/* 
This code intends to demostrate how the IR sensor works with a VS1838B sensor.
You can use it to clone the IR Remote codes.

::VS1838B scheme::
.......(H)........
.....|..|..|......
... |...|...|.....
...|....|....|....
...|....|....|....
..OUT..GND..VCC...

OUT = PWM Pin 11
VCC = 2.7V-5.5V

Datasheet = https://www.alldatasheet.es/datasheet-pdf/pdf/1132465/ETC2/VS1838B.html

*/
const int RECV_PIN = 11;  // IR Sensor OUT

void setup() {
  Serial.begin(9600);
  pinMode(RECV_PIN, INPUT);
}

void loop() {
  unsigned long data = 0;
  if (readIR(&data)) {
    Serial.println(data, HEX);
  }
}

bool readIR(unsigned long *result) {
  // NEC = Start pulse: 9 ms HIGH + 4.5 ms LOW.

  // PIN HIGH = Withouth signal
  // PIN LOW  = Active signal

  // Waits while the ping isn't receiving anything
  while (digitalRead(RECV_PIN) == HIGH);

  // We receive a signal, starts the count while the signal is active
  unsigned long startTime = micros();
  while (digitalRead(RECV_PIN) == LOW);

  // Now the signal is off, calculate the time diff and get the first timestamp
  // Here we'll determinate if the protocol is NEC (9ms) or SONY (2.4ms)
  unsigned long firstTime = micros() - startTime;

  if (firstTime  < 8500) return false;

  // Starts again the count while signal is not active
  startTime = micros();
  while (digitalRead(RECV_PIN) == HIGH);

  // Now that the RECV_PIN received the second signal (that contains the data)
  // We will calculate the time diff so we get the second timestamp
  if (micros() - startTime < 4000) return false;

  // Now NEC start is confirmed
  // Start 32bit data read
  for (int i = 0; i < 32; i++) {
    while (digitalRead(RECV_PIN) == LOW);  // Waits until HIGH (off)
    unsigned long pulseDuration = micros();
    while (digitalRead(RECV_PIN) == HIGH); // Waits until LOW (on)
    pulseDuration = micros() - pulseDuration;

    // If is 1 or 0
    if (pulseDuration > 1000) {  // 1 is a long pulse
      *result |= (1UL << (31 - i));
    }
  }

  return true;
}