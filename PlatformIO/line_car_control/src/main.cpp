#include <Arduino.h>
#include <Pixy2.h>

Pixy2 pixy;

#include "AdvancePixyVector.h"

char buf[128];

void setup()
{
    Serial.begin(115200);
    Serial.print("Starting...\n");

    pixy.init();
    Serial.println(pixy.changeProg("line"));
}

void setServo(int16_t position) {
    // pixy.setServos(uint16_t s0, s1) - uint16_t s0/s1 => 0 - 1000
    // Alamak servo = 350 - 650
    const int16_t SERVO_INPUT_MAX_MIN = 100;

    const uint16_t ALAMAK_SERVO_CENTER_COEF = 50;
    const uint16_t ALAMAK_SERVO_MIN = 350;
    const uint16_t ALAMAK_SERVO_MAX = 650;

    int16_t mapServoValue = map(position, -1 * SERVO_INPUT_MAX_MIN, SERVO_INPUT_MAX_MIN, ALAMAK_SERVO_MIN, ALAMAK_SERVO_MAX) - ALAMAK_SERVO_CENTER_COEF;
    int8_t servoError = pixy.setServos(mapServoValue, 500);

    // sprintf(buf, "Servo - position: %4d (%4d), status: %2d", position, mapServoValue, servoError);
    // Serial.println(buf);
}

const uint8_t PIXY_HEIGHT_IN_LINE_MODE = 51;
const uint8_t PIXY_WIDTH_IN_LINE_MODE = 78;

void loop()
{
    pixy.line.getMainFeatures();

    if (pixy.line.numVectors) {
        Vector* mainVector = pixy.line.vectors;
        AdvancePixyVector vector{mainVector};
        //vector.print();

        int linePositionX = mainVector->m_x0;
        int error = 0;
        int angle = 0;

        const int center = PIXY_HEIGHT_IN_LINE_MODE / 2;
        if(linePositionX < center) {
          // line on left side
          error = linePositionX;

          Serial.print("L: ");
          angle = vector.angleDegree() - 55; /* magic constant for left side - angle measured on straight forward track */
        } else {
          // line on right side
          error = linePositionX - PIXY_WIDTH_IN_LINE_MODE;

          Serial.print("R: ");
          angle = vector.angleDegree() - 125; /* magic constant for right side - angle measured on straight forward track */
        } 

        sprintf(buf, "%3d   angle: %4d   rawAngle: %4d\n", error, angle, vector.angleDegree());
        Serial.print(buf);
        
        const int8_t OFFSET_COEF = -25;
        int16_t servoPosition = (( - vector.angleDegree())) * -1 + OFFSET_COEF;
        setServo(servoPosition);
    }

    delay(50);
}
