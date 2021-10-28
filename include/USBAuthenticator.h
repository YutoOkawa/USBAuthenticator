#ifndef USBAUTHENTICATOR_H
#define USBAUTHENTICATOR_H

#include "PluggableUSBHID.h"
#include "platform/Stream.h"
#include "PlatformMutex.h"
#include "ctap.hpp"

namespace arduino {

struct CTAPInterfaceDescriptorParam {
    static const int NUM_ENDPOINTS;
    static const int INTERFACE_SUBCLASS;
    static const int INTERFACE_PROTOCOL;
};

struct CTAPEndpointOneDescriptorParam {
    static const int M_ATTRIBUTES;
    static const int ENDPOINT_ADDRESS;
    static const int MAX_PACKET_SIZE;
    static const int INTERVAL;
};

struct CTAPEndpointTwoDescriptorParam {
    static const int M_ATTRIBUTES;
    static const int ENDPOINT_ADDRESS;
    static const int MAX_PACKET_SIZE;
    static const int INTERVAL;
};

class USBAuthenticator: public USBHID {
    public:
        /** 完全コピペ
        * Basic constructor
        *
        * Construct this object optionally connecting and blocking until it is ready.
        *
        * @note Do not use this constructor in derived classes.
        *
        * @param connect_blocking true to perform a blocking connect, false to start in a disconnected state
        * @param vendor_id Your vendor_id
        * @param product_id Your product_id
        * @param product_release Your product_release
        */
        USBAuthenticator(bool connect_blocking = true, uint16_t vendor_Id = 0x1235, uint16_t product_id = 0x0050, uint16_t product_release = 0x0001);

        /** 完全コピペ
        * Fully featured constructor
        *
        * Construct this object with the supplied USBPhy and parameters. The user
        * this object is responsible for calling connect() or init().
        *
        * @note Derived classes must use this constructor and call init() or
        * connect() themselves. Derived classes should also call deinit() in
        * their destructor. This ensures that no interrupts can occur when the
        * object is partially constructed or destroyed.
        *
        * @param phy USB phy to use
        * @param vendor_id Your vendor_id
        * @param product_id Your product_id
        * @param product_release Your product_release
        */    
        USBAuthenticator(USBPhy *phy, uint16_t vendor_id = 0x1235, uint16_t product_id = 0x0050, uint16_t product_release = 0x0001);

        /**
         * @brief Destroy the USBAuthenticator object
         */
        virtual ~USBAuthenticator();

        /**
         * @brief test Function.
         * 
         * @return true 
         * @return false 
         */
        bool test(HID_REPORT test);

        /**
         * @brief To define the report descriptor.
         * 
         * @return const uint8_t* 
         */
        virtual const uint8_t *report_desc();

        /* 完全コピペ
        * Called when a data is received on the OUT endpoint. Useful to switch on LED of LOCK keys
        */
        virtual void report_rx();

        /** 完全コピペ
        * Read status of lock keys. Useful to switch-on/off leds according to key pressed. Only the first three bits of the result is important:
        *   - First bit: NUM_LOCK
        *   - Second bit: CAPS_LOCK
        *   - Third bit: SCROLL_LOCK
        *
        * @returns status of lock keys
        */
        uint8_t lock_status();

        void operate();

        void parseRequest(HID_REPORT report);
        void operateCTAPCommand();
        void operateMSGCommand();
        void operateCBORCommand();
        void operateINITCommand();
        void operatePINGCommand();
        void operateCANCELCommand();
        void operateERRORCommand();
        void operateKEEPALIVECommand();

        bool getWriteFlag();
        bool getContinuationFlag();

        void setWriteFlag(bool writeFlag);
        void setContinuationFlag(bool continuationFlag);

    protected:
        virtual const uint8_t *configuration_desc(uint8_t index);
    
    private:
        uint8_t _lock_status;
        uint8_t _configuration_descriptor[41];
        PlatformMutex _mutex;

        /**
         * @brief 継続バケットが存在するかのフラグ
         */
        bool continuationFlag;

        /**
         * @brief 認証器処理のフラグ
         */
        bool writeFlag;

        /**
         * @brief Request
         */
        Request req;

        AuthenticatorAPI *authAPI;

        Response *response;
};

}

#endif