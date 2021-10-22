#include "message.hpp"

/* ----------------------Command---------------------- */
Command::~Command() {
    // Serial.println("Command destroy");
    // delete[] commandParameter;
}

/* ----------------------Response---------------------- */
Response::~Response() {
    // Serial.println("Response destroy");
    // delete[] responseData;
}

/* ----------------------StatusCodeParam---------------------- */
/** @brief Indicates successful response. (0x00) */
const int StatusCodeParam::CTAP1_ERR_SUCCESS = 0x00;

/** @brief The command is not a valid CTAP command. (0x01) */
const int StatusCodeParam::CTAP1_ERR_INVALID_COMMAND = 0x01;

/** @brief The command included an invalid parameter. (0x02) */
const int StatusCodeParam::CTAP1_ERR_INVALID_PARAMETER = 0x02;

/** @brief Invalid message or item length. (0x03) */
const int StatusCodeParam::CTAP1_ERR_INVALID_LENGTH = 0x03;

/** @brief Invalid message sequencing. (0x04) */
const int StatusCodeParam::CTAP1_ERR_INVALID_SEQ = 0x04;

/** @brief Message timed out. (0x05) */
const int StatusCodeParam::CTAP1_ERR_TIMEOUT = 0x05;

/** @brief Channel busy. (0x06) */
const int StatusCodeParam::CTAP1_ERR_CHANNEL_BUSY = 0x06;

/** @brief Command requires channel lock. (0x0A) */
const int StatusCodeParam::CTAP1_ERR_LOCK_REQUIRED = 0x0A;

/** @brief Command not allowed on this cid. (0x0B) */
const int StatusCodeParam::CTAP1_ERR_INVALID_CHANNEL = 0x0B;

/** @brief Invalid/unexpected CBOR error. (0x11) */
const int StatusCodeParam::CTAP2_ERR_CBOR_UNEXPECTED_TYPE = 0x11;

/** @brief Error when parsing CBOR. (0x12) */
const int StatusCodeParam::CTAP2_ERR_INVALID_CBOR = 0x12;

/** @brief Missing non-optional parameter. (0x14) */
const int StatusCodeParam::CTAP2_ERR_MISSING_PARAMETER = 0x14;

/** @brief Limit for number of items exceeded. (0x15) */
const int StatusCodeParam::CTAP2_ERR_LIMIT_EXCEEDED = 0x15;

/** @brief Unsupported extension. (0x16) */
const int StatusCodeParam::CTAP2_ERR_UNSUPPORTED_EXTENSION = 0x16;

/** @brief Valid credential found in the exclude list. (0x19) */
const int StatusCodeParam::CTAP2_ERR_CREDENTIAL_EXCLUDED = 0x19;

/** @brief Processing (Lengthy operation is in progress). (0x21) */
const int StatusCodeParam::CTAP2_ERR_PROCESSING = 0x21;

/** @brief Credential not valid for the authenticator. (0x22) */
const int StatusCodeParam::CTAP2_ERR_INVALID_CREDENTIAL = 0x22;

/** @brief Authentication is waiting for user interaction. (0x23) */
const int StatusCodeParam::CTAP2_ERR_USER_ACTION_PENDING = 0x23;

/** @brief Processing, lengthy operation is in progress. (0x24) */
const int StatusCodeParam::CTAP2_ERR_OPERATION_PENDING = 0x24;

/** @brief No request is pending. (0x25) */
const int StatusCodeParam::CTAP2_ERR_NO_OPERATIONS = 0x25;

/** @brief Authenticator does not support requested algorithm. (0x26) */
const int StatusCodeParam::CTAP2_ERR_UNSUPPORTED_ALGORITHM = 0x26;

/** @brief Not authorized for requested operation. (0x27) */
const int StatusCodeParam::CTAP2_ERR_OPERATION_DENIED = 0x27;

/** @brief Internal key storage is full. (0x28) */
const int StatusCodeParam::CTAP2_ERR_KEY_STORE_FULL = 0x28;

/** @brief Authenticator cannot cancel as it is not busy. (0x29) */
const int StatusCodeParam::CTAP2_ERR_NOT_BUSY = 0x29;

/** @brief No outstanding operations. (0x2A) */
const int StatusCodeParam::CTAP2_ERR_NO_OPERATION_PENDING = 0x2A;

/** @brief Unsupported option. (0x2B) */
const int StatusCodeParam::CTAP2_ERR_UNSUPPORTED_OPTION = 0x2B;

/** @brief Not a valid option for current operation. (0x2C) */
const int StatusCodeParam::CTAP2_ERR_INVALID_OPTION = 0x2C;

/** @brief Pending keep alive was cancelled. (0x2D) */
const int StatusCodeParam::CTAP2_ERR_KEEPALIVE_CANCEL = 0x2D;

/** @brief No valid credentials provided. (0x2E) */
const int StatusCodeParam::CTAP2_ERR_NO_CREDENTIALS = 0x2E;

/** @brief Timeout waiting for user interaction. (0x2F) */
const int StatusCodeParam::CTAP2_ERR_USER_ACTION_TIMEOUT = 0x2F;

/** @brief Continuation command, such as, authenticatorGetNextAssertion not allowed. (0x30) */
const int StatusCodeParam::CTAP2_ERR_NOT_ALLOWED = 0x30;

/** @brief PIN Invalid. (0x31) */
const int StatusCodeParam::CTAP2_ERR_PIN_INVALID = 0x31;

/** @brief PIN Blocked. (0x32) */
const int StatusCodeParam::CTAP2_ERR_PIN_BLOCKED = 0x32;

/** @brief PIN authentication,pinAuth, verification failed. (0x33) */
const int StatusCodeParam::CTAP2_ERR_PIN_AUTH_INVALID = 0x33;

/** @brief PIN authentication,pinAuth, blocked. Requires power recycle to reset. (0x34) */
const int StatusCodeParam::CTAP2_ERR_PIN_AUTH_BLOCKED = 0x34;

/** @brief No PIN has been set. (0x35) */
const int StatusCodeParam::CTAP2_ERR_PIN_NOT_SET = 0x35;

/** @brief PIN is required for the selected operation. (0x36) */
const int StatusCodeParam::CTAP2_ERR_PIN_REQUIRED = 0x36;

/** @brief PIN policy violation. Currently only enforces minimum length. (0x37) */
const int StatusCodeParam::CTAP2_ERR_PIN_POLICY_VIOLATION = 0x37;

/** @brief pinToken expired on authenticator. (0x38) */
const int StatusCodeParam::CTAP2_ERR_PIN_TOKEN_EXPIRED = 0x38;

/** @brief Authenticator cannot handle this request due to memory constraints. (0x39) */
const int StatusCodeParam::CTAP2_ERR_REQUEST_TOO_LARGE = 0x39;

/** @brief The current operation has timed out. (0x3A) */
const int StatusCodeParam::CTAP2_ERR_ACTION_TIMEOUT = 0x3A;

/** @brief User presence is required for the requested operation. (0x3B) */
const int StatusCodeParam::CTAP2_ERR_UP_REQUIRED = 0x3B;

/** @brief Other unspecified error. (0x7F) */
const int StatusCodeParam::CTAP1_ERR_OTHER = 0x7F;

/** @brief CTAP 2 spec last error. (0xDF) */
const int StatusCodeParam::CTAP2_ERR_SPEC_LAST = 0xDF;

/** @brief Extension specific error. (0xE0) */
const int StatusCodeParam::CTAP2_ERR_EXTENSION_FIRST = 0xE0;

/** @brief Extension specific error. (0xEF) */
const int StatusCodeParam::CTAP2_ERR_EXTENSTION_LAST = 0xEF;

/** @brief Vendor specific error. (0xF0) */
const int StatusCodeParam::CTAP2_ERR_VENDOR_FIRST = 0xF0;

/** @brief Vendor specific error. (0xFF) */
const int StatusCodeParam::CTAP2_ERR_VENDOR_LAST = 0xFF;


/* ----------------------MakeCredentialParam---------------------- */
/** @brief byte string (CBOR major type 2) */
const int MakeCredentialParam::KEY_CLIENT_DATA_HASH = 0x01;

/** @brief CBOR definite length map (CBOR major type 5) */
const int MakeCredentialParam::KEY_RP = 0x02;

/** @brief CBOR definite length map (CBOR major type 5) */
const int MakeCredentialParam::KEY_USER = 0x03;

/** @brief CBOR definite length array (CBOR major type 4) of CBOR definite length maps (CBOR major type 5) */
const int MakeCredentialParam::KEY_PUBKEY_CRED_PARAM = 0x04;

/** @brief CBOR definite length array (CBOR major type 4) of CBOR definite length maps (CBOR major type 5). */
const int MakeCredentialParam::KEY_EXCLUDE_LIST = 0x05;

/** @brief CBOR definite length map (CBOR major type 5) */
const int MakeCredentialParam::KEY_EXTENSIONS = 0x06;

/** @brief CBOR definite length map (CBOR major type 5). */
const int MakeCredentialParam::KEY_OPTIONS = 0x07;

/** @brief byte string (CBOR major type 2). */
const int MakeCredentialParam::KEY_PIN_AUTH = 0x08;

/** @brief PIN protocol version chosen by the client. For this version of the spec, this SHALL be the number 1.*/
const int MakeCredentialParam::KEY_PIN_PROTOCOL = 0x09;

/* ------------keydata testParam.------------- */
const int MakeCredentialParam::KEY_TPK = 0x0a;
const int MakeCredentialParam::KEY_APK = 0x0b;
const int MakeCredentialParam::KEY_SKA = 0x0c;

/* ----------------------GetAssertionParam---------------------- */
/** @brief UTF-8 encoded text string (CBOR major type 3). */
const int GetAssertionParam::KEY_RPID = 0x01;

/** @brief byte string (CBOR major type 2). */
const int GetAssertionParam::KEY_CLIENT_DATA_HASH = 0x02;

/** @brief CBOR definite length array (CBOR major type 4) of CBOR definite length maps (CBOR major type 5). */
const int GetAssertionParam::KEY_ALLOW_LIST = 0x03;

/** @brief CBOR definite length map (CBOR major type 5). */
const int GetAssertionParam::KEY_EXTENSIONS = 0x04;

/** @brief CBOR definite length map (CBOR major type 5). */
const int GetAssertionParam::KEY_OPTIONS = 0x05;

/** @brief byte string (CBOR major type 2). */
const int GetAssertionParam::KEY_PIN_AUTH = 0x06;

/** @brief PIN protocol version chosen by the client. For this version of the spec, this SHALL be the number 1. */
const int GetAssertionParam::KEY_PIN_PROTOCOL = 0x07;


/* ----------------------ClientPINParam---------------------- */
/** @brief Unsigned Integer. (CBOR major type 0) */
const int ClientPINParam::KEY_PIN_PROTOCOL = 0x01;

/** @brief Unsigned Integer. (CBOR major type 0) */
const int ClientPINParam::KEY_SUB_COMMAND = 0x02;

/** @brief COSE_Key */
const int ClientPINParam::KEY_KEY_AGREEMENT = 0x03;

/** @brief byte string (CBOR major type 2). */
const int ClientPINParam::KEY_PIN_AUTH = 0x04;

/** @brief byte string (CBOR major type 2). It is UTF-8 representation of encrypted input PIN value. */
const int ClientPINParam::KEY_NEW_PIN_ENC = 0x05;

/** @brief byte string (CBOR major type 2). */
const int ClientPINParam::KEY_PIN_HASH_ENC = 0x06;


/* ----------------------MakeCredentialResponseParam---------------------- */
/** @brief text string (CBOR major type 3). */
const int MakeCredentialResponseParam::KEY_FMT = 0x01;

/** @brief byte string (CBOR major type 2). */
const int MakeCredentialResponseParam::KEY_AUTH_DATA = 0x02;

/** @brief definite length map (CBOR major type 5). */
const int MakeCredentialResponseParam::KEY_ATT_STMT = 0x03;


/* ----------------------GetAssertionResponseParam---------------------- */
/** @brief definite length map (CBOR major type 5). */
const int GetAssertionResponseParam::KEY_CREDENTIAL = 0x01;

/** @brief byte string (CBOR major type 2). */
const int GetAssertionResponseParam::KEY_AUTH_DATA = 0x02;

/** @brief byte string (CBOR major type 2). */
const int GetAssertionResponseParam::KEY_SIGNATURE = 0x03;

/** @brief definite length map (CBOR major type 5). */
const int GetAssertionResponseParam::KEY_PUBKEY_CRED_USER_ENTITY = 0x04;

/** @brief unsigned integer(CBOR major type 0). */
const int GetAssertionResponseParam::KEY_NUMBER_OF_CRED = 0x05;


/* ----------------------GetNextAssertionResponseParam---------------------- */
/** @brief definite length map (CBOR major type 5). */
const int GetNextAssertionResponseParam::KEY_CREDENTIAL = 0x01;

/** @brief byte string (CBOR major type 2). */
const int GetNextAssertionResponseParam::KEY_AUTH_DATA = 0x02;

/** @brief byte string (CBOR major type 2). */
const int GetNextAssertionResponseParam::KEY_SIGNATURE = 0x03;

/** @brief definite length map (CBOR major type 5). */
const int GetNextAssertionResponseParam::KEY_PUBKEY_CRED_USER_ENTITY = 0x04;


/* ----------------------GetInfoResponseParam---------------------- */
/** @brief definite length array (CBOR major type 4) of UTF-8 encoded strings (CBOR major type 3). */
const int GetInfoResponseParam::KEY_VERSIONS = 0x01;

/** @brief definite length array (CBOR major type 4) of UTF-8 encoded strings (CBOR major type 3). */
const int GetInfoResponseParam::KEY_EXTENSIONS = 0x02;

/** @brief byte string (CBOR major type 2). 16 bytes in length and encoded the same as MakeCredential AuthenticatorData, as specified in [WebAuthN]. */
const int GetInfoResponseParam::KEY_AAGUID = 0x03;

/** @brief Definite length map (CBOR major type 5) of key-value pairs where keys are UTF8 strings (CBOR major type 3) and values are booleans (CBOR simple value 21). */
const int GetInfoResponseParam::KEY_OPTIONS = 0x04;

/** @brief unsigned integer(CBOR major type 0). This is the maximum message size supported by the authenticator. */
const int GetInfoResponseParam::KEY_MAX_MSG_SIZE = 0x05;

/** @brief array of unsigned integers (CBOR major type). This is the list of pinProtocols supported by the authenticator. */
const int GetInfoResponseParam::KEY_PIN_PROTOCOLS = 0x06;


/* ----------------------ClientPINResponseParam---------------------- */
/** @brief Authenticator public key in COSE_Key format. The COSE_Key-encoded public key MUST contain the optional "alg" parameter and MUST NOT contain any other optional parameters. The "alg" parameter MUST contain a COSEAlgorithmIdentifier value. */
const int ClientPINResponseParam::KEY_KEY_AGREEMENT = 0x01;

/** @brief byte string (CBOR major type 2). */
const int ClientPINResponseParam::KEY_PIN_TOKEN = 0x02;

/** @brief Unsigned integer (CBOR major type 0). This is number of retries left before lockout. */
const int ClientPINResponseParam::KEY_RETRIES = 0x03;