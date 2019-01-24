/* @author ntwong0
 * Angle conversions from accelerometer data
 */

#include <math.h>
#include "types.h"

#ifndef __MY_ACCEL_TO_ANGLES_H__
#define __MY_ACCEL_TO_ANGLES_H__

/* 
 * Orientations
 * 
 * Consider the IC as though sitting atop a hypothetical ball. At "Rest", IC 
 * is flat, facing up. "Rolling" the ball into the following orientations 
 * achieves the following ACCEL_H values.
 *    Orientation, aX, aY, aZ
 *    Rest       , 00, 00, 40
 *    Forward    , 00, c0, 00
 *    Back       , 00, 40, 00
 *    Left       , c0, 00, 00
 *    Right      , 40, 00, 00
 *    Inverted   , 00, 00, c0
 * 
 * Orientation vs. cartesian axis
 *    Roll : X
 *    Pitch: Y
 *    Yaw  : Z
 */

/*
 * Converting a(ccelerometer) values to angles
 * 
 * We now roll the ball back, and hold it in our palm (i.e. the IC is facing 
 * our palm). Rolling the ball clockwise rotates us in the Z-axis, and 
 * provides us with the following ACCEL_H values:
 *    Degrees, aX, aY, aZ
 *      0    , 00, 40, 00
 *     45    , e0, 20, 00
 *     90    , c0, 00, 00
 *    135    , e0, e0, 00
 *    180    , 00, c0, 00
 *    225    , 20, e0, 00
 *    270    , 40, 00, 00
 *    315    , 20, 20, 00
 * 
 * How do we frame these values to a unit circle? Obviously aZ values can be 
 * ignored. What about aX and aY? Let's remap aX and aY to the following:
 *    aY ->  x      aX ->  y
 *    40,    1.0,   40,   -1.0   
 *    20,    0.7,   20,   -0.7
 *    00,    0.0,   00,    0.0
 *    e0,   -0.7,   e0,    0.7
 *    c0,   -1.0,   c0,    1.0
 * 
 * The x and y values now become the adjacent and opposite sides of our right
 * triangle, which means we can use arctan() for our conversion, because
 *    tan(theta) = y/x
 * Note that the result of arctan() spans -pi to pi, instead of 0 to 2*pi
 * 
 * What functions do we need?
 * 1. aX_to_y(aX)
 * 2. aY_to_x(aY), which is just a negation of aX_to_y()
 * 3. arctan(y,x), which is just atan2() from <math.h>
 * 4. radians_to_degrees(radVal), which returns (int)360*0.5*radVal/pi
 * 5. accelVal_to_degrees(aX, aY), which encapsulates arctan(y,x)
 */

double aY_to_x(int8_t aY)
{
  return (double)aY / (double)0x40;
}

double aX_to_y(int8_t aX)
{
  return -aY_to_x(aX);
}

// for arctan(y,x), use atan2()

// @param -pi <= radVal <= pi
uint16_t radians_to_degrees(double radVal)
{
  if(radVal < 0) // degree = (radVal + 2pi)*(360/2pi),
    return (uint16_t)(180.0*(radVal/M_PI + 2));
  else 
    return (uint16_t)(180.0*radVal/M_PI);
}

// with the above in mind, we define the following converter:
/* accelVal_to_degrees
 * @return the angle in degrees
 */
uint16_t accelVal_to_degrees(int8_t aX, int8_t aY)
{
  return radians_to_degrees(atan2(aX_to_y(aX),aY_to_x(aY)));
}

uint32_t normalize_degree_for_servo(uint16_t degree)
{
    if(degree > 270) return 0;
    else if(degree > 180) return 180;
    else return degree;
}

#endif // __MY_ACCEL_TO_ANGLES_H__