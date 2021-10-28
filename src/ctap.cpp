#include "ctap.hpp"

Request::~Request() {

}

void Request::requestSerialDebug() {
    Serial.print("Channel Identifier:");
    for (int i=0; i<4; i++) {
        Serial.print(String(this->channelID[i], HEX));
    }
    Serial.println("");

    Serial.print("Command Identifier:");
    Serial.println(String(this->command, HEX));

    Serial.print("BCNTH:");
    Serial.println(String(this->BCNTH, HEX));

    Serial.print("BCNTL:");
    Serial.println(String(this->BCNTL, HEX));

    Serial.print("Command Value:");
    Serial.println(String(this->data.commandValue, HEX));

    Serial.print("Command Parameter:");
    for (size_t i = 0; i < this->BCNTL-1; i++) {
        if (this->data.commandParameter[i] < 0x10) {
            Serial.print("0");
        }
        Serial.print(String(this->data.commandParameter[i], HEX));
    }
    Serial.println("");
}

ContinuationPacket::~ContinuationPacket() {
    
}