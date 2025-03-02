#ifndef REG_MAP_H
#define REG_MAP_H

#include <Arduino.h>
#include <ArduinoModbus.h>

/* --- COILS --- */

/* --- DISCRETE INPUTS --- */
#define ENABLE_DS_INPUT 20001

/* --- INPUT REGISTERS --- */
#define BRIGHTNESS_INPUT_REG 30001

#define RED_INPUT_REG 30002
#define GREEN_INPUT_REG 30003
#define BLUE_INPUT_REG 30004
#define COLOR_INPUT_REG RED_INPUT_REG
#define COLOR_INPUT_REG_LENGTH 3

/* --- HOLDING REGISTERS --- */
#define UID_REG 4000

#define MAC_ADDR_0_REG 40002
#define MAC_ADDR_1_REG 40003
#define MAC_ADDR_2_REG 40004
#define MAC_ADDR_3_REG 40005
#define MAC_ADDR_4_REG 40006
#define MAC_ADDR_REG MAC_ADDR_0_REG
#define MAC_ADDR_LENGTH 5

#define IP_ADDR_0_REG 40007
#define IP_ADDR_1_REG 40008
#define IP_ADDR_2_REG 40009
#define IP_ADDR_3_REG 40010
#define IP_ADDR_REG IP_ADDR_0_REG
#define IP_ADDR_LENGTH 4


#endif