#include "BLEFunctions.h"

BLEHandler::BLEHandler() : pServer(nullptr), pCharacteristic(nullptr), deviceConnected(false) {}

void BLEHandler::MyServerCallbacks::onConnect(BLEServer* pServer) {
    handler->deviceConnected = true;
    BLEDevice::startAdvertising();
    Serial.println("Device Connected");
}

void BLEHandler::MyServerCallbacks::onDisconnect(BLEServer* pServer) {
    handler->deviceConnected = false;
    Serial.println("Device Disconnected");
}

void BLEHandler::init() {
    // Crear el BLE Device
    BLEDevice::init(BLE_DEVICE_NAME);
    // Crear el BLE Server
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks(this));
    // Crear el servicio
    BLEService* pService = pServer->createService(SERVICE_UUID);
    // Crear una característica
    pCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_READ |
        BLECharacteristic::PROPERTY_WRITE |
        BLECharacteristic::PROPERTY_NOTIFY
    );
    pCharacteristic->setValue("Hola Mundo!");
    // Crear el descriptor
    //pCharacteristic->addDescriptor(new BLE2902());

    // Iniciar el servicio
    pService->start();

    // Register device info service, that contains the device's UUID, manufacturer and name.
    pService = pServer->createService(DEVINFO_UUID);
    BLECharacteristic *characteristic = pService->createCharacteristic(DEVINFO_MANUFACTURER_UUID, BLECharacteristic::PROPERTY_READ);
    characteristic->setValue(DEVICE_MANUFACTURER);
    characteristic = pService->createCharacteristic(DEVINFO_NAME_UUID, BLECharacteristic::PROPERTY_READ);
    characteristic->setValue(DEVICE_NAME);
    characteristic = pService->createCharacteristic(DEVINFO_SERIAL_UUID, BLECharacteristic::PROPERTY_READ);
    String chipId = String((uint32_t)(ESP.getEfuseMac() >> 24), HEX);
    characteristic->setValue(chipId.c_str());
    pService->start();


    // Iniciar el advertising
    BLEAdvertising* pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06);
    pAdvertising->setMinPreferred(0x12);
    BLEDevice::startAdvertising();
    Serial.println("BLE Terminado");
}

void BLEHandler::update(uint16_t valor) {
    
    if (deviceConnected) {
        pCharacteristic->setValue("25");
        pCharacteristic->notify();
        Serial.printf("Valor enviado: %f\n", valor);
    }
}
