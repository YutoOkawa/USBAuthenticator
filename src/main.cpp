#include <Arduino.h>
#include "USBAuthenticator.h"

USBAuthenticator authenticator;
HID_REPORT hidReceive;
HID_REPORT hidSend;

void setup() {
  // put your setup code here, to run once:
  SerialUSB.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  bool readResult = authenticator.read(&hidReceive);
  if (readResult) {
    SerialUSB.println("AA!!");
    SerialUSB.println(hidReceive.length);
    for (size_t i=0; i<hidReceive.length; i++) {
      SerialUSB.print(String(hidReceive.data[i], HEX));
    }
    SerialUSB.println("");
    bool sendResult = authenticator.test(hidReceive);
    if (sendResult) {
      SerialUSB.println("send!");
    } else {
      SerialUSB.println("failed...");
    }
  }
  delay(1);
}