#include <Arduino.h>
#include <Pixy2.h>

Pixy2 pixy;

#include "AdvancePixyVector.h"
#include "Atoms\numeric\vector.h"

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

void loop()
{
  int8_t i;
  char buf[128];
 
  pixy.line.getMainFeatures();

  // pixy.line.vectors
  if (pixy.line.numVectors) {
    Vector* mainVector = pixy.line.vectors;
    //atoms::Vector2<int> vec = PixyVectorToAtomVector2(mainVector); 
    AdvancePixyVector vector{mainVector};
    vector.print();
  }

  if (pixy.line.numIntersections)
    pixy.line.intersections->print();

  if (pixy.line.barcodes)
    pixy.line.barcodes->print();

  delay(250);
}
