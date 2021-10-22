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