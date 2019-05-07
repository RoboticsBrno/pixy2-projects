#pragma once

#include <Pixy2.h>

struct AdvancePixyVector {
    ::Vector* pixyVector;
    int8_t x = -1;
    int8_t y = -1;
    
    AdvancePixyVector(::Vector *vector) : pixyVector(vector) {
        // normalize the vector to same direction -> from down to up
        if (vector->m_y0 < vector->m_y1)  { // top left coordinate is 0 => https://docs.pixycam.com/wiki/lib/exe/detail.php?id=wiki%3Av2%3Aline_quickstart&media=wiki:v2:image_366.png
            uint8_t temp = vector->m_y0;
            vector->m_y0 = vector->m_y1;
            vector->m_y1 = temp; 

            temp = vector->m_y0;
            vector->m_y0 = vector->m_y1;
            vector->m_y1 = temp; 
            
        }
        y = (vector->m_y0 - vector->m_y1);
        x = (vector->m_x0 - vector->m_x1);
    }

    int16_t angleDegree() {
        return atan2(y, x) * ANGLE_COEFFICIENT;
    }    

    void print()
    {
        char buf[256];
        sprintf(buf, "vector: (%3d %3d) (%3d %3d) index: %3d, x: %3d, y: %3d, angle: %3d",
            pixyVector->m_x0, 
            pixyVector->m_y0, 
            pixyVector->m_x1, 
            pixyVector->m_y1, 
            pixyVector->m_index, 
            x,
            y,
            angleDegree()
        );
        Serial.println(buf);
    }

    static const int16_t ANGLE_COEFFICIENT = 180.0 / (M_PI); // approximation to integer
    static const uint8_t PIXY_HEIGHT_IN_LINE_MODE = 51;
    static const uint8_t PIXY_WIDTH_IN_LINE_MODE = 78;
};
