#include "abs.hpp"

void test() {
    // // ECP->OCT->ECPの転写テスト
    ECP test;
    ECP_generator(&test);
    // Serial.print("ECP generator:");
    ECP_output(&test);
    char char_test[MODBYTES_B256_28+1];
    octet octet_test = {0, sizeof(char_test), char_test};
    ECP_toOctet(&octet_test, &test, true);
    // Serial.print("ECP generator OCT:");
    OCT_output(&octet_test);
    ECP test1;
    ECP_fromOctet(&test1, &octet_test);
    // Serial.print("ECP generator OCT→ECP");
    ECP_output(&test1);
}

/* ----------------------TPK---------------------- */
TPK::TPK() {

}

/**
 * @brief Construct a new TPK::TPK object
 * 
 * @param cbor_tpk tpkのCBOR形式
 */
TPK::TPK(CBOR cbor_tpk) {
    this->cbor_tpk = cbor_tpk;
}

/**
 * @brief Destroy the TPK::TPK object
 */
TPK::~TPK() {

}

/**
 * @brief cborデータから鍵データをパースする
 */
void TPK::parse() {
    parseECPElement(&this->g, this->cbor_tpk, "g");
    for (int i=0; i<this->cbor_tpk.n_elements()-2; i++) {
        String key = "h" + String(i);
        this->h.push_back(*parseECP2Element(this->cbor_tpk, key.c_str()));
    }
}

/**
 * @brief tpkを出力する
 */
void TPK::SerialDebug() {
    Serial.print("tpk->g:");
    outputECP(&this->g);

    int count = 1;
    for (auto& e : this->h) {
        Serial.print("tpk->h"+String(count)+":");
        outputECP2(&e);
        count++;
    }
}

/**
 * @brief getter
 * 
 * @return ECP Gを返す
 */
ECP *TPK::getG() {
    return &this->g;
}

/**
 * @brief getter
 * 
 * @return MsgPack::arr_t<ECP2>  hを返す
 */
MsgPack::arr_t<ECP2> TPK::getH() {
    return this->h;
}

/**
 * @brief getter
 * 
 * @return MsgPack::map_t<String, int> 属性集合を返す
 */
MsgPack::map_t<String, int> TPK::getAttriblist() {
    return this->attriblist;
}

/**
 * @brief getter
 * 
 * @return CBOR cbor_tpk
 */
CBOR TPK::getCBOR() {
    return this->cbor_tpk;
}

/**
 * @brief setter
 * 
 * @param g Gをセットする
 */
void TPK::setG(ECP g) {
    this->g = g;
}

/**
 * @brief setter
 * 
 * @param h hをセットする
 */
void TPK::setH(MsgPack::arr_t<ECP2> h) {
    this->h = h;
}

/**
 * @brief setter
 * 
 * @param attriblist 属性集合をセットする 
 */
void TPK::setAttriblist(MsgPack::map_t<String, int> attriblist) {
    this->attriblist = attriblist;
}

/**
 * @brief setter
 * 
 * @param cbor_tpk cbor_tpk 
 */
void TPK::setCBOR(CBOR cbor_tpk) {
    this->cbor_tpk = cbor_tpk;
}

/* ----------------------APK---------------------- */
APK::APK() {

}

/**
 * @brief Construct a new APK::APK object
 * 
 * @param cbor_apk apkのCBOR形式
 */
APK::APK(CBOR cbor_apk) {
    this->cbor_apk = cbor_apk;
}

/**
 * @brief Destroy the APK::APK object
 */
APK::~APK() {
    
}

/**
 * @brief cborデータから鍵データをパースする
 */
void APK::parse() {
    this->A0 = *parseECP2Element(this->cbor_apk, "A0");
    for (int i=1; i<=(this->cbor_apk.n_elements()-3)/2; i++) {
        String Akey = "A" + String(i);
        this->A.push_back(*parseECP2Element(this->cbor_apk, Akey.c_str()));
        String Bkey = "B" + String(i);
        this->B.push_back(*parseECP2Element(this->cbor_apk, Bkey.c_str()));
    }
    parseECPElement(&this->C, this->cbor_apk, "C");
}

/**
 * @brief apkを出力する
 */
void APK::SerialDebug() {
    Serial.print("apk->A0:");
    outputECP2(&this->A0);

    int count = 1;
    for (auto& e : this->A) {
        Serial.print("apk->A"+String(count)+":");
        outputECP2(&e);
        count++;
    }

    count = 1;
    for (auto& e : this->B) {
        Serial.print("apk->B"+String(count)+":");
        outputECP2(&e);
        count++;
    }

    Serial.print("apk->C:");
    outputECP(&this->C);
}

/**
 * @brief getter
 * 
 * @return ECP2 A0を返す
 */
ECP2 *APK::getA0() {
    return &this->A0;
}

/**
 * @brief getter
 * 
 * @return MsgPack::arr_t<ECP2> Aを返す
 */
MsgPack::arr_t<ECP2> APK::getA() {
    return this->A;
}

/**
 * @brief getter
 * 
 * @return MsgPack::arr_t<ECP2> Bを返す
 */
MsgPack::arr_t<ECP2> APK::getB() {
    return this->B;
}

/**
 * @brief getter
 * 
 * @return ECP Cを返す
 */
ECP *APK::getC() {
    return &this->C;
}

/**
 * @brief getter
 * 
 * @return CBOR cbor_apk 
 */
CBOR APK::getCBOR() {
    return this->cbor_apk;
}

/**
 * @brief setter
 * 
 * @param A0 A0をセットする
 */
void APK::setA0(ECP2 A0) {
    this->A0 = A0;
}

/**
 * @brief setter
 * 
 * @param A Aをセットする
 */
void APK::setA(MsgPack::arr_t<ECP2> A) {
    this->A = A;
}

/**
 * @brief setter
 * 
 * @param B Bをセットする
 */
void APK::setB(MsgPack::arr_t<ECP2> B) {
    this->B = B;
}

/**
 * @brief setter
 * 
 * @param C Cをセットする
 */
void APK::setC(ECP C) {
    this->C = C;
}

/**
 * @brief setter
 * 
 * @param cbor_apk cbor_apkをセットする 
 */
void APK::setCBOR(CBOR cbor_apk) {
    this->cbor_apk = cbor_apk;
}

/* ----------------------SKA---------------------- */
SKA::SKA() {

}

/**
 * @brief Construct a new SKA::SKA object
 * 
 * @param cbor_ska skaのCBOR形式
 */
SKA::SKA(CBOR cbor_ska) {
    this->cbor_ska = cbor_ska;
}

/**
 * @brief Destroy the SKA::SKA object
 */
SKA::~SKA() {
    
}

/**
 * @brief CBORデータから鍵データをパースする
 * 
 */
void SKA::parse() {
    String keylist[this->cbor_ska.n_elements()];
    for (int i=0; i<this->cbor_ska.n_elements(); i++) {
        this->cbor_ska.key_at(i).get_string(keylist[i]);
        // Serial.printf("Key:%s\n", keylist[i].c_str());
    }
    /* 固定Key値のパース */
    parseECPElement(&this->KBase, this->cbor_ska, "KBase");
    parseECPElement(&this->K0, this->cbor_ska, "K0");

    /* 可変Key値のパース */
    for (int i=2; i<this->cbor_ska.n_elements(); i++) {
        parseECPElement(&this->K[keylist[i]], this->cbor_ska, keylist[i]);
    }
}

/**
 * @brief skaを出力する
 */
void SKA::SerialDebug() {
    Serial.print("ska->KBase:");
    outputECP(&this->KBase);

    Serial.print("ska->K0:");
    outputECP(&this->K0);

    for (int i=0; i<this->K.size(); i++) {
        String key = "K" + String(i+2);
        if (this->K.count(key) != 0) {
            Serial.print("ska->"+key+":");
            outputECP(&this->K[key]);
        }
    }
}

/**
 * @brief getter
 * 
 * @return ECP* KBaseを返す
 */
ECP *SKA::getKBase() {
    return &this->KBase;
}

/**
 * @brief getter
 * 
 * @return ECP* K0を返す
 */
ECP *SKA::getK0() {
    return &this->K0;
}

/**
 * @brief getter
 * 
 * @return MsgPack::map_t<String, ECP> Kを返す
 */
MsgPack::map_t<String, ECP> SKA::getK() {
    return this->K;
}

/**
 * @brief getter
 * 
 * @return CBOR cbor_ska
 */
CBOR SKA::getCBOR() {
    return this->cbor_ska;
}

/**
 * @brief setter
 * 
 * @param KBase KBaseをセットする
 */
void SKA::setKBase(ECP KBase) {
    this->KBase = KBase;
}

/**
 * @brief setter
 * 
 * @param K0 K0をセットする
 */
void SKA::setK0(ECP K0) {
    this->K0 = K0;
}

/**
 * @brief setter
 * 
 * @param cbor_ska cbor_skaをセットする 
 */
void SKA::setCBOR(CBOR cbor_ska) {
    this->cbor_ska = cbor_ska;
}

/* ----------------------Signature--------------------- */

/**
 * @brief 署名を生成する
 * 
 * @param tpk Trustee Public Key
 * @param apk Public Key
 * @param ska ユーザ秘密鍵
 * @param message 署名対象のメッセージ
 * @param policy ポリシー
 * @param RNG 乱数値
 * @return Signature 署名情報
 */
// TODO:署名生成方法をコア任せにしない変更
void generateSign(SignatureParams *sigParams) {
    String attributes[] = {"USER", "PARENTS", "GUARDIANSHIP", "A"};
    BIG rd;

    unsigned long start_time = start();

    /* mspの生成 */
    /* msp本体は配列の1番目から */
    MsgPack::arr_t<MsgPack::arr_t<int>> msp(sizeof(attributes)/sizeof(attributes[0]), MsgPack::arr_t<int>(1));
    getMSP(&msp, sigParams->policy, attributes);

    /* 位数の設定 */
    BIG_rcopy(rd, CURVE_Order);
    // outputBIG(rd);

    /* 署名対象データの生成 */
    size_t length = sigParams->signDataLength + sigParams->policy.length();
    char *sign_data = new char[length];
    memcpy(sign_data, sigParams->signData, length);
    memcpy(sign_data+sigParams->signDataLength, sigParams->policy.c_str(), sigParams->policy.length());

    /* ハッシュ値生成 */
    BIG mu;
    createHash(sign_data, length, &mu);
    BIG_mod(mu, rd);

    /* r_{0}をランダムに選ぶ */
    BIG r0;
    BIG_randtrunc(r0, rd, 2 * CURVE_SECURITY_BN254, &sigParams->RNG);

    int32_t *rlist[msp.size()];
    BIG r;
    for (size_t i=0; i<msp.size(); i++) {
        // BIG r;
        rlist[i] = new int32_t;
        BIG_randtrunc(r, rd, 2* CURVE_SECURITY_BN254, &sigParams->RNG);
        rlist[i] = r;
    }

    /* 署名値の計算 */
    Serial.println("Signature:Y");
    ECP Y; // Y = r[0] * KBase
    ECP_copy(&Y, sigParams->ska->getKBase());
    PAIR_G1mul(&Y, r0);
    sigParams->signature->setY(Y);

    Serial.println("Signature:W");
    ECP W; // W = r[0] * K0
    ECP_copy(&W, sigParams->ska->getK0());
    PAIR_G1mul(&W, r0);
    sigParams->signature->setW(W);

    // S_{i} = (K_{u(i)}^ui)^r0 * (Cg^μ)^r_{i}
    Serial.println("Signature:S");
    for (size_t i=0; i<msp.size(); i++) {
        ECP Si; // multi = r_{i} * (C + μg)
        ECP_copy(&Si, sigParams->tpk->getG());
        PAIR_G1mul(&Si, mu);
        ECP_add(&Si, sigParams->apk->getC());
        PAIR_G1mul(&Si, rlist[i]);

        // ユーザ秘密鍵の検索 - 存在している場合のみ処理を行う
        String key = "K" + String(i+2);
        if (sigParams->ska->getK().count(key) != 0) { /* キー値が存在している場合 */
            ECP rK; // K_{u(i)}^r0
            ECP_copy(&rK, &sigParams->ska->getK()[key]);
            PAIR_G1mul(&rK, r0);
            ECP_add(&Si, &rK);
        }
        sigParams->signature->setS(Si);
    }

    // P_{j} = \prod i=1~l (Aj+Bj^u(i))^Mij*ri
    Serial.println("Signature:P");
    for (size_t j=1; j<msp.at(0).size()+1; j++) {
        ECP2 Pj;
        for (size_t i=1; i<msp.size()+1; i++) {
            ECP2 base;
            BIG ui;
            BIG mij;
            BIG exp; // Mji*ri
            ECP2_copy(&base, &sigParams->apk->getB().at(j-1)); // base<-Bj
            convertInt(ui, i+1); // ui<-i
            PAIR_G2mul(&base, ui); // Bj^u(i)
            ECP2_add(&base, &sigParams->apk->getA().at(j-1)); // Aj+Bj^u(i)
            convertInt(mij, msp.at(i-1).at(j)); // exp<-Mij(j=0はダミーデータ)
            BIG_modmul(exp, mij, rlist[i-1], rd); // exp<-Mji*ri
            PAIR_G2mul(&base, exp); // (Aj+Bj^u(i))^Mij*ri
            if (i==1) {
                ECP2_copy(&Pj, &base);
            } else if (!ECP2_isinf(&base)) {
                ECP2_add(&Pj, &base);
            } else {
                
            }
        }
        sigParams->signature->setP(Pj);
    }
    stop(start_time);
}

Signature ::Signature() {

}

ECP *Signature::getY() {
    return &this->Y;
}

ECP *Signature::getW() {
    return &this->W;
}

MsgPack::arr_t<ECP> Signature::getS() {
    return this->S;
}

MsgPack::arr_t<ECP2> Signature::getP() {
    return this->P;
}

void Signature::setY(ECP Y) {
    this->Y = Y;
}

void Signature::setW(ECP W) {
    this->W = W;
}

void Signature::setS(ECP S) {
    this->S.push_back(S);
}

void Signature::setP(ECP2 P) {
    this->P.push_back(P);
}