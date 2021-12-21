#include "authAPI.hpp"

/* ----------------------AuthenticatorAPICommandParam---------------------- */
/** @brief authenticatorMakeCredentialのコマンド識別子(0x01) */
const int AuthenticatorAPICommandParam::COMMAND_MAKECREDENTIAL = 0x01;

/** @brief authenticatorGetAssertionのコマンド識別子(0x02) */
const int AuthenticatorAPICommandParam::COMMAND_GETASSERTION = 0x02;

/** @brief authenticatorGetInfoのコマンド識別子(0x04) */
const int AuthenticatorAPICommandParam::COMMAND_GETINFO = 0x04;

/** @brief authenticatorClientPINのコマンド識別子(0x06) */
const int AuthenticatorAPICommandParam::COMMAND_CLIENTPIN = 0x06;

/** @brief authenticatorResetのコマンド識別子(0x07) */
const int AuthenticatorAPICommandParam::COMMAND_RESET = 0x07;

/** @brief authenticatorGetNextAssertionのコマンド識別子(0x08) */
const int AuthenticatorAPICommandParam::COMMAND_GETNEXTASSERTION = 0x08;

/** @brief authenticatorVendorFirstのコマンド識別子(0x40) */
const int AuthenticatorAPICommandParam::COMMAND_VENDORFIRST = 0x40;

/** @brief authenticatorVendorLastのコマンド識別子(0xbf) */
const int AuthenticatorAPICommandParam::COMMAND_VENDORLAST = 0xbf;


/* ----------------------AuthDataSizeParam---------------------- */
/** @brief AuthDataのRPIDHashの固定長サイズ(32bytes) */
const int AuthDataSizeParam::AUTHDATA_RPIDHASH = 32;

/** @brief AuthDataのFlagsの固定長サイズ(1bytes) */
const int AuthDataSizeParam::AUTHDATA_FLAGS = 1;

/** @brief AuthDataのCounterの固定長サイズ(4bytes) */
const int AuthDataSizeParam::AUTHDATA_COUNTER = 4;


/* ----------------------AttestedCredentialDataSizeParam---------------------- */
/** @brief AttestedCredentialDataのAAGUIDの固定長サイズ(16bytes) */
const int AttestedCredentialDataSizeParam::ATTESTED_AAGUID = 16;

/** @brief AttestedCredentialDataのLENGTHの固定長サイズ2bytes) */
const int AttestedCredentialDataSizeParam::ATTESTED_LENGTH = 2;


/* ----------------------PublicKeyCredentialUserEntity---------------------- */
PublicKeyCredentialUserEntity::~PublicKeyCredentialUserEntity() {
    // Serial.println("delete PublicKeyCredentialUserEntity");
    delete[] this->id;
}

/* ----------------------ParsedMakeCredentialParams---------------------- */
ParsedMakeCredentialParams::~ParsedMakeCredentialParams() {
    // Serial.println("delete ParsedMakeCredentialParams");
    delete[] this->hash;
    delete this->rp;
    delete this->user;
    delete this->pubKeyCredParams;
}

ParsedGetAssertionParams::~ParsedGetAssertionParams() {
    delete[] this->hash;
}

/* ----------------------DebugUtility---------------------- */
/**
 * @brief responseのデバッグ情報出力(Serial)
 * 
 * @param response - responseデータ
 * @param data_len - responseDataの長さ
 */
void responseSerialDebug(Response response, size_t data_len) {
    // Serial.printf("Response Status:%x\n", response.status);
    for (size_t i=0; i < data_len; ++i) {
        // Serial.printf("%.2x", response.responseData[i]);
    }
    // Serial.println("Serial Debug");
}

/**
 * @brief Stringのデバッグ情報出力(Serial)
 * 
 * @param prop - prop表記
 * @param str - String本体
 */
void stringSerialDebug(String prop, String str) {
    // Serial.print(prop);
    // Serial.println(str);
}

/**
 * @brief uint8_tのデバッグ情報出力(Serial)
 * 
 * @param prop - prop表記
 * @param data - uint8_t本体
 * @param data_len - uint8_tの長さ
 */
void uint8SerialDebug(String prop, uint8_t *data, size_t data_len) {
    // Serial.print(prop);
    for (size_t i=0; i < data_len; ++i) {
        // Serial.printf("%.2x", data[i]);
    }
    // Serial.println("");
}

/**
 * @brief intのデバッグ情報出力(Serial)
 * 
 * @param prop - prop表記
 * @param data - int本体
 */
void intSerialDebug(String prop, int data) {
    // Serial.print(prop);
    // Serial.println(data);
}

/* ----------------------AuthenticatorAPI---------------------- */
/**
 * @brief Construct a new AuthenticatorAPI::AuthenticatorAPI object
 * 
 */
AuthenticatorAPI::AuthenticatorAPI() {
    this->command = 0x00;
    this->parameter = 0;
    this->length = 0;
}

/**
 * @brief Construct a new AuthenticatorAPI:: AuthenticatorAPI object
 * 
 * @param command AuthenticatorAPIのコマンド識別子
 */
AuthenticatorAPI::AuthenticatorAPI(unsigned int command) {
    this->command = command;
    this->parameter = 0;
    this->length = 0;
}

/**
 * @brief Construct a new AuthenticatorAPI:: AuthenticatorAPI object
 * 
 * @param command AuthenticatorAPIのコマンド識別子
 * @param parameter AuthenticatorAPIのパラメータ
 * @param length AuthenticatorAPIのパラメータの長さ
 */
AuthenticatorAPI::AuthenticatorAPI(unsigned int command, uint8_t *parameter, unsigned int length) {
    this->command = command;
    this->parameter = parameter;
    this->length = length;
}

AuthenticatorAPI::~AuthenticatorAPI() {
    // Serial.println("authAPI destroy");
}

/**
 * @brief Commandに対応したAuthenticatorAPIを実行する
 *        Requestをパースしたのち各APIの機能を呼び出す
 * 
 * @return Response - AuthenticatorAPIの返り値
 */
Response *AuthenticatorAPI::operateCommand() {
    Response *response;

    if (this->command == AuthenticatorAPICommandParam::COMMAND_MAKECREDENTIAL) {
        /**
         * MemberName           Data type           Required?
         * -------------        ----------          ----------
         * clientDataHash:      Byte Array          Required
         * rp:                  PubkeyCredEntity    Required 
         * users:               PubkeyCredEnity     Required
         * pubKeyCredParams:    CBOR Array          Required
         * excludeList          Seq of PubkeyDesc   Optional
         * extensions           CBOR map            Optional
         * options              Map of options      Optional
         * pinAuth              ByteArray           Optional
         * pinProtocol          Unsigned Int        Optional
         */

        // Serial.println("Make Credential.");
        ParsedMakeCredentialParams *params = new ParsedMakeCredentialParams;

        params->data = CBOR(this->parameter, this->length, true);

        /* clientDataHash */
        if (params->data[MakeCredentialParam::KEY_CLIENT_DATA_HASH].is_bytestring()) {
            params->cbor_clientDataHash = params->data[MakeCredentialParam::KEY_CLIENT_DATA_HASH];
            params->hash = new uint8_t[params->cbor_clientDataHash.get_bytestring_len()];
            params->cbor_clientDataHash.get_bytestring(params->hash);
            // uint8SerialDebug("hash:", params->hash, params->cbor_clientDataHash.get_bytestring_len());
        }

        /* rp  */
        params->rp = new PublicKeyCredentialRpEntity;
        if (params->data[MakeCredentialParam::KEY_RP].is_pair()) {
            params->cbor_rp = params->data[MakeCredentialParam::KEY_RP];
            params->cbor_rp["id"].get_string(params->rp->id);
            params->cbor_rp["name"].get_string(params->rp->name);
            // stringSerialDebug("rp.id:", params->rp->id);
            // stringSerialDebug("rp.name:", params->rp->name);
        }

        /* user */
        params->user = new PublicKeyCredentialUserEntity;
        if (params->data[MakeCredentialParam::KEY_USER].is_pair()) {
            params->cbor_user = params->data[MakeCredentialParam::KEY_USER];
            params->user->id = new uint8_t[params->cbor_user["id"].get_bytestring_len()];
            params->cbor_user["id"].get_bytestring(params->user->id);
            params->cbor_user["name"].get_string(params->user->name);
            params->cbor_user["displayName"].get_string(params->user->displayName);
            // uint8SerialDebug("user.id:", params->user->id, params->cbor_user["id"].get_bytestring_len());
            // stringSerialDebug("user.name:", params->user->name);
            // stringSerialDebug("user.displayName:", params->user->displayName);
        }

        /* pubKeyCredParams */
        /* TODO:pubKeyCredParamsのサイズの可変長化(2つ以上引数として求められる場合がある) */
        params->pubKeyCredParams = new PubKeyCredParam;
        if (params->data[MakeCredentialParam::KEY_PUBKEY_CRED_PARAM].is_array()) {
            params->cbor_pubKeyCredParams = params->data[MakeCredentialParam::KEY_PUBKEY_CRED_PARAM];
            CBOR pubKeyCredParam = params->cbor_pubKeyCredParams[0];
            params->pubKeyCredParams->alg = (int32_t)pubKeyCredParam["alg"];
            pubKeyCredParam["type"].get_string(params->pubKeyCredParams->type);
            // intSerialDebug("pubKeyCredParam.alg:", params->pubKeyCredParams->alg);
            // stringSerialDebug("pubKeyCredParams.type:", params->pubKeyCredParams->type);
        }

        if (params->data[MakeCredentialParam::KEY_TPK].is_bytestring()) {
            CBOR cbor_tpk = params->data[MakeCredentialParam::KEY_TPK];
            uint8_t *raw_tpk = new uint8_t[cbor_tpk.get_bytestring_len()];
            cbor_tpk.get_bytestring(raw_tpk); // CBORデータが正しく取れていることは確認済み
            this->tpk = new TPK(CBOR(raw_tpk, cbor_tpk.get_bytestring_len(), true));
            this->tpk->parse();
            /* TODO:適切な解放 */
            // delete raw_tpk; /* 解放するとCBORデータごと吹き飛ぶ */
        }

        if (params->data[MakeCredentialParam::KEY_APK].is_bytestring()) {
            CBOR cbor_apk = params->data[MakeCredentialParam::KEY_APK];
            uint8_t *raw_apk = new uint8_t[cbor_apk.get_bytestring_len()];
            cbor_apk.get_bytestring(raw_apk);
            this->apk = new APK(CBOR(raw_apk, cbor_apk.get_bytestring_len(), true));
            this->apk->parse();
            /* TODO:適切な解放 */
            // delete raw_apk;　/* 解放するとCBORデータごと吹き飛ぶ */
        }

        if (params->data[MakeCredentialParam::KEY_SKA].is_bytestring()) {
            CBOR cbor_ska = params->data[MakeCredentialParam::KEY_SKA];
            uint8_t *raw_ska = new uint8_t[cbor_ska.get_bytestring_len()];
            cbor_ska.get_bytestring(raw_ska);
            this->ska = new SKA(CBOR(raw_ska, cbor_ska.get_bytestring_len(), true));
            this->ska->parse();
            /* TODO:適切な解放 */
            // delete raw_ska;　/* 解放するとCBORデータごと吹き飛ぶ */
        }

        /* API呼び出し */
        response = this->authenticatorMakeCredential(params);

        return response;

    } else if (this->command == AuthenticatorAPICommandParam::COMMAND_GETASSERTION) {
        /**
         * MemberName           Data type           Required?
         * -------------        ----------          ----------
         * rpId:                String              Required
         * clientDataHash:      Byte Array          Required
         * allowList            Seq of PubkeyDesc   Optional
         * extensions           CBOR map            Optional
         * options              Map of options      Optional
         * pinAuth              ByteArray           Optional
         * pinProtocol          Unsigned Int        Optional
         */
        // Serial.println("Get Assertion.");
        ParsedGetAssertionParams *params = new ParsedGetAssertionParams;

        params->data = CBOR(this->parameter, this->length, true);

        /* rpId */
        if (params->data[GetAssertionParam::KEY_RPID].is_string()) {
            params->cbor_rpId = params->data[GetAssertionParam::KEY_RPID];
            params->cbor_rpId.get_string(params->rpId);
            // stringSerialDebug("rpId:", params->rpId);
        }

        /* clientDataHash */
        if (params->data[GetAssertionParam::KEY_CLIENT_DATA_HASH].is_bytestring()) {
            params->cbor_clientDataHash = params->data[GetAssertionParam::KEY_CLIENT_DATA_HASH];
            params->hash = new uint8_t[params->cbor_clientDataHash.get_bytestring_len()];
            params->cbor_clientDataHash.get_bytestring(params->hash);
            // uint8SerialDebug("hash:", params->hash, params->cbor_clientDataHash.get_bytestring_len());
        }

        /* policy */
        if (params->data[GetAssertionParam::KEY_POLICY].is_string()) {
            params->cbor_policy = params->data[GetAssertionParam::KEY_POLICY];
            params->cbor_policy.get_string(params->policy);
            // stringSerialDebug("policy:", params->policy);
        }

        response = this->authenticatorGetAssertion(params);

        return response;

    } else if (this->command == AuthenticatorAPICommandParam::COMMAND_GETINFO) {
        return this->authenticatorGetInfo();
    } else if (this->command == AuthenticatorAPICommandParam::COMMAND_CLIENTPIN) {
        return this->authenticatorClientPIN();
    } else if (this->command == AuthenticatorAPICommandParam::COMMAND_RESET) {
        return this->authenticatorReset();
    } else if (this->command == AuthenticatorAPICommandParam::COMMAND_GETNEXTASSERTION) {
        return this->authenticatorGetNextAssertion();
    } else if (this->command == AuthenticatorAPICommandParam::COMMAND_VENDORFIRST) {
        return this->authenticatorVendorFirst();
    } else if (this->command == AuthenticatorAPICommandParam::COMMAND_VENDORLAST) {
        return this->authenciatorVendorLast();
    } else { /* Commandが存在しない場合 */
        throw implement_error("This command isn't implemented.");
    }
}

/**
 * @brief authenticatorMakeCredential(0x01)を実行する
 * 
 * @return Response - authenticatorMakeCredentialに対応した返り値
 */
Response *AuthenticatorAPI::authenticatorMakeCredential(ParsedMakeCredentialParams *params) {
    /**
     * authenticatorMakeCredential Response
     */
    Response *response = new Response();

    /**
     * MemberName       Required?
     * -------------    ----------
     * authData:        Required
     * fmt:             Required
     * attStmt:         Required
     */
    CBORPair response_data;

    /* 1.excludeListにパラメータが存在し、認証器内部のデータと一致した場合はエラー */

    /* 2.pubKeyCredParamsにサポートしないCOSEのvalueがあればエラー */
    if (params->data[MakeCredentialParam::KEY_PUBKEY_CRED_PARAM].is_array()) {
        /* TODO:サポートするアルゴリズムの定数化 */
        switch (params->pubKeyCredParams->alg) {
            case -121:
                // Serial.printf("Algorithm %d is Supported by this authenticator.\n", params->pubKeyCredParams->alg);
                break;
            case -50:
                // Serial.printf("Algorithm %d is Supported by this authenticator.\n", params->pubKeyCredParams->alg);
                break;
            default: /* 該当しないalgであればエラーを返す */
                // Serial.println("This Algorithm is not Supported by this authenticator.");
                response->status = StatusCodeParam::CTAP2_ERR_UNSUPPORTED_ALGORITHM;
                return response;
        }
    }

    /* 3.optionのチェック */

    /* 4.extensionsのチェック */

    /* 5.pinAuthパラメータのチェック */

    /* 6.pinAuthがなくclientPinがセットされていればエラー */

    /* 7.pinAuthがありpinProtocolがサポートされていなければエラー */

    /* 8.ユーザのローカル認証を要求 */

    /* 9.アルゴリズムにしたがって鍵ペアを生成する */
    /* 鍵情報を取り出す */

    /* 10.optionsにrkが設定されている場合の処理 */

    /* 11.clientDataHashを使ってAttestation Statementを生成する */
    /* extensionsは一旦省略する */
    /* TODO:暗号化された属性鍵を受け取り、復号して保存する */
    
    /* 固定長バイトの定義 */
    uint8_t *rpIDHash = generateSha256(params->rp->id);
    uint8_t flags[AuthDataSizeParam::AUTHDATA_FLAGS] = {0x85};
    uint8_t counter[AuthDataSizeParam::AUTHDATA_COUNTER] = {0x00, 0x00, 0x00, 0x01};
    uint8_t aaguid[AttestedCredentialDataSizeParam::ATTESTED_AAGUID] = {
        0xF8, 0xA0, 0x11, 0xF3, 0x8C, 0x0A, 0x4D, 
        0x15, 0x80, 0x06, 0x17, 0x11, 0x1F, 0x9E, 0xDC, 0x7D
    };
    uint8_t credentialIDLength[AttestedCredentialDataSizeParam::ATTESTED_LENGTH] = {0x00, 0x40}; /* credentialIDのサイズ */
    size_t apk_size = this->apk->getCBOR().length(); /* 公開鍵のサイズ */

    /* AuthDataのデータサイズ定義 */
    size_t authData_length = AuthDataSizeParam::AUTHDATA_RPIDHASH + AuthDataSizeParam::AUTHDATA_FLAGS
     + AuthDataSizeParam::AUTHDATA_COUNTER + AttestedCredentialDataSizeParam::ATTESTED_AAGUID
     + AttestedCredentialDataSizeParam::ATTESTED_LENGTH + 64
     + apk_size
     ;

    /* AuthDataのデータ定義 */
    uint8_t *authData = new uint8_t[authData_length];
    size_t authData_pointer = 0;
    for (size_t i=0; i<AuthDataSizeParam::AUTHDATA_RPIDHASH; i++) { /* rpIDHashの格納 */
        authData[authData_pointer] = rpIDHash[i];
        authData_pointer++;
    }
    authData[AuthDataSizeParam::AUTHDATA_RPIDHASH] = flags[0]; /* Flagsの格納 */
    authData_pointer++;
    for (size_t i=0; i<AuthDataSizeParam::AUTHDATA_COUNTER; i++) { /* Counterの格納 */
        authData[authData_pointer] = counter[i];
        authData_pointer++;
    }
    for (size_t i=0; i<AttestedCredentialDataSizeParam::ATTESTED_AAGUID; i++) { /* AAGUIDの格納 */
        authData[authData_pointer] = aaguid[i];
        authData_pointer++;
    }
    for (size_t i=0; i<AttestedCredentialDataSizeParam::ATTESTED_LENGTH; i++) { /* lengthの格納 */
        authData[authData_pointer] = credentialIDLength[i];
        authData_pointer++;
    }
    /* 検証項目:ここでいうcredentialIDとはWebAuthnの返り値に含まれるIDと同一なのか？ */
    /* 検証結果:同一であることが判明 -> Arduino内部で生成し返り値データとして送信？ */
    srand((unsigned) time(NULL)); /* TODO:一定のseed値を渡してしまっている？ */
    for (size_t i=0; i<64; i++) {
        /* 乱数値の生成 */
        // uint32_t randomNumber = esp_random();
        uint32_t randomNumber;
        randomNumber = rand();
        authData[authData_pointer] = (uint8_t)(randomNumber);
        authData_pointer++;
    }

    const uint8_t *raw_apk = this->apk->getCBOR().to_CBOR();
    for (size_t i=0; i<apk_size; i++) { /* APKの格納 */
        authData[authData_pointer] = raw_apk[i];
        authData_pointer++;
    }

    /* Attestation Statementのデータ定義 */
    CBORPair attStmt = CBORPair(100);
    attStmt.append("alg", "abs");
    attStmt.append("sig", "no data.");
    attStmt.append("key", "no data.");

    /* CBORデータの作成 */
    CBOR cbor_authData = CBOR();
    cbor_authData.encode(authData, authData_length);
    response_data.append(MakeCredentialResponseParam::KEY_FMT, "packed");
    response_data.append(MakeCredentialResponseParam::KEY_AUTH_DATA, cbor_authData);
    response_data.append(MakeCredentialResponseParam::KEY_ATT_STMT, attStmt);
    
    // CBORエンコードしResponseを作成する
    response->responseData = response_data.to_CBOR();
    response->length = response_data.length();
    // responseSerialDebug(*response, response->length);
    delete params;
    delete authData;
    // Serial.println("MakeCredential command end.");

    return response;
    // throw implement_error("Not implement MakeCredential Content.");
}

/**
 * @brief authenticatorGetAssertion(0x02)を実行する
 * 
 * @return Response - authenticatorGetAssertionに対応した返り値
 */
Response *AuthenticatorAPI::authenticatorGetAssertion(ParsedGetAssertionParams *params) {
    /**
     * authenticatorGetAssertion Response
     */
    Response *response = new Response();

    /**
     * MemberName           Required?
     * -------------        ----------
     * credential           Optional
     * authData:            Required
     * signature:           Required
     * user:                Optional
     * numberOfCredentials  Optional
     */
    CBORPair response_data;

    /* 1.rpIdに結びついたクレデンシャル情報を探し出す(allowListにあるものを優先) */

    /* 2.pinAuthの値チェック */

    /* 3.pinProtocolのサポートチェック */

    /* 4.pinAuthがなくてclietnPinが設定されていればResponseにuvをセットする */

    /* 5.optionsパラメータを実行する */

    /* 6.extensionsがあれば実行する */

    /* 7.正しいユーザの同意を撮る */

    /* 8.1でクレデンシャルがなければエラー */

    /* 9.複数のクレデンシャルがあれば逆順から処理 */

    /* 10.displayがない場合 */
    /* parameterを保管しておく */
    /* counterをインクリメントする */
    /* Timerをスタートする */
    /* User情報を更新する */

    /* 11.displayがある場合 */
    /* ユーザ情報とrpIdを表示する */
    /* 長い時間触れていなければエラー返す */

    /* 12.署名生成 */
    /* authDataの生成 */
    uint8_t *rpIDHash = generateSha256(params->rpId);
    uint8_t flags[AuthDataSizeParam::AUTHDATA_FLAGS] = {0x85};
    uint8_t counter[AuthDataSizeParam::AUTHDATA_COUNTER] = {0x00, 0x00, 0x00, 0x01};
    size_t authData_length = AuthDataSizeParam::AUTHDATA_RPIDHASH
     + AuthDataSizeParam::AUTHDATA_FLAGS + AuthDataSizeParam::AUTHDATA_COUNTER;
    uint8_t *authData = new uint8_t[authData_length];
    size_t authData_pointer = 0;
    for (size_t i=0; i<AuthDataSizeParam::AUTHDATA_RPIDHASH; i++) { /* rpIDHashの格納 */
        authData[authData_pointer] = rpIDHash[i];
        authData_pointer++;
    }
    authData[authData_pointer] = flags[0]; /* flagsの格納 */
    authData_pointer++;
    for (size_t i=0; i<AuthDataSizeParam::AUTHDATA_COUNTER; i++) { /* counterの格納 */
        authData[authData_pointer] = counter[i];
        authData_pointer++;
    }

    /* 署名生成用データの作成 */
    uint8_t *signData = new uint8_t[params->cbor_clientDataHash.get_bytestring_len()+authData_length];
    memcpy(signData, authData, authData_length); /* authDataのコピー */
    memcpy(signData+authData_length, params->hash, params->cbor_clientDataHash.get_bytestring_len()); /* clientDataHashのコピー */

    /* 乱数生成機 */
    char raw[100];
    csprng RNG;
    octet RAW = {0, sizeof(raw), raw};
    RAW.len = 100;
    srand(time(NULL));
    for (int i=0; i<100; i++) RAW.val[i] = rand();
    CREATE_CSPRNG(&RNG, &RAW);

    /* 署名生成 */
    Signature *signature = new Signature;
    SignatureParams sigParams = {
        this->tpk, /* Trustee Public Key */
        this->apk, /* Public Key */
        this->ska, /* User Secret Key */
        signData,  /* signData */
        authData_length+params->cbor_clientDataHash.get_bytestring_len(), /* signData length */
        params->policy, /* ABS Policy */
        RNG, /* random generator */
        signature, /* signature */
    };
    generateSign(&sigParams);

    /* 署名データのバイト変換 */
    CBORPair cbor_signature = CBORPair(100);
    setECPSignature(&cbor_signature, signature->getY(), "Y");
    setECPSignature(&cbor_signature, signature->getW(), "W");
    int count=1;
    for (auto& e : signature->getS()) { /* Sの値を格納 */
        setECPSignature(&cbor_signature, &e, "S"+String(count));
        count++;
    }
    count = 1;
    for (auto& e : signature->getP()) { /* Pの値を格納 */
        setECP2Signature(&cbor_signature, &e, "P"+String(count));
        count++;
    }

    /* CBORデータの生成 */
    CBOR cbor_authData = CBOR();
    cbor_authData.encode(authData, authData_length);
    response_data.append(GetAssertionResponseParam::KEY_AUTH_DATA, cbor_authData);
    response_data.append(GetAssertionResponseParam::KEY_SIGNATURE, cbor_signature);

    response->responseData = response_data.to_CBOR();
    response->length = response_data.length();
    delete params;
    delete authData;
    // responseSerialDebug(*response, response->length);

    return response;
    // throw implement_error("Not implement GetAssertion Content.");
}

/**
 * @brief authenticatorGetInfo(0x04)を実行する
 * 
 * @return Response - authenticatorGetInfoに対応した返り値
 */
Response *AuthenticatorAPI::authenticatorGetInfo() {
    /**
     * authenticatorGetInfo Response
     */
    Response *response = new Response();

    /**
     * MemberName       Required?
     * -------------    ----------
     * versions:        Required
     * extensions:      Optional 
     * aaguid:          Required
     * options:         Optional
     * maxMsgSize:      Optional
     * pinProtocols:    Optional
     */
    CBORPair response_data = CBORPair(100);

    /**
     *  MemberName: versions
     *  DataType: Sequence of String
     *  Required?: Required
     */
    CBORArray cbor_versions = CBORArray(20);
    const char *versions[2] = {"CTAP_ABS", "FIDO_ABS"};
    cbor_versions.append(versions, 2);
    response_data.append(GetInfoResponseParam::KEY_VERSIONS, cbor_versions);

    /**
     * MemberName: extensions
     * DataType: Sequence of String
     * Required?: Optional
     */
    CBORArray cbor_extensions = CBORArray(20);
    const char *extensions[1] = {"hmac-secret"};
    cbor_extensions.append(extensions, 1);
    response_data.append(GetInfoResponseParam::KEY_EXTENSIONS, cbor_extensions);

    /**
     * MemberName: aaguid
     * DataType: Byte String
     * Required?: Required
     */
    const uint8_t aaguid[] = {
        0xF8, 0xA0, 0x11, 0xF3, 0x8C, 0x0A, 0x4D, 
        0x15, 0x80, 0x06, 0x17, 0x11, 0x1F, 0x9E, 0xDC, 0x7D
    };
    CBOR cbor_aaguid = CBOR();
    cbor_aaguid.encode(aaguid, 16);
    response_data.append(GetInfoResponseParam::KEY_AAGUID, cbor_aaguid);


    /**
     * MemberName: options
     * DataType: Map
     * Required?: Optional
     */
    CBORPair cbor_options = CBORPair(100);
    cbor_options.append("plat", false);
    cbor_options.append("rk", false);
    cbor_options.append("clientPin", false);
    cbor_options.append("up", false);
    cbor_options.append("uv", false);
    response_data.append(GetInfoResponseParam::KEY_OPTIONS, cbor_options);


    /**
     * MemberName: maxMsgSize
     * DataType: Unsigned Integer
     * Required?: Optional
     */
    unsigned int maxMsgSize = 1200;
    response_data.append(GetInfoResponseParam::KEY_MAX_MSG_SIZE, maxMsgSize);


    /**
     * MemberName: pinProtocols
     * DataType: Array Of Unsigned Integers
     * Required?: Optional
     */
    CBORArray cbor_pinProtocols = CBORArray(20);
    const unsigned int pinProtocols[1] = {0x01};
    cbor_pinProtocols.append(pinProtocols, 1);
    response_data.append(GetInfoResponseParam::KEY_PIN_PROTOCOLS, cbor_pinProtocols);


    response->responseData = response_data.to_CBOR();
    
    response->length = response_data.length();

    // Serial.println("GetInfo command End.");

    return response;

    // throw implement_error("Not implement GetInfo Content.");
}

/**
 * @brief authenticatorClientPIN(0x06)を実行する
 * 
 * @return Response - authenticatorClientPINに対応した返り値
 */
Response *AuthenticatorAPI::authenticatorClientPIN() {
    throw implement_error("Not implement ClientPIN Content.");
}

/**
 * @brief authenticatorReset(0x07)を実行する
 * 
 * @return Reponse - authenticatorResetに対応した返り値
 */
Response *AuthenticatorAPI::authenticatorReset() {
    throw implement_error("Not implement Reset Content.");
}

/**
 * @brief authenticatorGetNextAssertion(0x08)を実行する
 * 
 * @return Response - authenticatorGetNextAssertionに対応した返り値
 */
Response *AuthenticatorAPI::authenticatorGetNextAssertion() {
    throw implement_error("Not implement GetNextAssertion Content.");
}

/**
 * @brief authenticatorVendorFirst(0x40)を実行する
 * 
 * @return Response - authenticatorVendorFirstに対応した返り値
 */
Response *AuthenticatorAPI::authenticatorVendorFirst() {
    throw implement_error("Not implement VendorFirst Content.");
}

/**
 * @brief authenticatorVendorLast(0xbf)を実行する
 * 
 * @return Response - authenticatorVendorLastに対応した返り値
 */
Response *AuthenticatorAPI::authenciatorVendorLast() {
    throw implement_error("Not implement VendorLast Content.");
}

/**
 * @brief getter - command
 * 
 * @return unsigned int - AuthenticatorAPIのコマンド識別子
 */
unsigned int AuthenticatorAPI::getCommand() {
    return this->command;
}

/**
 * @brief getter - parameter
 * 
 * @return uint8_t* - AuthenticatorAPIのパラメータ
 */
uint8_t *AuthenticatorAPI::getParameter() {
    return this->parameter;
}

/**
 * @brief getter - length
 * 
 * @return unsigned int - AuthenticatorAPIのパラメータの長さ
 */
unsigned int AuthenticatorAPI::getLength() {
    return this->length;
}

/**
 * @brief getter
 * 
 * @return TPK* Trustee Public Key
 */
TPK *AuthenticatorAPI::getTPK() {
    return this->tpk;
}

/**
 * @brief getter
 * 
 * @return APK* Public Key
 */
APK *AuthenticatorAPI::getAPK() {
    return this->apk;
}

/**
 * @brief getter
 * 
 * @return SKA* User Secret Key
 */
SKA *AuthenticatorAPI::getSKA() {
    return this->ska;
}

/**
 * @brief setter - command
 * 
 * @param command 
 */
void AuthenticatorAPI::setCommand(unsigned int command) {
    this->command = command;
}

/**
 * @brief setter - parameter
 * 
 * @param parameter 
 */
void AuthenticatorAPI::setParameter(uint8_t *parameter) {
    this->parameter = parameter;
}

/**
 * @brief setter - length
 * 
 * @param length parameterの長さ
 */
void AuthenticatorAPI::setLength(unsigned int length) {
    this->length = length;
}

/**
 * @brief setter
 * 
 * @param tpk Trustee Public Key
 */
void AuthenticatorAPI::setTPK(TPK *tpk) {
    this->tpk = tpk;
}

/**
 * @brief setter
 * 
 * @param apk Public Key
 */
void AuthenticatorAPI::setAPK(APK *apk) {
    this->apk = apk;
}

/**
 * @brief setter
 * 
 * @param ska User Secret Key
 */
void AuthenticatorAPI::setSKA(SKA *ska) {
    this->ska = ska;
}

/**
 * @brief パラメータを必要とするコマンドを確認する
 * 
 * @param command AuthenticatorAPIのコマンド識別子
 * @return コマンドが必要な値であればtrue
 *         必要がない値であればfalse
 */
bool checkHasParameters(unsigned int command) {
    if (command == AuthenticatorAPICommandParam::COMMAND_MAKECREDENTIAL ||
    command == AuthenticatorAPICommandParam::COMMAND_GETASSERTION ||
    command == AuthenticatorAPICommandParam::COMMAND_CLIENTPIN) {
        return true;
    } else {
        return false;
    }
}

/**
 * @brief SHA256を生成する
 * 
 * @param data sha256エンコードする対象のデータ
 * @return uint8_t* Sha256エンコードした値
 */
uint8_t *generateSha256(String data) {
    Sha256.init();
    Sha256.print(data);
    uint8_t * result = Sha256.result();
    return result;
}