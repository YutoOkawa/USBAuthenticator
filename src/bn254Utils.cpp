#include "bn254Utils.hpp"

/**
 * @brief G1の要素をランダムに返す
 * 
 * @param RNG 乱数値
 * @return ECP ランダムなG1の要素
 */
ECP getG1Element(csprng RNG) {
    FP fp;
    ECP G1;

    FP_rand(&fp, &RNG);
    ECP_map2point(&G1, &fp);
    return G1;
}

/**
 * @brief G2の要素をランダムに返す
 * 
 * @param RNG 乱数値
 * @return ECP2 ランダムなG2の要素
 */
ECP2 getG2Element(csprng RNG) {
    FP2 fp2;
    ECP2 G2;

    FP2_rand(&fp2, &RNG);
    ECP2_map2point(&G2, &fp2);
    return G2;
}

/**
 * @brief CBORデータからECPデータを抽出する
 * 
 * @param cbor CBORデータ
 * @param key ECPデータのポインタ
 */
void parseECPElement(ECP *elem, CBOR cbor, String name) {
    // ECP *key = new ECP();
    uint8_t *keyBytes_hex = new uint8_t[cbor[name.c_str()].get_bytestring_len()];
    char *keyBytes = new char[cbor[name.c_str()].get_bytestring_len()];
    octet KEYBYTES = {0, cbor[name.c_str()].get_bytestring_len(), keyBytes};

    cbor[name.c_str()].get_bytestring(keyBytes_hex); /* cbor->uint8_t */
    for (int i=0; i<cbor[name.c_str()].get_bytestring_len(); i++) { /* uint8_t->char */
        keyBytes[i] = keyBytes_hex[i];
    }
    OCT_jstring(&KEYBYTES, keyBytes); /* char->OCT */
    ECP_fromOctet(elem, &KEYBYTES); /* OCT->ECP */
    if (!PAIR_G1member(elem)) { /* ECPの内容がInfinityの場合 */
        // Serial.printf("%s(ECP):No Member.\n", name.c_str());
        // Serial.print("OCT:");
        OCT_output(&KEYBYTES);
        // Serial.printf("ECP:");
        ECP_output(elem);
    }
    delete keyBytes_hex;
    delete keyBytes;
}

/**
 * @brief CBORデータからECP2データを抽出する
 * 
 * @param cbor CBORデータ
 * @param key ECP2のデータのポインタ
 */
ECP2 *parseECP2Element(CBOR cbor, String name) {
    ECP2 *key = new ECP2();
    uint8_t *keyBytes_hex = new uint8_t[cbor[name.c_str()].get_bytestring_len()];
    char *keyBytes = new char[cbor[name.c_str()].get_bytestring_len()];
    octet KEYBYTES = {0, cbor[name.c_str()].get_bytestring_len(), keyBytes};

    cbor[name.c_str()].get_bytestring(keyBytes_hex); /* cbor->uint8_t */
    for (int i=0; i<cbor[name.c_str()].get_bytestring_len(); i++) { /* uint8_t->char */
        keyBytes[i] = keyBytes_hex[i];
    }
    OCT_jstring(&KEYBYTES, keyBytes); /* char->OCT */
    ECP2_fromOctet(key, &KEYBYTES); /* OCT->ECP2 */
    /* TODO:メモリ不足のためタスクを割り振り値をチェック(8192bytes以上必要) */
    // if (!PAIR_G2member(key)) { /* ECP2の内容がInfinityの場合 */
    //     Serial.printf("%s(ECP2):No Member.\n", name.c_str());
    //     Serial.print("OCT:");
    //     OCT_output(&KEYBYTES);
    //     Serial.printf("ECP:");
    //     ECP2_output(key);
    // }
    delete keyBytes_hex;
    delete keyBytes;
    return key;
}

/**
 * @brief ECPデータをバイトデータ(uint8_t)に変換する
 * 
 * @param g ECPデータ
 * @return uint8_t* ECPのバイトデータ
 */
uint8_t *ECPtoBuffer(ECP *g) {
    uint8_t *ecp = new uint8_t[MODBYTES_B256_28+1];
    char buf[MODBYTES_B256_28+1];
    octet BUF = {0, sizeof(buf), buf};
    ECP_toOctet(&BUF, g, true);
    memcpy(ecp, buf, MODBYTES_B256_28+1);
    return ecp;
}

uint8_t *ECP2toBuffer(ECP2 *h) {
    uint8_t *ecp2 = new uint8_t[2*MODBYTES_B256_28+1];
    char buf[2*(MODBYTES_B256_28+1)+1];
    octet BUF = {0, sizeof(buf), buf};
    ECP2_toOctet(&BUF, h, true);
    memcpy(ecp2, buf, 2*(MODBYTES_B256_28+1)+1);
    return ecp2;
}

/**
 * @brief CBORPairに署名情報を追加する
 * 
 * @param pair CBORPairのポインタ
 * @param g ECPデータ
 * @param key キーの名前
 */
void setECPSignature(CBORPair *pair, ECP *g, String key) {
    uint8_t *buf = ECPtoBuffer(g);
    CBOR G = CBOR();
    G.encode(buf, MODBYTES_B256_28+1);
    pair->append(key.c_str(), G);
    delete buf;
}

void setECP2Signature(CBORPair *pair, ECP2 *h, String key) {
    uint8_t *buf = ECP2toBuffer(h);
    CBOR H = CBOR();
    H.encode(buf, 2*(MODBYTES_B256_28)+1);
    pair->append(key.c_str(), H);
}

/**
 * @brief Create a Hash object
 * 
 * @param msg メッセージ
 * @return BIG* ハッシュ化されたメッセージ
 */
void createHash(char *msg, size_t length, BIG *mu) {
    hash512 sh512;
    int i;
    char digest[64];

    HASH512_init(&sh512);
    for(i=0; i<length; i++) {
        HASH512_process(&sh512, msg[i]);
    }
    HASH512_hash(&sh512, digest);
    BIG_fromBytes(*mu, digest);
}

/**
 * @brief Int型をBIG型に変換する
 * 
 * @param number 数字
 * @return BIG* BIG型の数字表現
 */
int32_t *convertInt(int32_t *big_number, int int_number) {
    BIG one;

    BIG_one(one);
    BIG_zero(big_number); //初期値は0値

    for(int i=0; i<int_number; i++) {
        BIG_add(big_number, big_number, one);
    }
}

unsigned long start() {
    unsigned long start_time = micros();
    return start_time;
}

void stop(unsigned long start_time) {
    unsigned long end_time = micros();
    // Serial.printf("%lu milli seconds\n", (end_time - start_time) / 1000);
    // Serial.printf("%lu milli seconds\n", (end_time - start_time));
}