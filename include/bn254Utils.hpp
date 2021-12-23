#ifndef INCLUDED_bn254Utils_h_
#define INCLUDED_bn254Utils_h_

#include "pair_BN254.h"
#include "randapi.h"
#include <string.h>
// #include <M5StickC.h>
#include "YACL.h"
#include "MsgPack.h"

using namespace core;
using namespace B256_28;
using namespace BN254;

struct AttrNode {
    /**
     * @brief 属性名
     */
    String attribute;

    /**
     * @brief 左右のノード情報
     */
    AttrNode *left, *right;

    AttrNode();

    AttrNode(String value);

    ~AttrNode();

    void printNode();
};

struct AttributeOperatorType {
    static const String OPERATOR_OR;
    static const String OPERATOR_AND;
};

void getMSP(MsgPack::arr_t<MsgPack::arr_t<int>> *msp, String policy, String *attributes);
void recursivefill(AttrNode *node, MsgPack::arr_t<MsgPack::arr_t<int>> *msp, int vector, MsgPack::map_t<String, int> matrix);
String remove_space(String exp);
int get_pos_operator(String exp);
int parse_expression(AttrNode *node);

/* -----Utility------ */
void outputOCT(octet *o);
void outputBIG(BIG *a);
void outputECP(ECP *g);
void outputECP2(ECP2 *h);
ECP getG1Element(csprng RNG);
ECP2 getG2Element(csprng RNG);
void parseECPElement(ECP *elem, CBOR cbor, String name);
ECP2 *parseECP2Element(CBOR cbor, String name);
/* TODO:以下の形式に変更 */
// void parseECP2Element(ECP2 *elem, CBOR cbor, String name);
uint8_t *ECPtoBuffer(ECP *g);
void setECPSignature(CBORPair *pair, ECP *g, String key);
void setECP2Signature(CBORPair *pair, ECP2 *h, String key);
void createHash(char *msg, size_t length, BIG *mu);
void convertInt(int32_t *big_number, int int_number);
unsigned long start();
void stop(unsigned long start_time);
#endif