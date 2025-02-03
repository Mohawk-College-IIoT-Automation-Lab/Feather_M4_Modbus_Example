#define _TASK_THREAD_SAFE
#include <TaskScheduler.h>

#include <ArduinoModbus.h>

#include <SPI.h>
#include <Adafruit_NeoPixel.h>
#include <Ethernet.h>

#define LIGHT_PIN 1
#define BUTTON_PIN 2

#define BASE_TASK_FREQ TASK_MS * 10 // 20 ms -> 50Hz

#define MODBUS_SERVER_INT BASE_TASK_FREQ * 2
#define IO_DAEMON_INT BASE_TASK_FREQ 

#define ENET_SERVER_PORT 502

const uint8_t ip_addr[] = {192, 168, 1, 177}; 
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
}; // fix this mac
IPAddress ip((const uint8_t *)&ip_addr);

EthernetServer ethServer(ENET_SERVER_PORT);
ModbusTCPServer modbusTCPServer;

Scheduler runner;

void modbuser_serer_callback();

Task modbus_server_task(MODBUS_SERVER_INT, TASK_FOREVER, &modbuser_serer_callback, &runner);
Task io_daemon_task(IO_DAEMON_INT)

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

}
