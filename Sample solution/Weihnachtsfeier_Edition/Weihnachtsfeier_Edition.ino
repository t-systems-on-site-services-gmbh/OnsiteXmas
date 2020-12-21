//*** Import of Libraries *********************************************************************************************************************************************
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <Adafruit_NeoPixel.h>
//*********************************************************************************************************************************************************************

//*** Global Settings *************************************************************************************************************************************************
#define LED_PIN     14  // Defines the Data-Input Pin
#define LED_COUNT    4  // Number of LEDs in the device

#define DEVICE_NAME "OnsiteXmas"

// https://www.uuidgenerator.net/ to get a unique UUID
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

uint32_t            colormode;              
std::string         brightness = "75";          // Brightness of the LEDs
std::string         oldBrightness = "0";        // The old value of the brightness
std::string         powerMode = "on";           // Powermode of the LEDs (on/off)
std::string         ledColor = "226,00,116";    // Color of the LEDs (Magenta)

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
        powerMode = pControlCharacteristic->getValue();
        Serial.print("New PowerMode: ");
        Serial.println(powerMode.c_str());
        
        // Dirty but cool solution for on/off bug ;-)
        oldBrightness = "-1";
      }
    }
};

// LED Colors
class LedColorCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic* pCharacteristic) {
      if (pColorCharacteristic->getValue().length() > 0) {
        // Get value via Bluetooth
        ledColor = pColorCharacteristic->getValue();
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
        brightness = pBrightCharacteristic->getValue();
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
  if (check_on_off())
  {
    setBright(brightness.c_str());
    turn_on_leds(ledColor.c_str());
  }
  else 
  {
    turn_off_leds();
  }
  
  delay(10);
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

  // Encryption
  Device.setEncryptionLevel(ESP_BLE_SEC_ENCRYPT);
  pControlCharacteristic->setAccessPermissions(ESP_GATT_PERM_READ_ENCRYPTED | ESP_GATT_PERM_WRITE_ENCRYPTED);
  pColorCharacteristic->setAccessPermissions(ESP_GATT_PERM_READ_ENCRYPTED | ESP_GATT_PERM_WRITE_ENCRYPTED);
  pBrightCharacteristic->setAccessPermissions(ESP_GATT_PERM_READ_ENCRYPTED | ESP_GATT_PERM_WRITE_ENCRYPTED);

  // Callbacks
  pServer->setCallbacks(new ServerCallbacks());
  pControlCharacteristic->setCallbacks(new ControlCharacteristicCallbacks());
  pColorCharacteristic->setCallbacks(new LedColorCallbacks());
  pBrightCharacteristic->setCallbacks(new BrightnessCallbacks());

  // Set start values
  pControlCharacteristic->setValue("on");
  pColorCharacteristic->setValue("226,00,116"); // Magenta
  pBrightCharacteristic->setValue("75");

  // Get start values
  powerMode  = pControlCharacteristic->getValue();
  ledColor   = pColorCharacteristic->getValue();
  brightness = pBrightCharacteristic->getValue();

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
//*********************************************************************************************************************************************************************

// *** Loop Functions *************************************************************************************************************************************************
boolean check_on_off() {
  if (powerMode == "on" || powerMode == "On" || powerMode == "ON" || powerMode == "1") 
  {
    return true;
  }
  else
  {
    return false;
  }
}

void turn_off_leds() {
  Serial.println("Turning off");
  
  Stripe.clear();
  Stripe.show();
  Stripe.setBrightness(0);
  colormode = Stripe.Color(0, 0, 0);
}

// 163,73,164
void turn_on_leds(String rgb) {
  Serial.println("Turning on");
  
  int firstSplit = rgb.indexOf(',');
  int lastSplit  = rgb.lastIndexOf(',');

  // Get single values of Red, Green and Blue
  int red   = rgb.substring(0, firstSplit).toInt();
  int green = rgb.substring(firstSplit+1, lastSplit).toInt();
  int blue  = rgb.substring(lastSplit+1, rgb.length()).toInt();

  Serial.print("RGB: ");
  Serial.print(red);
  Serial.print(", ");
  Serial.print(green);
  Serial.print(", ");
  Serial.println(blue);

  setColor(Stripe.Color(red, green, blue));
}

void setColor(uint32_t color) {
  for (int x = 0; x <= LED_COUNT-1; x++) {
    Stripe.setPixelColor(x, color);
  }

  // Only if something changed
  if (colormode != color) {
    Stripe.show();

    colormode = color;
  }
}

void setBright(std::string bright) {
  Serial.print("Brightness: ");
  Serial.println(bright.c_str());

  // Only if something changed
  if (bright != oldBrightness) {
    turn_off_leds();
    Stripe.setBrightness(atoi(bright.c_str()));
      
    oldBrightness = bright; 
  }
}
//*********************************************************************************************************************************************************************
