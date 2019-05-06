#include <Arduino.h>
#include <Pixy2.h>

Pixy2 pixy;

#include "AdvancePixyVector.h"
#include "Atoms\numeric\vector.h"

char buf[128];

void setup()
{
  Serial.begin(115200);
  Serial.print("Starting...\n");

  pixy.init();
  // change to the line_tracking program.  Note, changeProg can use partial strings, so for example,
  // you can change to the line_tracking program by calling changeProg("line") instead of the whole
  // string changeProg("line_tracking")
  Serial.println(pixy.changeProg("line"));
}

// atoms::Vector2<int> PixyVectorToAtomVector2(::Vector *inputPixyVector) {
//     int x = inputPixyVector->m_x1 - inputPixyVector->m_x0;
//     int y = inputPixyVector->m_y1 - inputPixyVector->m_y0;
//     int temp[2] = {x, y};
//     atoms::Vector2<int> outputAtomsVector{temp};
//     return outputAtomsVector;
// }

void setServo(int16_t position) {
    // pixy.setServos(uint16_t s0, s1) - uint16_t s0/s1 => 0 - 1000
    // Alamak servo = 350 - 650
    const int16_t SERVO_INPUT_MAX_MIN = 100;
    
    const uint16_t ALAMAK_SERVO_CENTER_COEF = 50;
    const uint16_t ALAMAK_SERVO_MIN = 350;
    const uint16_t ALAMAK_SERVO_MAX = 650;
  
    int16_t mapServoValue = map(position, -1 * SERVO_INPUT_MAX_MIN, SERVO_INPUT_MAX_MIN, ALAMAK_SERVO_MIN, ALAMAK_SERVO_MAX) - ALAMAK_SERVO_CENTER_COEF;
    int8_t servoError = pixy.setServos(mapServoValue, 500);

    sprintf(buf, "Servo - position: %4d (%4d), status: %2d", position, mapServoValue, servoError);
    Serial.println(buf);
}

int16_t servoPosition = 0;
void loop()
{
   pixy.line.getMainFeatures();

  // pixy.line.vectors
  if (pixy.line.numVectors) {
    Vector* mainVector = pixy.line.vectors;
    //atoms::Vector2<int> vec = PixyVectorToAtomVector2(mainVector); 
    AdvancePixyVector vector{mainVector};
    vector.print();

    // servoPosition += 10;
    // if(servoPosition > 100) {
    //   servoPosition = -100;
    // }
    int8_t OFFSET_COEF = -25;
    servoPosition = ((90 - vector.angleDegree())) * -1 + OFFSET_COEF;
    setServo(servoPosition);
  }

  if (pixy.line.numIntersections)
    pixy.line.intersections->print();

  if (pixy.line.barcodes)
    pixy.line.barcodes->print();

  delay(50);
}
