#include <Arduino.h>
#include "USBAuthenticator.h"

void sendTest();
void parseTest();

USBAuthenticator authenticator;
HID_REPORT hidReceive;
HID_REPORT hidSend;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  // sendTest();
  // parseTest();
  bool readResult = authenticator.read(&hidReceive);
  if (readResult) {
    if (!authenticator.getContinuationFlag()) { /* 初期パケットのパース */
      authenticator.parseRequest(hidReceive);
    } else { /* 継続パケットのパース */
      authenticator.parseContinuationPacket(hidReceive);
    }
    if (!authenticator.getContinuationFlag()) {
      authenticator.operate();
    }
  }
  delay(1);
}

/* Send Test function */
void sendTest() {
  bool readResult = authenticator.read(&hidReceive);
  if (readResult) {
    Serial.println("AA!!");
    Serial.println(hidReceive.length);
    for (size_t i=0; i<hidReceive.length; i++) {
      Serial.print(String(hidReceive.data[i], HEX));
    }
    Serial.println("");
    bool sendResult = authenticator.test(hidReceive);
    if (sendResult) {
      Serial.println("send!");
    } else {
      Serial.println("failed...");
    }
  }
}

void parseTest() {
  bool readResult = authenticator.read(&hidReceive);
  if (readResult) {
    authenticator.parseRequest(hidReceive);
  }
}