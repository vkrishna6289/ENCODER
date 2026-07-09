#include <AccelStepper.h>

// ----------------------- Pins -----------------------
#define STEP_PIN    2
#define DIR_PIN     3
#define ENABLE_PIN  4
#define LIMIT_PIN   8
#define ESTOP_PIN   7
#define POT_PIN     A0

// ----------------------- Stepper -----------------------
AccelStepper stepper(AccelStepper::DRIVER, STEP_PIN, DIR_PIN);

// ----------------------- Variables -----------------------
bool homed = false;
bool continuousMode = false;

long targetPosition = 0;

float maxSpeed = 3000;
float acceleration = 1500;

unsigned long lastPrint = 0;

// --------------------------------------------------------
void setup()
{
    Serial.begin(115200);

    pinMode(ENABLE_PIN, OUTPUT);
    pinMode(LIMIT_PIN, INPUT_PULLUP);
    pinMode(ESTOP_PIN, INPUT_PULLUP);

    digitalWrite(ENABLE_PIN, LOW);   // Enable driver

    stepper.setMaxSpeed(maxSpeed);
    stepper.setAcceleration(acceleration);

    Serial.println("=================================");
    Serial.println("Advanced Stepper Controller Ready");
    Serial.println("Commands:");
    Serial.println("HOME");
    Serial.println("POS xxxx");
    Serial.println("CW");
    Serial.println("CCW");
    Serial.println("STOP");
    Serial.println("POT");
    Serial.println("=================================");
}

// --------------------------------------------------------
void loop()
{
    emergencyStop();

    serialCommands();

    if (!homed)
    {
        homeMotor();
        return;
    }

    if (continuousMode)
    {
        int pot = analogRead(POT_PIN);

        float speed = map(pot, 0, 1023, -maxSpeed, maxSpeed);

        stepper.setSpeed(speed);

        stepper.runSpeed();
    }
    else
    {
        stepper.run();
    }

    statusPrint();
}

// --------------------------------------------------------
void homeMotor()
{
    stepper.setMaxSpeed(800);
    stepper.setSpeed(-300);

    while (digitalRead(LIMIT_PIN))
    {
        emergencyStop();
        stepper.runSpeed();
    }

    stepper.stop();

    stepper.setCurrentPosition(0);

    stepper.setMaxSpeed(maxSpeed);
    stepper.setAcceleration(acceleration);

    homed = true;

    Serial.println("Homing Complete.");
}

// --------------------------------------------------------
void emergencyStop()
{
    if (!digitalRead(ESTOP_PIN))
    {
        stepper.stop();

        while (!digitalRead(ESTOP_PIN));

        Serial.println("Emergency Stop Activated");
    }
}

// --------------------------------------------------------
void serialCommands()
{
    if (!Serial.available())
        return;

    String cmd = Serial.readStringUntil('\n');

    cmd.trim();

    if (cmd == "HOME")
    {
        homed = false;
    }

    else if (cmd.startsWith("POS"))
    {
        targetPosition = cmd.substring(4).toInt();

        continuousMode = false;

        stepper.moveTo(targetPosition);

        Serial.print("Moving to ");
        Serial.println(targetPosition);
    }

    else if (cmd == "CW")
    {
        continuousMode = true;

        stepper.setSpeed(1000);

        Serial.println("Continuous Clockwise");
    }

    else if (cmd == "CCW")
    {
        continuousMode = true;

        stepper.setSpeed(-1000);

        Serial.println("Continuous Counter Clockwise");
    }

    else if (cmd == "STOP")
    {
        continuousMode = false;

        stepper.stop();

        Serial.println("Motor Stopped");
    }

    else if (cmd == "POT")
    {
        continuousMode = true;

        Serial.println("Potentiometer Speed Control Enabled");
    }
}

// --------------------------------------------------------
void statusPrint()
{
    if (millis() - lastPrint > 500)
    {
        lastPrint = millis();

        Serial.print("Position : ");
        Serial.print(stepper.currentPosition());

        Serial.print("   Distance : ");
        Serial.print(stepper.distanceToGo());

        Serial.print("   Speed : ");
        Serial.println(stepper.speed());
    }
}