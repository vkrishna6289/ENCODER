#include <Arduino.h>
#include "stepangle.h"

// ---------------- Pin Definitions ----------------
#define STEP_PIN      2
#define DIR_PIN       3
#define ENABLE_PIN    4
#define LIMIT_PIN     8
#define ESTOP_PIN     7
#define POT_PIN       A0

// Create the AdvancedStepper object
AdvancedStepper motor(
    STEP_PIN,
    DIR_PIN,
    ENABLE_PIN,
    LIMIT_PIN,
    ESTOP_PIN,
    POT_PIN
);

void setup()
{
    Serial.begin(115200);

    // Initialize the motor
    motor.begin();

    // Configure motion parameters
    motor.setMaxSpeed(3000);
    motor.setAcceleration(1500);

    // Enable the motor driver
    motor.enable();

    Serial.println("--------------------------------");
    Serial.println(" Advanced Stepper Controller");
    Serial.println("--------------------------------");
    Serial.println("Commands:");
    Serial.println("HOME");
    Serial.println("POS 1000");
    Serial.println("POS -500");
    Serial.println("CW");
    Serial.println("CCW");
    Serial.println("STOP");
    Serial.println("POT");
    Serial.println("--------------------------------");

    // Optional: Home the motor on startup
    motor.home();
}

void loop()
{
    // Update the stepper controller
    motor.update();

    // Process incoming serial commands
    motor.processSerial();

    // Example: check emergency stop
    if (motor.emergencyStopPressed())
    {
        Serial.println("Emergency Stop Activated!");
    }

    // Add your application code here if needed
}