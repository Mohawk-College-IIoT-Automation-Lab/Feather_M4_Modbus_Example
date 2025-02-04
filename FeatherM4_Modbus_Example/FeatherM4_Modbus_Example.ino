#include <SPI.h>
#include <Adafruit_NeoPixel.h>
#include <Ethernet.h>

#include <TaskScheduler.h>

#include <ArduinoModbus.h>

#define NEO_PIXEL_PIN 8

#define MAX_NUM_CLIENTS 2 // Max num clients in 4

#define BASE_FREQ TASK_MS * 50 // 50ms -> 40Hz

#define MODBUS_SERVER_INT BASE_FREQ
#define MODBUS_CLIENT_INT BASE_FREQ / MAX_NUM_CLIENTS  // freq scales with the number of clients, more clients most go faster

#define ENET_SERVER_PORT 502

const uint8_t ip_addr[] = {192, 168, 1, 177}; 
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
}; // fix this mac
IPAddress ip((const uint8_t *)&ip_addr);

EthernetServer ethServer(ENET_SERVER_PORT);
ModbusTCPServer modbusTCPServer;

Scheduler runner;


typedef _server_data{
  uint8_t num_connected = 0, current_client = 0;
  EthernetClients clients[MAX_NUM_CLIENTS];
}ServerData_t;

void modbus_server_callback();
void modbus_client_callback();

void add_client();
void remove_client(uint8_t id);

Task modbus_server_task(MODBUS_SERVER_INT, TASK_FOREVER, &modbuser_serer_callback, &runner);
Task modbus_client_task(MODBUS_CLIENT_INT, TASK_FOREVER, &modbus_client_callback, &runner);

void setup() {
  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);

  // Check for Ethernet hardware present
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    while (true) {
      delay(1); // do nothing, no point running without Ethernet hardware
    }
  }
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
  }

  // start the server
  ethServer.begin();

  // start the Modbus TCP server
  if (!modbusTCPServer.begin()) {
    Serial.println("Failed to start Modbus TCP Server!");
    while (1);
  }

  // configure the LED
  pinMode(LIGHT_PIN, OUTPUT);
  digitalWrite(LIGHT_PIN, LOW);

  // configure a single coil at address 0x00
  modbusTCPServer.configureCoils(0x00, 1);



}

void loop() {
  // put your main code here, to run repeatedly:
  runner.execute();
}

void add_client(){
  return;
}

void remove_client(uint8_t id){
  // remove requested client
  /* 
  reduce list size to and ensure that all active ids are in the lowest
  possible spot in the array
  */
  return;
}

void modbus_server_callback(){
  // check if there are any clients waiting
  // assign the client to a location in the struct
  // enable the client task if num_clients > 1
  // if too many clients ignore and loop
  return;
}

void modbus_client_callback(){
  // check which client we are checking
  // is client still connected? if no close and remove
  // service request from client
  // 
  return;
}