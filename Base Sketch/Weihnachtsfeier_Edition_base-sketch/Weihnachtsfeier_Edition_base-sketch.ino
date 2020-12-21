//*** Import of Libraries *********************************************************************************************************************************************
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <Adafruit_NeoPixel.h>
//*********************************************************************************************************************************************************************

//*** Global Settings *************************************************************************************************************************************************
#define LED_PIN     14  // Defines the Data-Input Pin
#define LED_COUNT    4  // Number of LEDs in the device

#define DEVICE_NAME "OnsiteXmas" //Name is not allowed to be changed when using the onsite xmas app

// https://www.uuidgenerator.net/ to get a unique UUID
//UUIDs is not allowed to be changed when using the onsite xmas app
#define SERVICE_UUID        "fcf88392-048b-11ea-8d71-362b9e155667"
#define CONTROL_CHAR_UUID   "fcf88b58-048b-11ea-8d71-362b9e155667"
#define COLOR_CHAR_UUID     "93783900-051a-11ea-8d71-362b9e155667"
#define BRIGHT_CHAR_UUID    "5cd72205-09d2-4a58-b704-f59bca4bfc8c"
//*********************************************************************************************************************************************************************

//*** Global Objects & Variables **************************************************************************************************************************************
BLEDevice           Device;
BLEServer*          pServer;
BLEService*         pService;
BLECharacteristic*  pControlCharacteristic;
BLECharacteristic*  pColorCharacteristic;
BLECharacteristic*  pBrightCharacteristic;
BLEAdvertising*     pAdvertising;

Adafruit_NeoPixel Stripe(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
//*********************************************************************************************************************************************************************

//*** Callbacks Classes ***********************************************************************************************************************************************
// Connect / Disconnect
class ServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer* pServerCallback) {
      Serial.println("Client connected");
    }

    void onDisconnect(BLEServer* pServerCallback) {
      Serial.println("Client disconnected");
    }
};

// On / Off
class ControlCharacteristicCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic* pCharacteristic) {
      if (pControlCharacteristic->getValue().length() > 0) {
        // Get value via Bluetooth
        std::string powerMode = pControlCharacteristic->getValue();
        Serial.print("New PowerMode: ");
        Serial.println(powerMode.c_str());
      }
    }
};

// LED Colors
class LedColorCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic* pCharacteristic) {
      if (pColorCharacteristic->getValue().length() > 0) {
        // Get value via Bluetooth
        std::string ledColor = pColorCharacteristic->getValue();
        Serial.print("New LED Color: ");
        Serial.println(ledColor.c_str());
      }
    }
};

// LED Brightness
class BrightnessCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic* pCharacteristic) {
      if (pBrightCharacteristic->getValue().length() > 0) {
        // Get value via Bluetooth
        std::string brightness = pBrightCharacteristic->getValue();
        Serial.print("New Brightness: ");
        Serial.println(brightness.c_str());
      }
    }
};
//*********************************************************************************************************************************************************************

// *** Arduino Main Functions *****************************************************************************************************************************************
void setup() {
  // Set Baudrate to get the right output
  Serial.begin(9600);

  initBLE();

  // Initialize LED Stripe
  Stripe.begin();
}

void loop() {

}
//*********************************************************************************************************************************************************************

// *** BLE Functions **************************************************************************************************************************************************
// Method to initialize Bluetooth Low Energy
void initBLE() {
  // Give your Device a custom name (Bluetooth-Name)
  Device.init(DEVICE_NAME); 
  
  // Init Server, Service and Characteristics
  pServer       = Device.createServer();
  pService      = pServer->createService(SERVICE_UUID);
  pControlCharacteristic  = pService->createCharacteristic(CONTROL_CHAR_UUID, BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_READ);
  pColorCharacteristic    = pService->createCharacteristic(COLOR_CHAR_UUID, BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_READ);
  pBrightCharacteristic   = pService->createCharacteristic(BRIGHT_CHAR_UUID, BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_READ);


  // Callbacks
  pServer->setCallbacks(new ServerCallbacks());
  pControlCharacteristic->setCallbacks(new ControlCharacteristicCallbacks());
  pColorCharacteristic->setCallbacks(new LedColorCallbacks());
  pBrightCharacteristic->setCallbacks(new BrightnessCallbacks());

  // Set start values
  pControlCharacteristic->setValue("on");
  pColorCharacteristic->setValue("226,00,116"); // Magenta
  pBrightCharacteristic->setValue("75");


  pService->start();

  // Advertising
  pAdvertising = Device.getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);
  pAdvertising->setMinPreferred(0x12);
  Device.startAdvertising();

  Serial.print(DEVICE_NAME);
  Serial.println(" Advertised");
};
