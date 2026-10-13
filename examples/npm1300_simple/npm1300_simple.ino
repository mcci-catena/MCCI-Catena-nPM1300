/*

Module: npm1300_simple.ino

Function:
        Simple example nPM1300 PMIC module.

Copyright and License:
        See accompanying LICENSE file.

Author:
        Pranau Ravikumar, MCCI Corporation   June 2025

*/

#include <Arduino.h>
#include <Wire.h>
#include <MCCI_Catena_nPM1300.h>

/****************************************************************************\
|
|   Manifest constants & typedefs.
|
\****************************************************************************/

using namespace McciCatenaNpm1300;

/****************************************************************************\
|
|   Variables.
|
\****************************************************************************/

McciCatenaNpm1300::cNPM1300 gNpm1300 {Wire};

/****************************************************************************\
|
|   Code.
|
\****************************************************************************/

void setup()
    {
    Wire.begin();
    Serial.begin(9600);

    // wait for USB to be attached.
    while (! Serial)
        yield();

    Serial.println("nPM1300 Simple Test");

    if (!gNpm1300.begin())
        {
        Serial.println("Failed to initialize nPM1300!");
        delay(1000);
        while (true);
        }

    gNpm1300.setVterm(cNPM1300::Vterm::V4_15);
    gNpm1300.ignoreNtc(true); // mandatory, if you don't have a ntc connected!
    gNpm1300.setChargeCurrent(400);
    }

void loop()
    {
    gNpm1300.setVterm(cNPM1300::Vterm::V4_15);

    float vBat = gNpm1300.measureVbat();
    Serial.print("Battery Voltage: ");
    Serial.println(vBat, 3);

    float temperature = gNpm1300.measureTemperature();
    Serial.print("Temperature: ");
    Serial.println(temperature);

    float vSys = gNpm1300.measureVsys();
    Serial.print("System Voltage: ");
    Serial.println(vSys);

    float vBus = gNpm1300.measureVbus();
    Serial.print("Bus Voltage: ");
    Serial.println(vBus);

    uint8_t vBusStatus;
    if (gNpm1300.getVbusStatus(vBusStatus))
        {
        Serial.print("Bus Status: ");
        Serial.println(vBusStatus);
        }
    else
        {
        Serial.print("getVbusStatus() failed: ");
        Serial.println(gNpm1300.getLastErrorName());
        }

    Serial.println();
    Serial.println("##################################");
    Serial.println();
    delay (1000);
    }