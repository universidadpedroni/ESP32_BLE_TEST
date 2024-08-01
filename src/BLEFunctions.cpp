#include "BLEFunctions.h"

// Definitions of BLECharacteristics and BLEDescriptors
BLECharacteristic temperatureCharacteristic(BLEUUID((uint16_t)0x2A6E), BLECharacteristic::PROPERTY_NOTIFY);
BLEDescriptor temperatureDescriptor(BLEUUID((uint16_t)0x2902));
BLECharacteristic humidityCharacteristic(BLEUUID((uint16_t)0x2A6F), BLECharacteristic::PROPERTY_NOTIFY);
BLEDescriptor humidityDescriptor(BLEUUID((uint16_t)0x2902));
BLECharacteristic pressureCharacteristic(BLEUUID((uint16_t)0x2A6D), BLECharacteristic::PROPERTY_NOTIFY);
BLEDescriptor pressureDescriptor(BLEUUID((uint16_t)0x2902));

BLEHandler::BLEHandler() : pServer(nullptr), pCharacteristic(nullptr), pWriteCharacteristic(nullptr), deviceConnected(false), lastValue("") {}

void BLEHandler::MyCharacteristicCallbacks::onWrite(BLECharacteristic *pWriteCharacteristic) {
    std::string value = pWriteCharacteristic->getValue();
    if (value.length() > 0) {
        handler->lastValue = String(value.c_str()); // Actualiza el valor leído
        /*Serial.println("*********");
        Serial.print("Nuevo valor escrito: ");
        for (int i = 0; i < value.length(); i++)
            Serial.print(value[i]);
        Serial.println();
        Serial.println("*********");*/
    }
}

void BLEHandler::MyServerCallbacks::onConnect(BLEServer* pServer) {
    handler->deviceConnected = true;
    Serial.println("Device Connected");
}

void BLEHandler::MyServerCallbacks::onDisconnect(BLEServer* pServer) {
    handler->deviceConnected = false;
    Serial.println("Device Disconnected");
}

void BLEHandler::init() {
    // Crear el BLE Device
    BLEDevice::init(BLE_SERVER_NAME);
    // Crear el BLE Server
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks(this));
    
    // Crear el servicio principal
    BLEService* pService = pServer->createService(SERVICE_UUID);
    if (!pService) {
        Serial.println("Failed to create service");
        return;
    }

    // Crear las características y los descriptores
    pService->addCharacteristic(&temperatureCharacteristic);
    temperatureCharacteristic.addDescriptor(&temperatureDescriptor);
  
    pService->addCharacteristic(&humidityCharacteristic);
    humidityCharacteristic.addDescriptor(&humidityDescriptor);

    pService->addCharacteristic(&pressureCharacteristic);
    pressureCharacteristic.addDescriptor(&pressureDescriptor);

    pWriteCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID,
                            BLECharacteristic::PROPERTY_READ |
                            BLECharacteristic::PROPERTY_WRITE);
    pWriteCharacteristic->setCallbacks(new MyCharacteristicCallbacks(this));
    pWriteCharacteristic->setValue("Write Me!");
    
    // Iniciar el servicio principal
    pService->start();
    
    // Registrar servicio de información del dispositivo
    BLEService* pDevInfoService = pServer->createService(DEVINFO_UUID);
    if (!pDevInfoService) {
        Serial.println("Failed to create device info service");
        return;
    }
    BLECharacteristic *characteristic = pDevInfoService->createCharacteristic(DEVINFO_MANUFACTURER_UUID, BLECharacteristic::PROPERTY_READ);
    characteristic->setValue(DEVICE_MANUFACTURER);
    characteristic = pDevInfoService->createCharacteristic(DEVINFO_NAME_UUID, BLECharacteristic::PROPERTY_READ);
    characteristic->setValue(DEVICE_NAME);
    characteristic = pDevInfoService->createCharacteristic(DEVINFO_SERIAL_UUID, BLECharacteristic::PROPERTY_READ);
    String chipId = String((uint32_t)(ESP.getEfuseMac() >> 24), HEX);
    characteristic->setValue(chipId.c_str());
    pDevInfoService->start();

    // Iniciar el advertising
    BLEAdvertising* pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06);
    pAdvertising->setMinPreferred(0x12);
    BLEDevice::startAdvertising();
    Serial.println("BLE Terminado");
}

void BLEHandler::update(float t, float h, float p) {
    if (deviceConnected) {
        // Notify temperature reading
        uint16_t temperature = (uint16_t)t;
        temperatureCharacteristic.setValue(temperature);
        temperatureCharacteristic.notify();
        //Serial.print("Temperature Celsius: ");
        //Serial.print(t);
        //Serial.println(" ºC");
   
        // Notify humidity reading
        uint16_t humidity = (uint16_t)h;
        humidityCharacteristic.setValue(humidity);
        humidityCharacteristic.notify();   
        //Serial.print("Humidity: ");
        //Serial.print(h);
        //Serial.println(" %");

        // Notify pressure reading
        uint16_t pressure = (uint16_t)p;
        pressureCharacteristic.setValue(pressure);
        pressureCharacteristic.notify();   
        //Serial.print("Pressure: ");
        //Serial.print(p);
        //Serial.println(" hPa");
    }
}

String BLEHandler::getLastValue() const {
    return lastValue;
}
