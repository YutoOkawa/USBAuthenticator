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

/* -----Utility------ */
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
int32_t *convertInt(int32_t *big_number, int int_number);
unsigned long start();
void stop(unsigned long start_time);
#endif