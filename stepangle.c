#ifndef ADVANCED_STEPPER_H
#define ADVANCED_STEPPER_H

#include <Arduino.h>
#include <AccelStepper.h>

class AdvancedStepper
{
public:
    // Constructor
    AdvancedStepper(uint8_t stepPin,
                    uint8_t dirPin,
                    uint8_t enablePin,
                    uint8_t limitPin,
                    uint8_t estopPin,
                    uint8_t potPin);

    // Initialization
    void begin();

    // Call repeatedly inside loop()
    void update();

    // Motion
    void moveTo(long position);
    void move(long steps);
    void home();
    void stop();

    // Continuous rotation
    void rotateCW(float speed);
    void rotateCCW(float speed);

    // Driver control
    void enable();
    void disable();

    // Configuration
    void setMaxSpeed(float speed);
    void setAcceleration(float accel);

    // Status
    long currentPosition();
    long targetPosition();
    long distanceToGo();
    float speed();

    // Serial Interface
    void processSerial();

    // Emergency Stop
    bool emergencyStopPressed();

private:

    // Pin numbers
    uint8_t _stepPin;
    uint8_t _dirPin;
    uint8_t _enablePin;
    uint8_t _limitPin;
    uint8_t _estopPin;
    uint8_t _potPin;

    // AccelStepper object
    AccelStepper _stepper;

    // Motion parameters
    float _maxSpeed;
    float _acceleration;

    // States
    bool _homed;
    bool _continuousMode;
    bool _enabled;

    long _targetPosition;

    unsigned long _lastStatusTime;

    // Internal functions
    void runContinuous();
    void checkLimitSwitch();
    void checkEmergencyStop();
    void printStatus();
};

#endif