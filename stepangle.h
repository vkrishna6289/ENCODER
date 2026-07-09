#ifndef ADVANCED_STEPPER_H
#define ADVANCED_STEPPER_H

#include <Arduino.h>
#include <AccelStepper.h>

class AdvancedStepper
{
public:

    // Constructor
    AdvancedStepper(
        uint8_t stepPin,
        uint8_t dirPin,
        uint8_t enablePin,
        uint8_t limitPin,
        uint8_t estopPin,
        uint8_t potPin
    );

    // Initialization
    void begin();

    // Main update function (call repeatedly in loop)
    void update();

    // Motion commands
    void moveTo(long position);
    void moveRelative(long steps);

    void rotateCW(float speed);
    void rotateCCW(float speed);

    void stop();

    // Homing
    void home();

    // Speed settings
    void setMaxSpeed(float speed);
    void setAcceleration(float accel);

    // Driver enable
    void enableMotor();
    void disableMotor();

    // Status
    long currentPosition();
    long distanceToGo();
    float currentSpeed();

    // Serial command parser
    void processSerial();

    // Emergency stop
    bool emergencyPressed();

private:

    // Pins
    uint8_t _stepPin;
    uint8_t _dirPin;
    uint8_t _enablePin;
    uint8_t _limitPin;
    uint8_t _estopPin;
    uint8_t _potPin;

    // Stepper object
    AccelStepper _stepper;

    // Variables
    bool _homed;
    bool _continuousMode;

    float _maxSpeed;
    float _acceleration;

    long _targetPosition;

    unsigned long _lastPrint;

    // Internal helper functions
    void handleEmergencyStop();
    void handleStatusPrint();
    void handleContinuousMode();
};

#endif