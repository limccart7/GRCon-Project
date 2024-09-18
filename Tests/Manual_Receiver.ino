#include <LoRa.h>

// Device pin settings for the T-Beam 
#define CS_PIN 18
#define RST_PIN 23
#define IRQ_PIN 26

// Macros for static device configuration
#define BANDWIDTH 125E3
#define SPREADING_FACTOR 7
#define CODING_RATE 5
#define SYNC_WORD 0x12
#define FREQUENCY 915E6 
#define OCP 240 
#define GAIN_LEVEL 0 

void setup() {
  Serial.begin(115200);  // baud rate must match the transmitter
  while (!Serial);

  Serial.println("LoRa Receiver");

  // Initialize LoRa with the specified pins
  LoRa.setPins(CS_PIN, RST_PIN, IRQ_PIN);

  if (!LoRa.begin(FREQUENCY)) {  // Initialize LoRa at the specified frequency
    Serial.println("LoRa init failed. Check your connections");
    while (true);
  }

  LoRa.disableCrc();
  LoRa.setOCP(OCP);
  
  // Setting the aforementioned configurable parameters
  LoRa.setSpreadingFactor(SPREADING_FACTOR);
  LoRa.setSignalBandwidth(BANDWIDTH);
  LoRa.setCodingRate4(CODING_RATE);
  LoRa.setGain(GAIN_LEVEL);

  LoRa.onReceive(onReceive);
  LoRa.receive();
}

void loop() {
  // Empty loop
}

void onReceive(int packetSize) {
  if (packetSize == 0) return;

  String receivedData = "";
  
  while (LoRa.available()) {
    char c = (char) LoRa.read();
    receivedData += c;
  }

  Serial.println(receivedData);  // Print only the received data
}