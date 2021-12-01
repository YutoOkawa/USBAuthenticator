#ifndef INCLUDED_message_h_
#define INCLUDED_message_h_

#include <stdint.h>
// #include <M5StickC.h>
#include <Arduino.h>

/**
 * @struct Command
 * @brief Message Encodingに従ったコマンド形式
 */
struct Command {
    unsigned int commandValue = 0x00;
    uint8_t *commandParameter;
    ~Command();
};

/**
 * @struct Response
 * @brief Message Encodingに従ったレスポンス形式
 */
struct Response {
    unsigned int status = 0x00;
    const uint8_t *responseData;
    unsigned int length = 0;
    ~Response();
    void ResponseSerialDebug();
};

/**
 * @struct StatusCodeParam
 * @brief レスポンスのエラーコード
 */
struct StatusCodeParam {
    static const int CTAP1_ERR_SUCCESS;
    static const int CTAP1_ERR_INVALID_COMMAND;
    static const int CTAP1_ERR_INVALID_PARAMETER;
    static const int CTAP1_ERR_INVALID_LENGTH;
    static const int CTAP1_ERR_INVALID_SEQ;
    static const int CTAP1_ERR_TIMEOUT;
    static const int CTAP1_ERR_CHANNEL_BUSY;
    static const int CTAP1_ERR_LOCK_REQUIRED;
    static const int CTAP1_ERR_INVALID_CHANNEL;
    static const int CTAP2_ERR_CBOR_UNEXPECTED_TYPE;
    static const int CTAP2_ERR_INVALID_CBOR;
    static const int CTAP2_ERR_MISSING_PARAMETER;
    static const int CTAP2_ERR_LIMIT_EXCEEDED;
    static const int CTAP2_ERR_UNSUPPORTED_EXTENSION;
    static const int CTAP2_ERR_CREDENTIAL_EXCLUDED;
    static const int CTAP2_ERR_PROCESSING;
    static const int CTAP2_ERR_INVALID_CREDENTIAL;
    static const int CTAP2_ERR_USER_ACTION_PENDING;
    static const int CTAP2_ERR_OPERATION_PENDING;
    static const int CTAP2_ERR_NO_OPERATIONS;
    static const int CTAP2_ERR_UNSUPPORTED_ALGORITHM;
    static const int CTAP2_ERR_OPERATION_DENIED;
    static const int CTAP2_ERR_KEY_STORE_FULL;
    static const int CTAP2_ERR_NOT_BUSY;
    static const int CTAP2_ERR_NO_OPERATION_PENDING;
    static const int CTAP2_ERR_UNSUPPORTED_OPTION;
    static const int CTAP2_ERR_INVALID_OPTION;
    static const int CTAP2_ERR_KEEPALIVE_CANCEL;
    static const int CTAP2_ERR_NO_CREDENTIALS;
    static const int CTAP2_ERR_USER_ACTION_TIMEOUT;
    static const int CTAP2_ERR_NOT_ALLOWED;
    static const int CTAP2_ERR_PIN_INVALID;
    static const int CTAP2_ERR_PIN_BLOCKED;
    static const int CTAP2_ERR_PIN_AUTH_INVALID;
    static const int CTAP2_ERR_PIN_AUTH_BLOCKED;
    static const int CTAP2_ERR_PIN_NOT_SET;
    static const int CTAP2_ERR_PIN_REQUIRED;
    static const int CTAP2_ERR_PIN_POLICY_VIOLATION;
    static const int CTAP2_ERR_PIN_TOKEN_EXPIRED;
    static const int CTAP2_ERR_REQUEST_TOO_LARGE;
    static const int CTAP2_ERR_ACTION_TIMEOUT;
    static const int CTAP2_ERR_UP_REQUIRED;
    static const int CTAP1_ERR_OTHER;
    static const int CTAP2_ERR_SPEC_LAST;
    static const int CTAP2_ERR_EXTENSION_FIRST;
    static const int CTAP2_ERR_EXTENSTION_LAST;
    static const int CTAP2_ERR_VENDOR_FIRST;
    static const int CTAP2_ERR_VENDOR_LAST;
};

/**
 * @struct MakeCredentialParam
 * @brief authenticatorMakeCredentialの入力値(CBOR)のキー
 */
struct MakeCredentialParam {
    static const int KEY_CLIENT_DATA_HASH;
    static const int KEY_RP;
    static const int KEY_USER;
    static const int KEY_PUBKEY_CRED_PARAM;
    static const int KEY_EXCLUDE_LIST;
    static const int KEY_EXTENSIONS;
    static const int KEY_OPTIONS;
    static const int KEY_PIN_AUTH;
    static const int KEY_PIN_PROTOCOL;
    /* ------------keydata testParam.------------- */
    static const int KEY_TPK;
    static const int KEY_APK;
    static const int KEY_SKA;
};

/**
 * @struct GetAssertionParam
 * @brief authenticatorGetAssertionの入力値(CBOR)のキー
 */
struct GetAssertionParam {
    static const int KEY_RPID;
    static const int KEY_CLIENT_DATA_HASH;
    static const int KEY_ALLOW_LIST;
    static const int KEY_EXTENSIONS;
    static const int KEY_OPTIONS;
    static const int KEY_PIN_AUTH;
    static const int KEY_PIN_PROTOCOL;
    static const int KEY_POLICY;
};

/**
 * @struct ClientPINParam
 * @brief authenticatorClientPINの入力値(CBOR)のキー
 */
struct ClientPINParam {
    static const int KEY_PIN_PROTOCOL;
    static const int KEY_SUB_COMMAND;
    static const int KEY_KEY_AGREEMENT;
    static const int KEY_PIN_AUTH;
    static const int KEY_NEW_PIN_ENC;
    static const int KEY_PIN_HASH_ENC;
};

/**
 * @struct MakeCredentialResponseParam
 * @brief authenticatorMakeCredentialのResponse(CBOR)のキー
 */
struct MakeCredentialResponseParam {
    static const int KEY_FMT;
    static const int KEY_AUTH_DATA;
    static const int KEY_ATT_STMT;
};

/**
 * @struct GetAssertionResponseParam
 * @brief authenticatorGetAssertionのRespose(CBOR)のキー
 */
struct GetAssertionResponseParam {
    static const int KEY_CREDENTIAL;
    static const int KEY_AUTH_DATA;
    static const int KEY_SIGNATURE;
    static const int KEY_PUBKEY_CRED_USER_ENTITY;
    static const int KEY_NUMBER_OF_CRED;
};

/**
 * @struct GetNextAssertionResponseParam
 * @brief authenticatorGetNextAssertionのResponse(CBOR)のキー
 */
struct GetNextAssertionResponseParam {
    static const int KEY_CREDENTIAL;
    static const int KEY_AUTH_DATA;
    static const int KEY_SIGNATURE;
    static const int KEY_PUBKEY_CRED_USER_ENTITY;
};

/**
 * @struct GetInfoResponseParam
 * @brief authenticatorGetInfoのResponse(CBOR)のキー
 */
struct GetInfoResponseParam {
    static const int KEY_VERSIONS;
    static const int KEY_EXTENSIONS;
    static const int KEY_AAGUID;
    static const int KEY_OPTIONS;
    static const int KEY_MAX_MSG_SIZE;
    static const int KEY_PIN_PROTOCOLS;
};
/**
 * @struct ClientPINResponseParam
 * @brief authenticatorClientPINのResponse(CBOR)のキー
 */
struct ClientPINResponseParam {
    static const int KEY_KEY_AGREEMENT;
    static const int KEY_PIN_TOKEN;
    static const int KEY_RETRIES;
};

#endif