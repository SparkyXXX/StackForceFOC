/*
 * @Project: Infantry Code
 * @Author: GDDG08
 * @Date: 2024-10-02 11:11:36
 * @LastEditors: SparkyXXX
 * @LastEditTime: 2024-10-04 21:47:15
 */

#include "SF_BLDC.h"
#include <Arduino.h>
#include <ESP32Servo.h>
#include <algorithm>

// #define DEBUG

SF_BLDC motors = SF_BLDC(Serial2);
SF_BLDC_DATA driver;
Servo myServo;
String inputString = "";
const int servoPin = 1;
float wheel_spd = 0.0;
float servo_pos = 0.0;

bool validatePacket(String packet)
{
    int commaCount = 0;
    for (int i = 0; i < packet.length(); i++)
    {
        if (packet.charAt(i) == ',')
        {
            commaCount++;
        }
    }
    if (commaCount != 3)
    {
        return false;
    }

    if (packet.startsWith("H") && packet.endsWith("X"))
    {
        int firstCommaIndex = packet.indexOf(',');
        int secondCommaIndex = packet.indexOf(',', firstCommaIndex + 1);
        int thirdCommaIndex = packet.indexOf(',', secondCommaIndex + 1);

        if (firstCommaIndex != -1 && secondCommaIndex != -1 && thirdCommaIndex != -1 && thirdCommaIndex < packet.length() - 1)
        {
            return true;
        }
    }
    return false;
}

void parsePacket(String packet)
{
    int firstCommaIndex = packet.indexOf(',');
    int secondCommaIndex = packet.indexOf(',', firstCommaIndex + 1);
    int thirdCommaIndex = packet.indexOf(',', secondCommaIndex + 1);
    wheel_spd = packet.substring(firstCommaIndex + 1, secondCommaIndex).toFloat();
    servo_pos = packet.substring(secondCommaIndex + 1, thirdCommaIndex).toFloat();
#ifdef DEBUG
    Serial.print("Parsed wheel_spd: ");
    Serial.println(wheel_spd);
    Serial.print("Parsed servo_pos: ");
    Serial.println(servo_pos);
#endif
}

void setup()
{
    Serial.begin(115200);
    myServo.attach(servoPin);
    motors.init();
    motors.setModes(4, 4);
    delay(1000);
}

void loop()
{
    while (Serial.available())
    {
        char inChar = (char)Serial.read();
        inputString += inChar;

        if (inChar == 'X')
        {
#ifdef DEBUG
            Serial.print("Received packet: ");
            Serial.println(inputString);
#endif
            if (validatePacket(inputString))
            {
                parsePacket(inputString);
            }
            else
            {
                Serial.println("Invalid packet received!");
            }
            inputString = "";
        }
    }
    // driver = motors.getBLDCData();
    // Serial.printf("%f,%f\n", driver.M0_speed, driver.M1_speed);
    wheel_spd = std::max(-2.0f, std::min(wheel_spd, 2.0f));
    servo_pos = std::max(60.0f, std::min(servo_pos, 120.0f));
    myServo.write(servo_pos);
    delay(15);
    motors.setTargets(wheel_spd, wheel_spd);
}
