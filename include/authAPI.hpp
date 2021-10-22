#ifndef INCLUDED_authAPI_h_
#define INCLUDED_authAPI_h_

#include "message.hpp"
#include "abs.hpp"
#include "YACL.h"
#include <exception>
// #include <M5StickC.h>

#include "sha256.h"

/**
 * @struct AuthenticatorAPICommandParam
 * @brief AuthenticatorAPIのコマンド識別子
 */
struct AuthenticatorAPICommandParam {
    static const int COMMAND_MAKECREDENTIAL;
    static const int COMMAND_GETASSERTION;
    static const int COMMAND_GETINFO;
    static const int COMMAND_CLIENTPIN;
    static const int COMMAND_RESET;
    static const int COMMAND_GETNEXTASSERTION;
    static const int COMMAND_VENDORFIRST;
    static const int COMMAND_VENDORLAST;
};

/**
 * @struct AuthDataSizeParam
 * @brief AuthDataの固定長サイズ
 */
struct AuthDataSizeParam {
    static const int AUTHDATA_RPIDHASH;
    static const int AUTHDATA_FLAGS;
    static const int AUTHDATA_COUNTER;
};

/**
 * @struct AttestedCredentialData
 * @brief AttestedCredentialDataの固定長サイズ
 */
struct AttestedCredentialDataSizeParam {
    static const int ATTESTED_AAGUID;
    static const int ATTESTED_LENGTH; 
};

/**
 * @brief AuthenticatorMakeCredentialの引数であるRPの構造体
 * @struct PublicKeyCredentialRpEntity
 */
struct PublicKeyCredentialRpEntity {
    /**
     * @brief the Relying Party Identifier
     */
    String id;

    /**
     * @brief a human-friendly RP name
     */
    String name;

    /**
     * @brief a URL referencing a RP icon image
     */
    String iconURL;
};

/**
 * @brief AuthenticatorMakeCredentialの引数であるuserの構造体
 * @struct PublicKeyCredentialUserEntity
 */
struct PublicKeyCredentialUserEntity {
    /**
     * @brief an RP-specific user account identifier
     */
    uint8_t *id;

    /**
     * @brief a user name
     */
    String name;

    /**
     * @brief a user display name
     */
    String displayName;

    /**
     * @brief a URL referencing a user icon image
     */
    String iconURL;

    /**
     * @brief Destroy the Public Key Credential User Entity object
     */
    ~PublicKeyCredentialUserEntity();
};

/**
 * @brief AuthenticatorMakeCredentialの引数であるpubKeyCredParamの構造体
 * @struct pubKeyCredParam
 */
struct PubKeyCredParam {
    /**
     * @brief cryptographic algorithm (a positive or negative integer)
     */
    int alg;

    /**
     * @brief PublicKeyCredentailType (a string)
     */
    String type;
};

struct ParsedMakeCredentialParams {
    /**
     * @brief clientDataHash - ByteArray
     */
    uint8_t *hash;

    /**
     * @brief rp - PublicKeyCredentialRpEntity 
     */
    PublicKeyCredentialRpEntity *rp;

    /**
     * @brief user - PublicKeyCredentialUserEntity
     */
    PublicKeyCredentialUserEntity *user;

    /**
     * @brief pubKeyCredParams - PubKeyCredParams;
     */
    PubKeyCredParam *pubKeyCredParams;

    /**
     * @brief request data of CBOR - CBOR
     */
    CBOR data;

    /**
     * @brief clientDataHash of CBOR - CBOR
     */
    CBOR cbor_clientDataHash;

    /**
     * @brief rp of CBOR - CBOR
     */
    CBOR cbor_rp;

    /**
     * @brief user of CBOR - CBOR
     */
    CBOR cbor_user;

    /**
     * @brief pubKeyCredParams of CBOR - CBOR
     */
    CBOR cbor_pubKeyCredParams;

    /**
     * @brief Destroy the Parsed Make Credential Params object
     */
    ~ParsedMakeCredentialParams();
};

struct ParsedGetAssertionParams {
    /**
     * @brief rpId - String
     */
    String rpId;

    /**
     * @brief clientDataHash - ByteArray
     */
    uint8_t *hash;

    /**
     * @brief request data of CBOR - CBOR
     */
    CBOR data;

    /**
     * @brief rpId of CBOR - CBOR
     */
    CBOR cbor_rpId;

    /**
     * @brief clietnDataHash of CBOR - CBOR
     */
    CBOR cbor_clientDataHash;

    /**
     * @brief Destroy the Parsed Make Credential Params object
     */
    ~ParsedGetAssertionParams();
};

/**
 *  @class AuthenticatorAPI
 *  @brief AuthenticatorAPIの各メソッドを実行するクラス
 */
class AuthenticatorAPI {
    private:
        /**
         * @var command
         * @brief AuthenticatorAPIのコマンド識別子
         */
        unsigned int command;

        /**
         * @var parameter
         * @brief AuthenticatorAPIの引数パラメータ
         */
        uint8_t *parameter;

        /**
         *  @var len
         *  @brief parameterの長さ(llen-hlen)
         */
        unsigned int length;

        /**
         * @var tpk
         * @brief Trustee Public Key
         */
        TPK *tpk;

        /**
         * @var apk
         * @brief Public Key
         */
        APK *apk;

        /**
         * @var ska
         * @brief User Secret Key
         */
        SKA *ska;

    public:
        AuthenticatorAPI();
        AuthenticatorAPI(unsigned int command);
        AuthenticatorAPI(unsigned int command, uint8_t *parameter, unsigned int length);
        ~AuthenticatorAPI();
        Response *operateCommand();
        Response *authenticatorMakeCredential(ParsedMakeCredentialParams *params);
        Response *authenticatorGetAssertion(ParsedGetAssertionParams *params);
        Response *authenticatorGetInfo();
        Response *authenticatorClientPIN();
        Response *authenticatorReset();
        Response *authenticatorGetNextAssertion();
        Response *authenticatorVendorFirst();
        Response *authenciatorVendorLast();

        unsigned int getCommand();
        uint8_t *getParameter();
        unsigned int getLength();
        TPK *getTPK();
        APK *getAPK();
        SKA *getSKA();
        void setCommand(unsigned int command);
        void setParameter(uint8_t *parameter);
        void setLength(unsigned int length);
        void setTPK(TPK *tpk);
        void setAPK(APK *APK);
        void setSKA(SKA *SKA);
};

bool checkHasParameters(unsigned int command);
uint8_t *generateSha256(String data);

/**
 * @class implement_error
 * @brief 未実装のエラー定義
 */
class implement_error : public std::exception {
    private:
        const char *msg;
    public:
        implement_error(const char* msg) {
            this->msg = msg;
        };
        const char* what() {
            return this->msg;
        };
};

#endif