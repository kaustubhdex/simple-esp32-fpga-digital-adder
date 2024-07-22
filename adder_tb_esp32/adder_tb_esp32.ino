/*
    adder logic implemented in fpga, digital read/write done by esp32

    adder implemented on fpga adder ckt: input: a_in, b_in, c_in - P5: 1,3,5 (FPGA_IOR2A, FPGA_IOR11A, FPGA_IOR15A) output: sum, carry - P5: 2, 4 (FPGA_IOR2B, FPGA_IOR11B)

    adder tb on esp32 tb ckt: input: sum, carry - P6: 2,4 (GPIO 21,17) output: a_in, b_in, c_in - P6: 1,3,5 (GPIO 18, 16, 12) 
 */

#include "Arduino.h"
#include "Wire.h"
#include "XPowersLib.h" //https://github.com/lewisxhe/XPowersLib

XPowersAXP2101 PMU;

#pragma once

#define PIN_BTN      0

#define PIN_IIC_SDA  38
#define PIN_IIC_SCL  39
#define PIN_PMU_IRQ  40

#define PIN_LED      46

#define PIN_FPGA_CS  1
#define PIN_FPGA_SCK 2
#define PIN_FPGA_D0  3
#define PIN_FPGA_D1  5
#define PIN_FPGA_D2  6
#define PIN_FPGA_D3  4
// outputs to be tested
#define a_in 18
#define b_in 16
#define c_in 12

// inputs to be read

#define sum 21
#define carry 17

bool sum_t;
bool carry_t;

int serial_int;
String serial_str;
void led_task(void *param);

void setup()
{
  pinMode(a_in, OUTPUT);
  pinMode(b_in, OUTPUT);
  pinMode(c_in, OUTPUT);

  
    Serial.begin(115200);
    Serial.println("Hello T-FPGA-CORE");
    xTaskCreatePinnedToCore(led_task, "led_task", 1024, NULL, 1, NULL, 1);

    bool result = PMU.begin(Wire, AXP2101_SLAVE_ADDRESS, PIN_IIC_SDA, PIN_IIC_SCL);

    if (result == false) {
        Serial.println("PMU is not online...");
        while (1)
            delay(50);
    }

    PMU.setDC4Voltage(1200);   // Here is the FPGA core voltage. Careful review of the manual is required before modification.
    PMU.setALDO1Voltage(3300); // BANK0 area voltage
    PMU.setALDO2Voltage(3300); // BANK1 area voltage
    PMU.setALDO3Voltage(2500); // BANK2 area voltage
    PMU.setALDO4Voltage(1800); // BANK3 area voltage

    PMU.enableALDO1();
    PMU.enableALDO2();
    PMU.enableALDO3();
    PMU.enableALDO4();
}

void loop()
{
    if (Serial.available())
    {
      serial_int = Serial.read();
      serial_str = String(serial_int);
      if (serial_str == "a_in=0")
      {
        digitalWrite(a_in,false);
      }
      if (serial_str == "a_in=1")
      {
        digitalWrite(a_in,true);
      }
      if (serial_str == "b_in=0")
      {
        digitalWrite(b_in,false);
      }
      if (serial_str == "b_in=1")
      {
        digitalWrite(b_in,true);
      }
      if (serial_str == "c_in=0")
      {
        digitalWrite(c_in,false);
      }
      if (serial_str == "c_in=1")
      {
        digitalWrite(c_in,true);
      }
    }
}

void led_task(void *param)
{
    pinMode(sum, INPUT);
    pinMode(carry, INPUT);
    while (true) {
        sum_t = digitalRead(sum);
        carry_t= digitalRead(carry);
        delay(5);
    }
}
