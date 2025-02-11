# Feather_M4_Modbus_Example
This example is built using the [Adafruit Feather M4 Express](https://www.adafruit.com/product/3857), [Adafruit Ethener Featherwing](https://www.adafruit.com/product/3201) and the built-in NeoPixel. This aim of this project is to create a modbus reference design using common and readily available hardware, software and libraries.

### Flow Chart 
![[Modbus_Neopixel_Demo_Flow_Chart.png]]

### Modbus Reg Map

##### Coils (0xxxx)

##### Discrete Inputs (1xxxx)

| Reg    | Addr  | Description |     |
| :----- | ----- | ----------- | --- |
| Enable | 20001 | Enable bit  |     |

##### Input Registers (3xxxx)

| Reg        | Addr        | Description    |
| :--------- | ----------- | -------------- |
| Brightness | 30001       | Brightness     |
| Color_Chan | 30002-30004 | Color Channels |
| Red        | 30002       | Red Channel    |
| Green      | 30003       | Green Channel  |
| Blue       | 30004       | Blue Channel   |

##### Holding Registers (4xxxx)

| Reg            | Addr         | Description |
| :------------- | ------------ | ----------- |
| UID            | 40001        | Unique ID   |
| MAC_ADDR (0:5) | 40002-40006  | MAC Address |
| MAC_ADDR_0     | 40002        | $MAC_0$     |
| MAC_ADDR_1     | 40003        | $MAC_1$     |
| MAC_ADDR_2     | 40004        | $MAC_2$     |
| MAC_ADDR_3     | 40005        | $MAC_3$     |
| MAC_ADDR_4     | 40006        | $MAC_4$     |
| IP_ADDR (0:4)  | 40007- 40010 | IP Address  |
| IP_ADDR_0      | 40007        | $IP_0$      |
| IP_ADDR_1      | 40008        | $IP_1$      |
| IP_ADDR_2      | 40009        | $IP_2$      |
| IP_ADDR_3      | 40010        | $IP_3$      |

#### Parts
- [Feather M4 Express](https://www.adafruit.com/product/3857)
- [Featherwing Ethernet](https://www.adafruit.com/product/3201)

#### Documents & Setup
- [FeatherM4 Getting Started](https://learn.adafruit.com/adafruit-feather-m4-express-atsamd51)
	- [Adding the board to Arduino IDE](https://learn.adafruit.com/adafruit-feather-m4-express-atsamd51/setup)
	- Install the Adafruit SAMD boards in the board explorer 
	- Select Adafruit Feather M4 Express and allow Arduino IDE install the SAMD Boards
	- Libraries to install
		- [Ethernet](https://docs.arduino.cc/libraries/ethernet/#Usage/Examples)
		- [Modbus](https://docs.arduino.cc/libraries/arduinomodbus/#Usage/Examples)
		- [Adafruit NeoPixel](https://github.com/adafruit/Adafruit_NeoPixel)
	- [Modbus Example from Arduino](https://github.com/arduino-libraries/ArduinoModbus/blob/master/examples/TCP/EthernetModbusServerLED/EthernetModbusServerLED.ino)
	- [Modicon Modbus Docs](https://modbus.org/docs/PI_MBUS_300.pdf)
	- [Modbus Data Address Format](https://www.fernhillsoftware.com/help/drivers/modbus/data-address-format.html)
- [Python & Tkinter Modbus GUI Example](https://github.com/Mohawk-College-IIoT-Automation-Lab/TKinter-Modbus-Example)

