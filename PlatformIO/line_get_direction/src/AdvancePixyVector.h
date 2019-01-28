#pragma once

#include <Pixy2.h>
#include "Atoms\numeric\vector.h"

struct AdvancePixyVector : atoms::Vector2<int8_t> {
    ::Vector* pixyVector;
    
    AdvancePixyVector(::Vector *vector) : pixyVector(vector) {
        int8_t x = -1;
        int8_t y = -1;

        // normalize the vector to same direction -> from down to up
        if (vector->m_y1 < vector->m_y0) {
            y = (PIXY_HEIGHT_IN_LINE_MODE - vector->m_y1) - (PIXY_HEIGHT_IN_LINE_MODE - vector->m_y0);
            x = (vector->m_x1 - vector->m_x0);
        } else {
            y = (PIXY_HEIGHT_IN_LINE_MODE - vector->m_y0) - (PIXY_HEIGHT_IN_LINE_MODE - vector->m_y1);
            x = (vector->m_x0 - vector->m_x1);
        }

        // save x and y to atoms::Vector2 elements
        _elems[0] = x;
        _elems[1] = y;
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

    static const int16_t ANGLE_COEFFICIENT = 180.0 / (M_PI); 
    static const uint8_t PIXY_HEIGHT_IN_LINE_MODE = 51;
    static const uint8_t PIXY_WIDTH_IN_LINE_MODE = 78;
};
