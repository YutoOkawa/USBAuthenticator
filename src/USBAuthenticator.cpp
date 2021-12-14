#include "USBAuthenticator.h"
#include "usb_phy_api.h"
#include "stdint.h"

using namespace arduino;

/* ----------------------CTAPInterfaceDescriptorParam---------------------- */
/** @brief One IN and one OUT endpoint */
const int CTAPInterfaceDescriptorParam::NUM_ENDPOINTS = 0x02;

/** @brief No Interface subclass */
const int CTAPInterfaceDescriptorParam::INTERFACE_SUBCLASS = 0x00;

/** @brief No Interface protocol */
const int CTAPInterfaceDescriptorParam::INTERFACE_PROTOCOL = 0x00;

/* ----------------------CTAPEndpointOneDescriptorParam---------------------- */
/** @brief Interrupt transfer */
const int CTAPEndpointOneDescriptorParam::M_ATTRIBUTES = 0x03;

/** @brief 1, OUT */
const int CTAPEndpointOneDescriptorParam::ENDPOINT_ADDRESS = 0x01;

/** @brief 64-byte packet max */
const int CTAPEndpointOneDescriptorParam::MAX_PACKET_SIZE = 64;

/** @brief Poll every 5 millisecond */
const int CTAPEndpointOneDescriptorParam::INTERVAL = 5;

/* ----------------------CTAPEndpointTwoDescriptorParam---------------------- */
/** @brief Interrupt transfer */
const int CTAPEndpointTwoDescriptorParam::M_ATTRIBUTES = 0x03;

/** @brief 1, IN */
const int CTAPEndpointTwoDescriptorParam::ENDPOINT_ADDRESS = 0x81;

/** @brief 64-byte packet max */
const int CTAPEndpointTwoDescriptorParam::MAX_PACKET_SIZE = 64;

/** @brief Poll every 5 millisecond */
const int CTAPEndpointTwoDescriptorParam::INTERVAL = 5;


/* ----------------------USBAuthenticator---------------------- */
USBAuthenticator::USBAuthenticator(bool connect, uint16_t vendor_id, uint16_t product_id, uint16_t product_release):
    USBHID(get_usb_phy(), 0, 0, vendor_id, product_id, product_release)
{
    _lock_status = 0;
}

USBAuthenticator::USBAuthenticator(USBPhy *phy, uint16_t vendor_id, uint16_t product_id, uint16_t product_release):
    USBHID(phy, 0, 0, vendor_id, product_id, product_release)
{
    _lock_status = 0;
}

USBAuthenticator::~USBAuthenticator() {
}

bool USBAuthenticator::test(HID_REPORT test) {
    _mutex.lock();

    HID_REPORT report;

    // report.data[0] = 0x01;
    
    /* test CTAPHID_INIT Command(0x06)*/
    // CID
    report.data[0] = test.data[0];
    report.data[1] = test.data[1];
    report.data[2] = test.data[2];
    report.data[3] = test.data[3];

    // CMD
    report.data[4] = 0x86;

    // BCNTH
    report.data[5] = 0x00;

    // BCNTL
    report.data[6] = 0x11;

    // DATA
    report.data[7] = test.data[7];
    report.data[8] = test.data[8];
    report.data[9] = test.data[9];
    report.data[10] = test.data[10];
    report.data[11] = test.data[11];
    report.data[12] = test.data[12];
    report.data[13] = test.data[13];
    report.data[14] = test.data[14];

    // DATA CID
    report.data[15] = 0x00;
    report.data[16] = 0x1d;
    report.data[17] = 0x00;
    report.data[18] = 0x08;

    // DATAs
    report.data[19] = 0x02;
    report.data[20] = 0x05;
    report.data[21] = 0x00;
    report.data[22] = 0x02;
    report.data[23] = 0x05;

    // zero DATA
    for (int i = 24; i < 64; i++) {
        report.data[i] = 0x00;
    }

    report.length = 64;

    if (!send(&report)) {
        _mutex.unlock();
        return false;
    }

    _mutex.unlock();
    return true;
}

const uint8_t *USBAuthenticator::report_desc() {
    static const uint8_t reportDescriptor[] = {
        USAGE_PAGE(2), 0xd1, 0xf1,      /* HID_UsagePage ( FIDO_USAGE_PAGE ) */
        USAGE(1), 0x01,                 /* HID_Usage ( FIDO_USAGE_CTAPHID ) */
        COLLECTION(1), 0x01,            /* HID_Collection ( HID_Application ) */

        USAGE(1), 0x20,                 /* HID_Usage ( FIDO_USAGE_DATA_IN ) */
        LOGICAL_MAXIMUM(1), 0x00,       /* HID_LogicalMin ( 0 ) */
        LOGICAL_MAXIMUM(2), 0xff, 0x00, /* HID_LogicalMaxS ( 0xff ) */
        REPORT_SIZE(1), 0x08,           /* HID_ReportSize ( 8 ) */
        REPORT_COUNT(1), 0x40,          /* HID_ReportCount ( HID_INPUT_REPORT_BYTES ) */
        INPUT(1), 0x02,                 /* HID_Input ( HID_DATA | HID_Absolute | HID_Variable ) */

        USAGE(1), 0x21,                 /* HID_Usage ( FIDO_USAGE_DATA_OUT) */
        LOGICAL_MINIMUM(1), 0x00,       /* HID_LogicalMin ( 0 ) */
        LOGICAL_MAXIMUM(2), 0xff, 0x00, /* HID_LogicalMaxS ( 0xff ) */
        REPORT_SIZE(1), 0x08,           /* HID_ReportSize (8) */
        REPORT_COUNT(1), 0x40,          /* HID_ReportCount ( HID_INPUT_REPORT_BYTES ) */
        OUTPUT(1), 0x02,                /* HID_Output ( HID_DATA | HID_Absolute | HID_Variable ) */

        END_COLLECTION(0),              /* HID_EndCollection */
    };
    reportLength = sizeof(reportDescriptor);
    return reportDescriptor;
}

void USBAuthenticator::report_rx() {
    assert_locked();

    HID_REPORT report;
    read_nb(&report);

    _lock_status = report.data[1] & 0x07;
}

uint8_t USBAuthenticator::lock_status() {
    return _lock_status;
}

#define DEFAULT_CONFIGURATION (1)
#define TOTAL_DESCRIPTOR_LENGTH ((1 * CONFIGURATION_DESCRIPTOR_LENGTH) \
                               + (1 * INTERFACE_DESCRIPTOR_LENGTH) \
                               + (1 * HID_DESCRIPTOR_LENGTH) \
                               + (2 * ENDPOINT_DESCRIPTOR_LENGTH))

const uint8_t *USBAuthenticator::configuration_desc(uint8_t index) {
    if (index != 0) {
        return NULL;
    }
    uint8_t configuration_descriptor_temp[] = {
        CONFIGURATION_DESCRIPTOR_LENGTH,                    // bLength
        CONFIGURATION_DESCRIPTOR,                           // bDescriptorType
        LSB(TOTAL_DESCRIPTOR_LENGTH),                       // wTotalLength (LSB)
        MSB(TOTAL_DESCRIPTOR_LENGTH),                       // wTotalLength (MSB)
        0x01,                                               // bNumInterfaces
        DEFAULT_CONFIGURATION,                              // bConfigurationValue
        0x00,                                               // iConfiguration
        C_RESERVED | C_SELF_POWERED,                        // bmAttributes
        C_POWER(0),                                         // bMaxPower

        INTERFACE_DESCRIPTOR_LENGTH,                        // bLength
        INTERFACE_DESCRIPTOR,                               // bDescriptorType
        0x00,                                               // bInterfaceNumber
        0x00,                                               // bAlternateSetting
        CTAPInterfaceDescriptorParam::NUM_ENDPOINTS,        // bNumEndpoints
        HID_CLASS,                                          // bInterfaceClass
        CTAPInterfaceDescriptorParam::INTERFACE_SUBCLASS,   // bInterfaceSubClass
        CTAPInterfaceDescriptorParam::INTERFACE_PROTOCOL,   // bInterfaceProtocol
        0x00,                                               // iInterface

        HID_DESCRIPTOR_LENGTH,                              // bLength
        HID_DESCRIPTOR,                                     // bDescriptorType
        LSB(HID_VERSION_1_11),                              // bcdHID (LSB)
        MSB(HID_VERSION_1_11),                              // bdcHID (MSB)
        0x00,                                               // bCountryCode
        0x01,                                               // bNumDescriptors
        REPORT_DESCRIPTOR,                                  // bDescriptorType,
        (uint8_t)(LSB(report_desc_length())),               // wDescriptorLength (LSB)
        (uint8_t)(MSB(report_desc_length())),               // wDescriptorLength (MSB)

        ENDPOINT_DESCRIPTOR_LENGTH,                         // bLength
        ENDPOINT_DESCRIPTOR,                                // bDescriptorType
        CTAPEndpointOneDescriptorParam::ENDPOINT_ADDRESS,   // bEndpointAddress
        CTAPEndpointOneDescriptorParam::M_ATTRIBUTES,       // bmAttributes
        LSB(CTAPEndpointOneDescriptorParam::MAX_PACKET_SIZE), // wMaxPacketSize (LSB)
        MSB(CTAPEndpointOneDescriptorParam::MAX_PACKET_SIZE), // wMaxPacketSize (MSB)
        CTAPEndpointOneDescriptorParam::INTERVAL,           // bInterval (milliseconds)

        ENDPOINT_DESCRIPTOR_LENGTH,                         // bLength,
        ENDPOINT_DESCRIPTOR,                                // bDescriptorType
        CTAPEndpointTwoDescriptorParam::ENDPOINT_ADDRESS,   // bEndpointAddress
        CTAPEndpointTwoDescriptorParam::M_ATTRIBUTES,       // bmAttributes
        LSB(CTAPEndpointTwoDescriptorParam::MAX_PACKET_SIZE), // wMaxPacketSize (LSB)
        MSB(CTAPEndpointTwoDescriptorParam::MAX_PACKET_SIZE), // wMaxPacketSize (MSB)
        CTAPEndpointTwoDescriptorParam::INTERVAL,           // bInterval (milliseconds)
    };
    MBED_ASSERT(sizeof(configuration_descriptor_temp) == sizeof(_configuration_descriptor));
    memcpy(_configuration_descriptor, configuration_descriptor_temp, sizeof(_configuration_descriptor));
    return _configuration_descriptor;
}

/**
 * @brief 初期パケットをパースする
 * 
 * @param report - 入力されたHID_REPORT
 */
void USBAuthenticator::parseRequest(HID_REPORT report) {
    this->req = new Request;
    /* CTAP Initialization Packet形式 */
    // Channel Identifierの取得
    memcpy(this->req->channelID, report.data, 4);
    // Command Identifierの取得
    this->req->command = (unsigned int)report.data[4];
    // BCNTHの取得
    this->req->BCNTH = (unsigned int)report.data[5];
    // BCNTLの取得
    // TODO;BCNTLが55以上ならフラグメントパースも行う
    this->req->BCNTL = (unsigned int)report.data[6] * 16 * 16;
    this->req->BCNTL = this->req->BCNTL + (unsigned int)report.data[7];
    this->writeCount = this->req->BCNTL;

    if (this->req->BCNTL > 55) { /* 継続パケットが存在する場合 */
        this->continuationFlag = true;
    }

    /* authenticatorAPIコマンド形式 */
    // Command Valueの取得
    this->req->data.commandValue = (unsigned int)report.data[8];
    // commandParameterの取得
    this->req->data.commandParameter = new uint8_t[this->req->BCNTL-1];
    for (int i=0; i<64-9; i++) {
        if (!(this->writeCount > 0)) {
            break;
        }
        this->req->data.commandParameter[i] = report.data[9+i];
        this->req->dataSize++;
        this->writeCount--;
    }
    // this->req->SerialDebug();
}

/**
 * @brief 継続パケットをパースする
 * 
 * @param report - 入力されたHID_REPORT
 */
void USBAuthenticator::parseContinuationPacket(HID_REPORT report) {
    this->continuation = new ContinuationPacket;
    // Channel Identifierの取得
    memcpy(this->continuation->channelID, report.data, 4);
    // Packet Sequenceの取得
    this->continuation->sequence = (unsigned int)report.data[4];
    // Payload dataの取得
    this->continuation->data = new uint8_t[64-5];
    for (int i=0; i<64-5; i++) {
        if (!(this->writeCount > 0)) {
            this->continuationFlag = false;
            break;
        }
        this->continuation->data[i] = report.data[5+i];
        this->continuation->dataSize++;
        this->writeCount--;
    }
    // this->continuation->SerialDebug();
    this->connectRequestData();

    /* 確保したメモリの解放 */
    delete this->continuation->data;
    delete this->continuation;
}

/**
 * @brief RequestのData部に継続パケットのデータ部を連結する
 */
void USBAuthenticator::connectRequestData() {
    for (int i=0; i<this->continuation->dataSize; i++) {
        this->req->data.commandParameter[this->req->dataSize] = this->continuation->data[i];
        this->req->dataSize++;
    }
}

/**
 * @brief CTAPを実行する
 */
void USBAuthenticator::operate() {
    try {
        operateCTAPCommand();
    } catch (std::exception& e) {
        // TODO:何故かエラー処理をするとうまくcatchできず落ちる
        Serial.println(e.what());
    }
    sendResponse();
    delete this->req->data.commandParameter;
    delete this->req;
}

/**
 * @brief CTAPのコマンドに対応した関数を呼び出す
 */
void USBAuthenticator::operateCTAPCommand() {
    switch (this->req->command) { /* Commandに応じた関数を呼び出す */
        case CTAPHID_MSG:
            operateMSGCommand(); break;
        case CTAPHID_CBOR:
            operateCBORCommand(); break;
        case CTAPHID_INIT:
            operateINITCommand(); break;
        case CTAPHID_PING:
            operatePINGCommand(); break;
        case CTAPHID_CANCEL:
            operateCANCELCommand(); break;
        case CTAPHID_ERROR:
            operateERRORCommand(); break;
        case CTAPHID_KEEPALIVE:
            operateKEEPALIVECommand(); break;
        default: /* Commandが存在しない場合 */
            throw implement_error("Not implement CTAP Command."); break;
    }
}

/**
 * @brief MSGコマンドを実行する
 */
void USBAuthenticator::operateMSGCommand() {
    throw implement_error("Not implement MSG Command.");
}

/**
 * @brief CBORコマンドを実行する
 */
void USBAuthenticator::operateCBORCommand() {
    // throw implement_error("Not implement CBOR Command.");
    if (checkHasParameters(this->req->data.commandValue)) {
        this->authAPI = new AuthenticatorAPI(this->req->data.commandValue, this->req->data.commandParameter, this->req->BCNTL);
    } else {
        this->authAPI = new AuthenticatorAPI(this->req->data.commandValue);
    }

    try {
        this->response = this->authAPI->operateCommand();
        this->response->ResponseSerialDebug();
    } catch (implement_error& e) {
        throw implement_error(e.what());
    }
}

/**
 * @brief INITコマンドを実行する
 */
void USBAuthenticator::operateINITCommand() {
    throw implement_error("Not implement INIT Command.");
}

/**
 * @brief PINGコマンドを実行する
 */
void USBAuthenticator::operatePINGCommand() {
    throw implement_error("Not implement PING Command.");
}

/**
 * @brief CANCELコマンドを実行する
 */
void USBAuthenticator::operateCANCELCommand() {
    throw implement_error("Not implement CANCEL Command.");
}

/**
 * @brief ERRORコマンドを実行する
 */
void USBAuthenticator::operateERRORCommand() {
    throw implement_error("Not implement ERROR Command.");
}

/**
 * @brief KEEPALIVEコマンドを実行する
 */ 
void USBAuthenticator::operateKEEPALIVECommand() {
    throw implement_error("Not implement KEEPALIVE Command.");
}

/**
 * @brief 作成したレスポンスを送信する
 */
void USBAuthenticator::sendResponse() {
    HID_REPORT report;
    size_t reportLength = 0;
    size_t responseDataLength = 0;
    size_t continuationCount = 0;
    // Responseサイズが55以上だった場合分割送信する
    if (this->response->length > 55) { // 継続パケットのパケット数を計算
        continuationCount = ((this->response->length - 55) / 59) + 1;
    }

    /* Initialize Packetの作成 */
    // channel identifierの設定(Requestと同じ)
    for (int i=0; i<4; i++) {
        report.data[reportLength] = this->req->channelID[i];
        reportLength++;
    }

    // command Identifierの設定(Requestと同じ)
    report.data[reportLength] = this->req->command;
    reportLength++;

    // BCNTHの設定
    report.data[reportLength] = 0x00;
    reportLength++;

    // BCNTLの設定(TODO:255Bytes以上なら最初のバイトにも格納)
    report.data[reportLength] = 0x00;
    reportLength++;
    report.data[reportLength] = this->response->length;
    reportLength++;

    // Dataの設定
    report.data[reportLength] = this->response->status;
    reportLength++;
    for (int i=0; i<55; i++) {
        if (responseDataLength > response->length) { /* 格納終了判定 */
            report.data[reportLength] = 0x00;
            reportLength++;
        } else { /* 通常データ格納 */
            report.data[reportLength] = this->response->responseData[responseDataLength];
            reportLength++;
            responseDataLength++;
        }
    }
    // 長さの設定
    report.length = 64;
    // 送信
    if (!send(&report)) {
        _mutex.unlock();
    }

    /* Continuation Packetの作成 */
    for (int i=0; i<continuationCount; i++) {
        HID_REPORT continuationReport;
        size_t continuationLength = 0;
        // channel identifierの設定(Requestと同じ)
        for (int j=0; j<4; j++) {
            continuationReport.data[continuationLength] = this->req->channelID[j];
            continuationLength++;
        }

        // Packet Sequenceの設定
        continuationReport.data[continuationLength] = 0x80 + i;
        continuationLength++;

        // Dataの設定
        for (int j=0; j<59; j++) { /* 格納終了判定 */
            if (responseDataLength > response->length) {
                continuationReport.data[continuationLength] = 0x00;
                continuationLength++;
            } else { /* 通常データ格納 */
                continuationReport.data[continuationLength] = this->response->responseData[responseDataLength];
                continuationLength++;
                responseDataLength++;
            }
        }
        // 長さの設定
        continuationReport.length = 64;
        // 送信
        if (!send(&continuationReport)) {
            _mutex.unlock();
        }
    }

    Serial.println("Packet end.");
}

bool USBAuthenticator::getWriteFlag() {
    return this->writeFlag;
}

bool USBAuthenticator::getContinuationFlag() {
    return this->continuationFlag;
}

void USBAuthenticator::setWriteFlag(bool writeFlag) {
    this->writeFlag = writeFlag;
}

void USBAuthenticator::setContinuationFlag(bool continuationFlag) {
    this->continuationFlag = continuationFlag;
}