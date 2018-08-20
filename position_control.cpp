
#include "position_control.h"

/*
extern volatile uint64_t enc_count_Left;  //count of left wheel encoder pulses
extern const uint8_t pinCW_Left;          // CW left wheel PMOS gate pin
extern const uint8_t pinCC_Left;          // CCW left wheel PMOS gate pin
extern const uint8_t pinSpeed_Left;       // left wheel speed reference pin
extern const uint8_t pinCW_Right;         // CW right wheel PMOS gate pin
extern const uint8_t pinCC_Right;         // CCW right wheel PMOS gate pin
extern const uint8_t pinSpeed_Right;      // left right speed reference pin
*/


//wheel speeds
#define ZERO_SPEED 0
#define EIGHTH_SPEED 31.8      
#define QUARTER_SPEED 63.75
#define HALF_SPEED 127.5
#define FULL_SPEED 255

void reverse(float Dist_inch, uint8_t Speed){
  
  //float
   encoderPulses = Dist_inch/.0209;         //(.53mm = .0209in) Find total number of encoder pulses to travel the distance, Dist_inch 
  
  analogWrite(pinSpeed_Left,Speed);        //set wheel speeds
  analogWrite(pinSpeed_Right,Speed);
  digitalWrite(pinCW_Left, LOW);           //START: turn on wheels so that they rotate in the same direction
  digitalWrite(pinCC_Left, HIGH);
  digitalWrite(pinCC_Right, LOW);
  digitalWrite(pinCW_Right, HIGH);

  while (enc_count_Left < encoderPulses);  //wait for the encoder count to be >= the caluclated encoder pulse count

  digitalWrite(pinCW_Left, LOW);           //STOP: turn off all wheels and directions
  digitalWrite(pinCC_Left, LOW);
  digitalWrite(pinCC_Right, LOW);
  digitalWrite(pinCW_Right, LOW);

  enc_count_Left = 0;                      //reset encoder count to 0
}
/*
void forward(float Dist_inch, uint8_t Speed){
  
  //float encoderPulses = Dist_inch/.0209; //(.53mm = .0209in) 
  encoderPulses = Dist_inch/.0209; //(.53mm = .0209in) 
    
  analogWrite(pinSpeed_Left,Speed);
  analogWrite(pinSpeed_Right,Speed);
  digitalWrite(pinCC_Left, LOW);
  digitalWrite(pinCW_Left, HIGH);
  digitalWrite(pinCW_Right, LOW);
  digitalWrite(pinCC_Right, HIGH);

  while (enc_count_Left < encoderPulses);
  
  digitalWrite(pinCW_Left, LOW);
  digitalWrite(pinCC_Left, LOW);
  digitalWrite(pinCC_Right, LOW);
  digitalWrite(pinCW_Right, LOW);

  enc_count_Left = 0;
}
*/
void turn_left(float Dist_inch, uint8_t Speed){

  //float
  encoderPulses = Dist_inch/.0209; //.53mm = .0209in
  
  analogWrite(pinSpeed_Left,Speed);
  analogWrite(pinSpeed_Right,Speed);
  digitalWrite(pinCW_Left, LOW);
  digitalWrite(pinCC_Left, HIGH);
  digitalWrite(pinCW_Right, LOW);
  digitalWrite(pinCC_Right, HIGH);

  while (enc_count_Left < encoderPulses);
  
  digitalWrite(pinCW_Left, LOW);
  digitalWrite(pinCC_Left, LOW);
  digitalWrite(pinCC_Right, LOW);
  digitalWrite(pinCW_Right, LOW);

  enc_count_Left = 0;
}

void turn_right(float Dist_inch, uint8_t Speed){

  //float 
  encoderPulses = Dist_inch/.0209; //.53mm = .0209in
 
  analogWrite(pinSpeed_Left,Speed);
  analogWrite(pinSpeed_Right,Speed);
  digitalWrite(pinCC_Left, LOW);
  digitalWrite(pinCW_Left, HIGH);
  digitalWrite(pinCC_Right, LOW);
  digitalWrite(pinCW_Right, HIGH);

  while (enc_count_Left < encoderPulses);
  
  digitalWrite(pinCW_Left, LOW);
  digitalWrite(pinCC_Left, LOW);
  digitalWrite(pinCC_Right, LOW);
  digitalWrite(pinCW_Right, LOW);

  enc_count_Left = 0;
}

void forward_and_right(float Dist_inch){

  //float 
  encoderPulses = Dist_inch/.0209; //.53mm = .0209in
 
  analogWrite(pinSpeed_Left, QUARTER_SPEED);
  analogWrite(pinSpeed_Right, QUARTER_SPEED);
  digitalWrite(pinCC_Left, LOW);
  digitalWrite(pinCW_Left, HIGH);
  digitalWrite(pinCC_Right, LOW);
  digitalWrite(pinCW_Right, HIGH);

  while (enc_count_Left < encoderPulses);
  
  digitalWrite(pinCW_Left, LOW);
  digitalWrite(pinCC_Left, LOW);
  digitalWrite(pinCC_Right, LOW);
  digitalWrite(pinCW_Right, LOW);

  enc_count_Left = 0;
}

void forward_and_left(float Dist_inch){

 // float 
 encoderPulses = Dist_inch/.0209; //.53mm = .0209in
 
  analogWrite(pinSpeed_Left,QUARTER_SPEED);
  analogWrite(pinSpeed_Right,QUARTER_SPEED);
  digitalWrite(pinCC_Left, LOW);
  digitalWrite(pinCW_Left, HIGH);
  digitalWrite(pinCC_Right, LOW);
  digitalWrite(pinCW_Right, HIGH);

  while (enc_count_Left < encoderPulses); //wait
  
  digitalWrite(pinCW_Left, LOW);
  digitalWrite(pinCC_Left, LOW);
  digitalWrite(pinCC_Right, LOW);
  digitalWrite(pinCW_Right, LOW);

  enc_count_Left = 0;
}

