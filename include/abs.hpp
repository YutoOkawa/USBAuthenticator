#ifndef INCLUDED_abs_h_
#define INCLUDED_abs_h_

#include "bn254Utils.hpp"

class TPK {
    private:
        ECP g;
        MsgPack::arr_t<ECP2> h;
        MsgPack::map_t<String, int> attriblist;
        CBOR cbor_tpk;

    public:
        TPK();
        TPK(CBOR cbor_tpk);
        ~TPK();

        void parse();

        ECP *getG();
        MsgPack::arr_t<ECP2> getH();
        MsgPack::map_t<String, int> getAttriblist();
        CBOR getCBOR();
        void setG(ECP g);
        void setH(MsgPack::arr_t<ECP2> h);
        void setAttriblist(MsgPack::map_t<String, int> attriblist);
        void setCBOR(CBOR cbor_tpk);
};

class APK {
    private:
        ECP2 A0;
        MsgPack::arr_t<ECP2> A;
        MsgPack::arr_t<ECP2> B;
        ECP C;
        CBOR cbor_apk;

    public:
        APK();
        APK(CBOR cbor_apk);
        ~APK();

        void parse();
        
        ECP2 *getA0();
        MsgPack::arr_t<ECP2> getA();
        MsgPack::arr_t<ECP2> getB();
        ECP *getC();
        CBOR getCBOR();
        void setA0(ECP2 A0);
        void setA(MsgPack::arr_t<ECP2> A);
        void setB(MsgPack::arr_t<ECP2> B);
        void setC(ECP C);
        void setCBOR(CBOR cbor_apk);
};

class SKA {
    private:
        ECP KBase;
        ECP K0;
        MsgPack::map_t<String, ECP> K;
        CBOR cbor_ska;

    public:
        SKA();
        SKA(CBOR cbor_ska);
        ~SKA();

        void parse();

        ECP *getKBase();
        ECP *getK0();
        MsgPack::map_t<String, ECP> getK();
        CBOR getCBOR();
        void setKBase(ECP KBase);
        void setK0(ECP K0);
        void setK(MsgPack::map_t<String, ECP> K);
        void setCBOR(CBOR cbor_ska);
};

class Signature {
    private:
        ECP Y;
        ECP W;
        MsgPack::arr_t<ECP> S;
        MsgPack::arr_t<ECP2> P;

    public:
        Signature();

        ECP *getY();
        ECP *getW();
        MsgPack::arr_t<ECP> getS();
        MsgPack::arr_t<ECP2> getP();
        void setY(ECP Y);
        void setW(ECP W);
        void setS(ECP S);
        void setP(ECP2 P);
};

struct SignatureParams {
    /**
     * @var tpk
     * @brief Trustee Public Key
     */
    TPK *tpk;

    /**
     * @var apk
     * @brief public Key
     */
    APK *apk;

    /**
     * @var ska
     * @brief user secret key
     */
    SKA *ska;

    /**
     * @var signData
     * @brief signature data.
     */
    uint8_t *signData;

    /**
     * @var signDataLength;
     * @brief signatureData Length.
     */
    size_t signDataLength;

    /**
     * @var policy
     * @brief ABS Policy.
     */
    String policy;

    /**
     * @var RNG
     * @brief random generator.
     */
    csprng RNG;

    /**
     * @var signature
     * @brief signature data.
     */
    Signature *signature;

    /**
     * @var xBinarySemaphore
     * @brief Binary Semaphore
     */
    // SemaphoreHandle_t *xBinarySemaphore;
};

void generateSign(void *pvParameters);
#endif