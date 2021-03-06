#include "ctap.hpp"

Request::~Request() {

}

void Request::SerialDebug() {
    Serial.print("Channel Identifier:");
    for (int i=0; i<4; i++) {
        Serial.print(String(this->channelID[i], HEX));
    }
    Serial.println("");

    Serial.print("Command Identifier:");
    Serial.println(String(this->command, HEX));

    Serial.print("BCNTH:");
    Serial.println(String(this->BCNTH));

    Serial.print("BCNTL:");
    Serial.println(String(this->BCNTL));

    Serial.print("Command Value:");
    Serial.println(String(this->data.commandValue, HEX));

    Serial.print("Command Parameter:");
    for (size_t i = 0; i < this->dataSize; i++) {
        if (this->data.commandParameter[i] < 0x10) {
            Serial.print("0");
        }
        Serial.print(String(this->data.commandParameter[i], HEX));
    }
    Serial.println("");
}

ContinuationPacket::~ContinuationPacket() {
    
}

void ContinuationPacket::SerialDebug() {
    Serial.print("Channel Identifier:");
    for (int i=0; i<4; i++) {
        Serial.print(String(this->channelID[i], HEX));
    }
    Serial.println("");

    Serial.print("Packet Sequence:");
    Serial.println(String(this->sequence));

    Serial.print("dataSize:");
    Serial.println(String(this->dataSize));
    Serial.print("data:");
    for(int i=0; i<this->dataSize; i++) {
        if (this->data[i] < 0x10) {
            Serial.print("0");
        }
        Serial.print(String(this->data[i], HEX));
    }
    Serial.println("");
}