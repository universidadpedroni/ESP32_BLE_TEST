#ifndef BLEFUNCTIONS_H
#define BLEFUNCTIONS_H

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define BLE_SERVER_NAME "Mi ESP32"
#define SERVICE_UUID (BLEUUID((uint16_t)0x181A))
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

#define DEVINFO_UUID (uint16_t)0x180a
#define DEVINFO_MANUFACTURER_UUID (uint16_t)0x2a29
#define DEVINFO_NAME_UUID (uint16_t)0x2a24
#define DEVINFO_SERIAL_UUID (uint16_t)0x2a25

#define DEVICE_MANUFACTURER "+54 Stompboxes"
#define DEVICE_NAME "BLETest"

class BLEHandler {
public:
    BLEHandler();
    void init();
    void update(float t = 25, float h = 80, float p = 100);

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
};

#endif // BLEFUNCTIONS_H


