//ALL CREDIT TO KEVIN KNOTTBERG

#include <LoRa.h>
#include <stdint.h>

#define CS_PIN 18
#define RST_PIN 23
#define IRQ_PIN 26

#define PACKET_SIZE 255

// Macros for static device configuration
/*=== Possible Bandwidths ===
    - 7.8E3 | 7.8 kHz
    - 10.4E3 | 10.4 kHz
    - 15.6E3 | 15.6 kHz
    - 20.8E3 | 20.8 kHz
    - 31.25E3 | 31.25 kHz
    - 41.7E3 | 41.7 kHz
    - 62.5E3 | 62.5 kHz
    - 125E3 | 125 kHz
    - 250E3 | 250 kHz
    - 500E3 | 500 kHz
*/
#define BANDWIDTH 250E3

/*=== Transmit Power Levels ===
    - 14
    - 15
    - 16
    - 17
    - 18
    - 19
    - 20 
*/
#define TX_POWER 20

/*=== Frequency ===
    - 902 - 928 MHz
*/
#define FREQ 915E6


uint8_t data_packet[PACKET_SIZE];
//const char dataPattern[256] PROGMEM = "F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F";


void setup() {
  Serial.begin(115200);
  while(!Serial);

  LoRa.setPins(CS_PIN, RST_PIN, IRQ_PIN);

  if(!LoRa.begin(FREQ))
  {
    Serial.println("LoRa init failed. Check your connections");
    while(true);
  }

  LoRa.disableCrc();
  LoRa.setTxPower(TX_POWER);
  LoRa.setSignalBandwidth(BANDWIDTH);
  LoRa.setCodingRate4(5);
  LoRa.setSpreadingFactor(7);
}

void loop() {
    clear_buffer();
    while(Serial.available() == 0);
    char input = (char) Serial.read(); // Clear
    if(input == 'e')
      while(true);
    else if(input == 'r')
    {
      Serial.println("Transmitter");
      configure_radio();
    }
    else
    {
      generate_random_packet();
      //Serial.println("F0" * 127 + "F");
      LoRa.beginPacket(false); // By adding true we are implying that we want the implicit header style
      LoRa.write(data_packet, PACKET_SIZE);
      LoRa.endPacket(false);
      delay(2000);
    }
}

void generate_random_packet()
{
  Serial.print(String(PACKET_SIZE) + " ");

  for(int i = 0; i < PACKET_SIZE; i++)
  {
    if(i%2){ data_packet[i] = '0'; }
    else{ data_packet[i] = 'F';}
    Serial.print((char)data_packet[i]);
  }

  Serial.println("");
}

void configure_radio()
{
    // Put the radio into standby to update
    LoRa.idle();
    
    Serial.println("Enter spreading factor: 6, 7, 8, 9, 10, 11, 12");
    clear_buffer();
    while(Serial.available() == 0);
    int spreading_factor = Serial.parseInt();
    LoRa.setSpreadingFactor(spreading_factor);

    Serial.println("Enter BW: 62500, 125000, 250000, 500000");
    clear_buffer();
    while(Serial.available() == 0);
    int bandwidth = Serial.parseInt();
    LoRa.setSignalBandwidth(bandwidth);

    Serial.println("Enter number of redundancy bits: 5, 6, 7, 8");
    clear_buffer();
    while(Serial.available() == 0);
    int coding_rate = Serial.parseInt();
    LoRa.setCodingRate4(coding_rate);

    Serial.println("Enter Output Power: 14, 17, 20");
    clear_buffer();
    while(Serial.available() == 0);
    int pwr = Serial.parseInt();
    LoRa.setTxPower(pwr);
    

    Serial.print("Set spreading factor to ");
    Serial.println(LoRa.getSpreadingFactor(), DEC);
    Serial.print("Current bandwidth: ");
    Serial.println(LoRa.getSignalBandwidth(), DEC);
    Serial.print("Setting coding rate to ");
    Serial.println(coding_rate);


}

void clear_buffer()
{
    while(Serial.available() > 0)
        Serial.read();
}
  