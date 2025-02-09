#include <SPI.h>
#include <Adafruit_NeoPixel.h>
#include <Ethernet.h>

#include <ArduinoModbus.h>

#define NEO_PIXEL_PIN 8
#define ENET_SS_PIN 10
#define ENET_SERVER_PORT 502

#define UID_REG 40001

#define MAC_ADDR_0_REG 40002
#define MAC_ADDR_1_REG 40003
#define MAC_ADDR_2_REG 40004
#define MAC_ADDR_3_REG 40005
#define MAC_ADDR_4_REG 40006
#define MAC_ADDR_LENGTH 5

#define IP_ADDR_0_REG 40007
#define IP_ADDR_1_REG 40008
#define IP_ADDR_2_REG 40009
#define IP_ADDR_3_REG 40010
#define IP_ADDR_LENGTH 4

#define ENABLE_COIL 20001
#define BRIGHTNESS_INPUT_REG 30002
#define RED_INPUT_REG 30003
#define GREEN_INPUT_REG 30004
#define BLUE_INPUT_REG 30005
#define COLOR_INPUT_REG RED_INPUT_REG
#define COLOR_INPUT_REG_LENGTH 3

const uint8_t UID = 1;
const uint8_t ip_addr[] = {192, 168, 1, 177}; 
byte mac[] = {
  0x98, 0x76, 0xB6, 0x12, 0xF8, 0xDC
}; // fix this mac
IPAddress ip((const uint8_t *)&ip_addr);

EthernetServer ethServer(ENET_SERVER_PORT);
EthernetClient ethClient;
ModbusTCPServer modbusTCPServer;

Adafruit_NeoPixel pixel(1, NEO_PIXEL_PIN, NEO_GRB + NEO_KHZ800);

uint8_t prev_brightness = 0, prev_r = 0, prev_g = 0, prev_b = 0, prev_en = 0, count = 0;

void setup() {
  // start the Ethernet connection and the server:
  pixel.begin();
  pixel.setPixelColor(0, 255, 0, 0);
  pixel.setBrightness(64);
  pixel.show();

  Serial.begin(9600);
  while(!Serial){ }

  // start the Modbus TCP server
  if (!modbusTCPServer.begin()) {
    Serial.println("Failed to start Modbus TCP Server!");
    while (1);
  }

  // configure a single coil at address 0x00
  modbusTCPServer.configureHoldingRegisters(UID_REG, 1);
  modbusTCPServer.configureHoldingRegisters(MAC_ADDR_0_REG, MAC_ADDR_LENGTH);
  modbusTCPServer.configureHoldingRegisters(IP_ADDR_0_REG, IP_ADDR_LENGTH);

  modbusTCPServer.configureDiscreteInputs(ENABLE_COIL, 1);
  modbusTCPServer.configureInputRegisters(BRIGHTNESS_INPUT_REG, 1);
  modbusTCPServer.configureInputRegisters(COLOR_INPUT_REG, COLOR_INPUT_REG_LENGTH);

  modbusTCPServer.holdingRegisterWrite(UID_REG, UID);

  modbusTCPServer.holdingRegisterWrite(MAC_ADDR_0_REG, mac[0]);
  modbusTCPServer.holdingRegisterWrite(MAC_ADDR_1_REG, mac[1]);
  modbusTCPServer.holdingRegisterWrite(MAC_ADDR_2_REG, mac[2]);
  modbusTCPServer.holdingRegisterWrite(MAC_ADDR_3_REG, mac[3]);
  modbusTCPServer.holdingRegisterWrite(MAC_ADDR_4_REG, mac[4]);

  modbusTCPServer.holdingRegisterWrite(IP_ADDR_0_REG, ip[0]);
  modbusTCPServer.holdingRegisterWrite(IP_ADDR_1_REG, ip[1]);
  modbusTCPServer.holdingRegisterWrite(IP_ADDR_2_REG, ip[2]);
  modbusTCPServer.holdingRegisterWrite(IP_ADDR_3_REG, ip[3]);

  modbusTCPServer.inputRegisterWrite(BRIGHTNESS_INPUT_REG, 128);

  Ethernet.init(ENET_SS_PIN)
  Ethernet.begin(mac, ip);

  // Check for Ethernet hardware present
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    return;
  }
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
  }

  // start the server
  ethServer.begin();

  pixel.setBrightness(255);
  pixel.show();

}

void loop() {
  // put your main code here, to run repeatedly:
  if(!ethClient){
    ethClient = ethServer.available();
    if(!ethClient)
      return;
    modbusTCPServer.accept(ethClient);
  }

  if(ethClient.connected())
    modbusTCPServer.poll();
  else
    modbusTCPServer.end();

  
  if(modbusTCPServer.coilRead(ENABLE_COIL) != prev_en){
    prev_en = modbusTCPServer.coilRead(ENABLE_COIL);
    count++;
  }
  
  // only do something if there is a change
  if(modbusTCPServer.inputRegisterRead(BRIGHTNESS_INPUT_REG) != prev_brightness){ 
    prev_brightness = modbusTCPServer.inputRegisterRead(BRIGHTNESS_INPUT_REG);
    count++;
  }

  if(modbusTCPServer.inputRegisterRead(RED_INPUT_REG) != prev_r){
    prev_r = modbusTCPServer.inputRegisterRead(RED_INPUT_REG);
    count++;
  }
  else if(modbusTCPServer.inputRegisterRead(GREEN_INPUT_REG) != prev_g){
    prev_g = modbusTCPServer.inputRegisterRead(GREEN_INPUT_REG);
    count++;
  }
  else if(modbusTCPServer.inputRegisterRead(BLUE_INPUT_REG) != prev_b){
    prev_b = modbusTCPServer.inputRegisterRead(BLUE_INPUT_REG);
    count++;
  }

  if(count > 0){
    pixel.setPixelColor(0, prev_r, prev_g, prev_b);
    pixel.setBrightness(prev_brightness);
    if(!prev_en)
      pixel.setBrightness(0);
    pixel.show();
  }
  
}

