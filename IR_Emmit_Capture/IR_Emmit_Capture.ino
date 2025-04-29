/*
  * Example code using the Emmiter and the Recept
  * 'read'  --> Capture a infrared code
  * 'send'  --> Send the current stored code
  * '0x.--' --> To input manually a code
*/

#include "src/IR_Emmiter.h"
#include "src/IR_Recept.h"

constexpr int IR_RECV_PIN = 11;
constexpr int IR_OUTPUT_PIN = 3;

IREmmiter emmiter;
IRRecept recept;

void setup() {
  Serial.begin(9600);
  recept.setup(IR_RECV_PIN);
  emmiter.setup(IR_OUTPUT_PIN, COM2B0);
}

unsigned long currentCode = 0xFD6897; // example code

void loop() {
  if (Serial.available()) {
    String inputString = Serial.readStringUntil('\n');
    inputString.trim();
    if (inputString.length() == 0) return;

    // Waits for a code and store it in data
    if (inputString == "read") {
      Serial.println("[<] Waiting IR signal...");
      if ((currentCode = recept.readNEC())) {
        Serial.print("Received code: ");
        Serial.println(currentCode, HEX);
      }
      return;
    }

    // To send the current stored code
    if (inputString == "send") {
      Serial.print("[>] Emitting: ");
      Serial.println(currentCode, HEX);
      emmiter.sendNEC(currentCode);
      return;
    }

    // TO input manually a code
    if (inputString.startsWith("0x")) {
      inputString = inputString.substring(2);
      inputString.toUpperCase();

      char buffer[9];
      inputString.toCharArray(buffer, sizeof(buffer));

      char* endPtr;
      unsigned long inputCode = strtoul(buffer, &endPtr, 16);

      if (*endPtr == '\0') {
        currentCode = inputCode;
        Serial.print("[>] Emitting: ");
        Serial.println(currentCode, HEX);
        emmiter.sendNEC(currentCode);
      } else {
        Serial.println("[x] Invalid hex format!");
      }
      return;
    }

    Serial.println("[x] Unknown command!");
  }
}

