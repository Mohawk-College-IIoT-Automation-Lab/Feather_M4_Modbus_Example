#include <SPI.h>
#include <Adafruit_NeoPixel.h>
#include <Ethernet.h>

#include <ArduinoModbus.h>
#include "REG_MAP.h"

#define DEBUG
#include "Debug.h"

/* --- CONSTANTS --- */
#define NEO_PIXEL_PIN 8
#define ENET_SS_PIN 10
#define MODBUS_SERVER_PORT 502

const uint8_t UID = 1;
const uint8_t ip_addr[] = {192, 168, 1, 177}; 
const uint8_t mac_addr[] = {
  0x98, 0x76, 0xB6, 0x12, 0xF8, 0xDC
}; // fix this mac

/* --- VARIABLES & OBJECTS --- */
IPAddress ip((const uint8_t *)&ip_addr);
EthernetServer ethServer(MODBUS_SERVER_PORT);
EthernetClient ethClient;
ModbusTCPServer modbusTCPServer;

Adafruit_NeoPixel pixel(1, NEO_PIXEL_PIN, NEO_GRB + NEO_KHZ800);

uint8_t prev_brightness = 0, prev_r = 0, prev_g = 0, prev_b = 0, prev_en = 0, count = 0, temp = 0;

/* --- FUNCTION PROTOTYPES --- */
/**
  * setupModbus helper functions
  *
  * @param m_server modbus server object, ModbusTCPServer object passed as a pointer
  * @param UID device's Unique ID, uint8_t
  * @param mac device's mac address, uint8_t[5]
  * @param ip device's static ip address, uint8_t[4]
  */
uint8_t setupModbus(ModbusTCPServer * m_server, uint8_t UID, uint8_t mac[5], uint8_t ip[4]);

/**
  * setupEthernet helper functions
  *
  * @param e_server ethernet server object, EthernetServer object passed as a pointer
  * @param mac device's mac address, uint8_t[5]
  * @param ip device's static ip address, IPAddress object
  * @param ss_pin the slave select pin for the ethernet phy, uint8_t
  */
uint8_t setupEthernet(EthernetServer *e_server, uint8_t mac[5], IPAddress ip, uint8_t ss_pin);


void setup() {
  // enable the neopixel
  pixel.begin();
  pixel.setPixelColor(0, 255, 0, 0);    // set the pixek to red
  pixel.setBrightness(64);              // turn it to 1/4 brightness
  pixel.show();                         // turn on the neopixel

  Serial.begin(9600);
    while(!Serial){ }
  
  // start the Modbus TCP server, if it fails to initialize kill the application by waiting forever
  if(!setupModbus(&modbusTCPServer, UID, (uint8_t*)mac_addr, (uint8_t*)ip_addr)){
    DEBUG_PRINTLN("Failed to init Modbus TCP Server");
    while(1){ }
  }
  DEBUG_PRINTLN("Successfully init Modbus TCP Server")

  // start the Ethernet hardware and ethernet server, if it fails to initialize kill the app by waiting forever
  if(!setupEthernet(&ethServer, (uint8_t*) mac_addr, ip, ENET_SS_PIN)){
    DEBUG_PRINTLN("Failed to init Ethernet");
    while(1){ }
  }
  DEBUG_PRINTLN("Successfully init Ethernet");

  pixel.setBrightness(255);                 // set the pixel brightness to 100%
  pixel.show();                             // update the pixel

  DEBUG_PRINTLN("Successfully init the device");

}

void loop() {

  // check if the ethernet client has been initialized or if it's not connected
  ethClient = ethServer.available(); // accept an available client 
  DEBUG_PRINTLN("Tried to accept a client on Etherenet Server");

  // if the ethernet client has been initialized
  if(ethClient){ 
    DEBUG_PRINTLN("EthernetClient available");
    
    modbusTCPServer.accept(ethClient);    // accept the new connection
    DEBUG_PRINTLN("Modbus Server accepted client");
    
    DEBUG_PRINT("Ethernet Client: "); DEBUG_PRINTLN(ethClient.connected());

    // execute the following loop while the device remains connected to the client
    while(ethClient.connected()){

      // poll the modbus server, returns true if a change has been made         
      if(modbusTCPServer.poll()){
        // Any time there is a change the device will read each of the registers that it has, this is not efficient if there are many registers
        DEBUG_PRINTLN("Modbus server poll success");

        // The statement below checks if the discrete input for enable has been set and writes the output based on that value,
        // if it's 0 (false), it will write 0 brightness, if 1 (true) it will read the brightness input register
        pixel.setBrightness((uint8_t) (!modbusTCPServer.discreteInputRead(ENABLE_DS_INPUT)? 0 : modbusTCPServer.inputRegisterRead(BRIGHTNESS_INPUT_REG)) );

        // read the r, g, b channels from the input registers and set the pixel colour
        pixel.setPixelColor(0, (uint8_t) modbusTCPServer.inputRegisterRead(RED_INPUT_REG), (uint8_t) modbusTCPServer.inputRegisterRead(GREEN_INPUT_REG), 
                                (uint8_t) modbusTCPServer.inputRegisterRead(BLUE_INPUT_REG));
        pixel.show(); // show the updates

      }
      else{ // if the ethernet client is disconnected
        delay(10); // delay 10 ms
        DEBUG_PRINTLN("No changes made via Modbus");
      }
    }   
    // when the ethernet client has disconnected
    ethClient.stop(); // stop the client
    DEBUG_PRINTLN("Client disconnected");
  }
}

/* --- FUNCTION DEFS --- */
uint8_t setupModbus(ModbusTCPServer * m_server, uint8_t UID, uint8_t mac[5], uint8_t ip[4]){
  // try to begin the modbus server
  if (!m_server->begin())  
    return 0; // returns 0 if it failes

  // configure discrete inputs
  m_server->configureDiscreteInputs(ENABLE_DS_INPUT, 1);

  // configure input registers
  m_server->configureInputRegisters(BRIGHTNESS_INPUT_REG, 1);
  m_server->configureInputRegisters(COLOR_INPUT_REG, COLOR_INPUT_REG_LENGTH);

  // configure holding registers
  m_server->configureHoldingRegisters(UID_REG, 1);
  m_server->configureHoldingRegisters(MAC_ADDR_0_REG, MAC_ADDR_LENGTH);
  m_server->configureHoldingRegisters(IP_ADDR_0_REG, IP_ADDR_LENGTH);

  // write default values to holding registers
  m_server->holdingRegisterWrite(UID_REG, UID);

  m_server->holdingRegisterWrite(MAC_ADDR_0_REG, mac[0]);
  m_server->holdingRegisterWrite(MAC_ADDR_1_REG, mac[1]);
  m_server->holdingRegisterWrite(MAC_ADDR_2_REG, mac[2]);
  m_server->holdingRegisterWrite(MAC_ADDR_3_REG, mac[3]);
  m_server->holdingRegisterWrite(MAC_ADDR_4_REG, mac[4]);

  m_server->holdingRegisterWrite(IP_ADDR_0_REG, ip[0]);
  m_server->holdingRegisterWrite(IP_ADDR_1_REG, ip[1]);
  m_server->holdingRegisterWrite(IP_ADDR_2_REG, ip[2]);
  m_server->holdingRegisterWrite(IP_ADDR_3_REG, ip[3]);

  // write default values to input registers
  m_server->inputRegisterWrite(BRIGHTNESS_INPUT_REG, 128);

  return 1;
}

uint8_t setupEthernet(EthernetServer *e_server, uint8_t mac[5], IPAddress ip, uint8_t ss_pin){
  Ethernet.init(ENET_SS_PIN);                 // init the ethernet hardware
  Ethernet.begin((uint8_t*)mac_addr, ip);     // set the mac and static ip for the ethernet harwdare and begin

  // Check for Ethernet hardware present
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    DEBUG_PRINTLN("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    return 0;
  }

  // check for Ethernet link status
  if (Ethernet.linkStatus() == LinkOFF) {
    DEBUG_PRINTLN("Ethernet cable is not connected.");
    return 0;
  }

  // start the server
  ethServer.begin();

  return 1;
}


