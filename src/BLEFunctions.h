#ifndef BLEFUNCTIONS_H
#define BLEFUNCTIONS_H

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include "BLEConstants.h"

class BLEHandler {
public:
    BLEHandler();
    void init();
    void update(uint16_t valor);

private:
    BLEServer* pServer;
    BLECharacteristic* pCharacteristic;
    bool deviceConnected;

    class MyServerCallbacks : public BLEServerCallbacks {
    public:
        MyServerCallbacks(BLEHandler* handler) : handler(handler) {}
        void onConnect(BLEServer* pServer);
        void onDisconnect(BLEServer* pServer);
    private:
        BLEHandler* handler;
    };

    //static constexpr const char* SERVICE_UUID = "4fafc201-1fb5-459e-8fcc-c5c9c331914b";
    //static constexpr const char* CHARACTERISTIC_UUID = "beb5483e-36e1-4688-b7f5-ea07361b26a8";
};

#endif // BLEFUNCTIONS_H

