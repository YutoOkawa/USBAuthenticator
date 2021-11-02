#ifndef INCLUDE_ctap_h_
#define INCLUDE_ctap_h_

#include <Arduino.h>
#include "authAPI.hpp"

/** @brief MSGコマンドの識別子(0x03)  */
#define CTAPHID_MSG 0x83

/** @brief CBORコマンドの識別子(0x10)  */
#define CTAPHID_CBOR 0x90

/** @brief INITコマンドの識別子(0x06)  */
#define CTAPHID_INIT 0x86

/** @brief PINGコマンドの識別子(0x01)  */
#define CTAPHID_PING 0x81

/** @brief CANCELコマンドの識別子(0x11)  */
#define CTAPHID_CANCEL 0x91

/** @brief ERRORコマンドの識別子(0x3f)  */
#define CTAPHID_ERROR 0xbf

/** @brief KEEPALIVEコマンドの識別子(0x3b)  */
#define CTAPHID_KEEPALIVE 0xbb

struct Request {
    /**
     * @var   channelID
     * @brief Channel identifier.
     *        Length:4
     */
    uint8_t channelID[4];

    /**
     * @var   command
     * @brief Command identifier (bit 7 always set)
     *        Length:1
     */
    unsigned int command;

    /**
     * @var   BCNTH
     * @brief High part of payload length
     *        Length:1
     */
    unsigned int BCNTH;

    /**
     * @var   BCNTL
     * @brief Low part of payload length
     *        Length:2
     */
    unsigned int BCNTL;

    /**
     * @var   data
     * @brief Payload data
     *        Length:s-7 (s is equal to the fixed packet size)
     */
    Command data;

    /**
     * @brief Request data size
     */
    unsigned int dataSize = 0;

    ~Request();

    void SerialDebug();
};

struct ContinuationPacket {
    /**
     * @var   channelID
     * @brief Channel identifier.
     *        Length:4
     */
    uint8_t channelID[4];

    /**
     * @var   sequence
     * @brief Packet sequence 0x00...0x7f(bit 7 always cleared)
     *        Length:1
     */
    unsigned int sequence;

    /**
     * @var   data
     * @brief Payload data
     *        Length:s-5 (s is equal to the fixed packet size)
     */
    uint8_t *data;

    /**
     * @brief Continuation data size
     */
    unsigned int dataSize = 0;

    ~ContinuationPacket();

    void SerialDebug();
};

void requestSerialDebug(Request request);

#endif