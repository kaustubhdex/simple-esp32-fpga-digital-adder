
/*
    adder logic implemented in fpga, digital read/write done by esp32

    adder implemented on fpga adder ckt: input: a_in, b_in, c_in - P5: 1,3,5 (FPGA_IOR2A, FPGA_IOR11A, FPGA_IOR15A) output: sum, carry - P5: 2, 4 (FPGA_IOR2B, FPGA_IOR11B)

    adder tb on esp32 tb ckt: input: sum, carry - P6: 2,4 (GPIO 21,17) output: a_in, b_in, c_in - P6: 1,3,5 (GPIO 18, 16, 12) 

    AXP2101 is used for power mgmt for FPGA and ESP32. 

    One blinking LED given on second core for recognising active status.

    Simulated using accurate timing adjustments via millis()
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

// Timing variables for LED 1
unsigned long previousMillis1 = 0;
const long interval1 = 1000; // LED 1 blinks every 1 second

// Timing variables for LED 2
unsigned long previousMillis2 = 0;
const long interval2 = 500;  // LED 2 blinks every 0.5 seconds

// Timing variables for LED 3
unsigned long previousMillis3 = 0;
const long interval3 = 2000; // LED 3 blinks every 2 seconds\

// LED states
int ledState1 = LOW;
int ledState2 = LOW;
int ledState3 = LOW;

unsigned long previousMillis = 0; // Store the last time the output was updated
const long interval = 10000; // 10 seconds interval

int state = 0; // Initialize state variable

void setup()
{
  pinMode(a_in, OUTPUT);
  pinMode(b_in, OUTPUT);
  pinMode(c_in, OUTPUT);
  pinMode(sum, INPUT);
  pinMode(carry, INPUT);
    Serial.begin(115200);
    Serial.println("Hello T-FPGA-CORE");
    Serial.println("Initialising adder testbench...");
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

  unsigned long currentMillis = millis(); // Get the current time

  // LED 1 control
  if (currentMillis - previousMillis1 >= interval1) {
    previousMillis1 = currentMillis; // Save the current time
    // Toggle LED 1
    ledState1 = !ledState1; // Toggle the state
    //Serial.println("ls 1 = " + String(ledState1));
    if (ledState1) {
      digitalWrite(a_in, HIGH); // Turn LED 1 ON
    } else {
      digitalWrite(a_in, LOW);  // Turn LED 1 OFF
    }
  }

  // LED 2 control
  if (currentMillis - previousMillis2 >= interval2) {
    previousMillis2 = currentMillis; // Save the current time
    // Toggle LED 2
    ledState2 = !ledState2; // Toggle the state
    //Serial.println("ls 2 = " + String(ledState2));
    if (ledState2) {
      digitalWrite(b_in, HIGH); // Turn LED 2 ON
    } else {
      digitalWrite(b_in, LOW);  // Turn LED 2 OFF
    }
    sum_t = digitalRead(sum);
        carry_t= digitalRead(carry);
        delay(100);
        delay(100);
        Serial.println("ls 1 = " + String(ledState1));
        Serial.println("ls 2 = " + String(ledState2));
        Serial.println("ls 3 = " + String(ledState3));
        Serial.println("sum = " + String(sum_t));
        Serial.println("carry = " + String(carry_t));
  }

  // LED 3 control
  if (currentMillis - previousMillis3 >= interval3) {
    previousMillis3 = currentMillis; // Save the current time
    // Toggle LED 3
    ledState3 = !ledState3; // Toggle the state
    //Serial.println("ls 3 = " + String(ledState3));
    if (ledState3) {
      digitalWrite(c_in, HIGH); // Turn LED 3 ON
    } else {
      digitalWrite(c_in, LOW);  // Turn LED 3 OFF
    }
  }
//  
}

void led_task(void *param)
{
    
    pinMode(PIN_LED, OUTPUT);
    
    while (true) {
        digitalWrite(PIN_LED, 1);
        delay(100);
        digitalWrite(PIN_LED, 0);
        
        delay(100);
    }
}
