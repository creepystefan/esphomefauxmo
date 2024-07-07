#include "esphome.h"
#include "fauxmoESP.h"
#include <Arduino.h>


fauxmoESP fauxmo;

#define SERIAL_BAUDRATE     115200

#define LED                 2
#define GPIO_RX             3
#define GPIO_TX             1
#define ID_LED              "pin2"
#define ID_GPIORX           "pin3"
#define ID_GPIOTX           "pin1"

class MyCustomSensor : public Component, public Sensor {
 public:
  void setup() override {
   
    // Init serial port and clean garbage
    Serial.begin(SERIAL_BAUDRATE);
    Serial.println();
    Serial.println();

   // Wifi
    wifiSetup();
    fauxmo.createServer(true); // not needed, this is the default value
    fauxmo.setPort(80); // This is required for gen3 devices
    fauxmo.enable(true);
    
    // LEDs
    pinMode(LED, OUTPUT);         //#define LED        2
    pinMode(GPIO_RX, OUTPUT);     //#define GPIO_RX    3
    pinMode(GPIO_TX, OUTPUT);     //#define GPIO_TX    1

    // Add virtual devices
    fauxmo.addDevice(ID_);        
    fauxmo.addDevice(ID_LED);      //#define ID_LED       "pin2"
    fauxmo.addDevice(ID_GPIORX);   //#define ID_GPIORX    "pin3"
    fauxmo.addDevice(ID_GPIOTX);   //#define ID_GPIORX    "pin1"
    
    fauxmo.onSetState([](unsigned char device_id, const char * device_name, bool state, unsigned char value) {
        
        
        Serial.printf("[MAIN] Device #%d (%s) state: %s value: %d\n", device_id, device_name, state ? "ON" : "OFF", value);


        if (strcmp(device_name, ID_LED)==0) {               //#define ID_LED       "pin2"
            digitalWrite(LED, state ? HIGH : LOW);          //#define LED              2
        } else if (strcmp(device_name, ID_GPIORX)==0) {     //#define ID_GPIORX    "pin3"
            digitalWrite(GPIO_RX, state ? HIGH : LOW);      //#define GPIO_RX             5
        } else if (strcmp(device_name, ID_GPIOTX)==0) {     //#define ID_GPIOTX    "pin1"
            digitalWrite(GPIO_TX, state ? HIGH : LOW);      //#define GPIO_TX             1
        }

    });


  }
  void loop() override {
      fauxmo.handle();

    static unsigned long last = millis();
    if (millis() - last > 5000) {
        last = millis();
        Serial.printf("[MAIN] Free heap: %d bytes\n", ESP.getFreeHeap());
    }
  }
};
