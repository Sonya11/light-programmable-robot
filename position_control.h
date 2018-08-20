#ifndef POSITION_CONTROL_H
#define POSITION_CONTROL_H

#include "Arduino.h"

extern volatile uint64_t enc_count_Left;  //count of left wheel encoder pulses
extern const uint8_t pinCW_Left;          // CW left wheel PMOS gate pin
extern const uint8_t pinCC_Left;          // CCW left wheel PMOS gate pin
extern const uint8_t pinSpeed_Left;       // left wheel speed reference pin
extern const uint8_t pinCW_Right;         // CW right wheel PMOS gate pin
extern const uint8_t pinCC_Right;         // CCW right wheel PMOS gate pin
extern const uint8_t pinSpeed_Right;      // left right speed reference pin
extern float encoderPulses;

void forward(float Dist_inch, uint8_t Speed);
void reverse(float Dist_inch, uint8_t Speed);
void turn_left(float Dist_inch, uint8_t Speed);
void turn_right(float Dist_inch, uint8_t Speed);
void forward_and_right(float Dist_inch);
void forward_and_left(float Dist_inch);


#endif
