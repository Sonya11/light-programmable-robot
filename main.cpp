/* 

Modified timer interrupt code from:
Example Timer1 Interrupt
Flash LED every second

http://www.hobbytronics.co.uk/arduino-timer-interrupts
*/








#include "FIFO_buffer.h"
#include "position_control.h"
#include "Arduino.h"

#define BUFFER_SIZE 10
#define TIMER_10S_PRELOAD 13453 //preload timer 65536-(16MHz/((interrupt_counter + 1)*1024*0.1Hz)) = 13452 <-change this value to change time interval that the robot will recieve data in

//wheel speeds
#define ZERO_SPEED 0
#define EIGHTH_SPEED 31.8      
#define QUARTER_SPEED 63.75
#define HALF_SPEED 127.5
#define FULL_SPEED 255

//pin defines
#define LEDPIN 13
#define PINON 6                               // ON/OFF switch, active HIGH
#define RESETPIN 4                            // pin to reset counter

#define BLUE_LED_PIN 9                        //LED control pins 
#define GREEN_LED_PIN 10
#define YELLOW_LED_PIN 13
  
//define light command global vars 
volatile uint8_t interrupt_counter = 0;       //control timing
volatile uint8_t light_count = 0;             //count light pulses
volatile uint8_t light_count_save = 0;        //save light pulse count to be processed in loop 
volatile uint8_t program_state = 0;           //to encode commands from user into operation state (the robot "listen" state and robot "act" state)
volatile bool timer_flag = 0;                 //set in timer ISR, referenced in loop
bool do_it = 0;                               //enable/disable robot actions (not vol since not changed or accessed by interrupts)


//define position control global vars
volatile uint64_t enc_count_Left = 0;         //count left wheel encoder pulses
const uint8_t pinCW_Left = 7;                 // CW left wheel PMOS gate pin
const uint8_t pinCC_Left = 8;                 // CCW left wheel PMOS gate pin
const uint8_t pinSpeed_Left = 5;   //9           // left wheel speed reference pin
const uint8_t pinCW_Right = 11;               // CW right wheel PMOS gate pin
const uint8_t pinCC_Right = 12;               // CCW right wheel PMOS gate pin
const uint8_t pinSpeed_Right = 6;  //10          // left right speed reference pin

uint8_t color_loop_count = 0;
bool led_on = 0;


float encoderPulses = 0; //do I need to make this global?






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







//buffer instantiation
FIFO_Buffer buff(BUFFER_SIZE);

//ISR declarations
void count_enc();
void count_light();




void setup()
{
////OUTPUT CONTROLS SETUP////
  pinMode(RESETPIN, OUTPUT);
  digitalWrite(RESETPIN, HIGH);              //reset pin on counter chip is active low
  pinMode(BLUE_LED_PIN, OUTPUT);
  digitalWrite(BLUE_LED_PIN, LOW);
  pinMode(GREEN_LED_PIN, OUTPUT);
  digitalWrite(GREEN_LED_PIN, LOW);
  pinMode(YELLOW_LED_PIN, OUTPUT);
  digitalWrite(YELLOW_LED_PIN, LOW);
  
////BUFFER SETUP////
  //UART setup -- for buffer testing
  Serial.begin(9600);

////SPEED/POSITION CONTROL SETUP////
  pinMode(PINON, INPUT);
    
  //left wheel
  pinMode(pinCW_Left,OUTPUT);               //set pins as outputs
  pinMode(pinCC_Left,OUTPUT);
  pinMode(pinSpeed_Left,OUTPUT);
  digitalWrite(pinCW_Left,LOW);             // stop clockwise
  digitalWrite(pinCC_Left,LOW);             // stop counter-clockwise
  analogWrite(pinSpeed_Left, ZERO_SPEED);   //set speed to 0
  
  //right wheel
  pinMode(pinCW_Right,OUTPUT);              //set pins as outputs
  pinMode(pinCC_Right,OUTPUT);
  pinMode(pinSpeed_Right,OUTPUT);
  digitalWrite(pinCW_Right,LOW);            // stop clockwise
  digitalWrite(pinCC_Right,LOW);            // stop counter-clockwise
  analogWrite(pinSpeed_Right, ZERO_SPEED);  // set speed to 0

////TIMER SETUP////
  //Pin setup -- for timer debugging
  //pinMode(LEDPIN, OUTPUT);

  //Timer1 setup
  noInterrupts();                            // disable all interrupts
  TCCR1A = 0;                                // initialize to 0
  TCCR1B = 0;
  
  TCNT1 = TIMER_10S_PRELOAD;                 // preload timer
  TCCR1B |= 0x5;                             // 1024 prescaler 
  TIMSK1 |= (1 << TOIE1);                    // enable timer overflow interrupt
  interrupts();                              // enable all interrupts

////PIN ISR SETUP////
//Connect encoder outputs to external interrupt pins via a 1k resistor
attachInterrupt(0, count_enc, RISING);       //ISR: count_enc -- pin 2
attachInterrupt(1, count_light, RISING);     //ISR: count_light -- pin 3

}





////ISRs////
  //Encoder Count ISR
  void count_enc(){
    enc_count_Left++;
  }

  //Light Pulse ISR
  void count_light(){
    light_count++;
  }


  
  //Timer1 ISR
  ISR(TIMER1_OVF_vect)  // interrupt service routine  -- ISR is C method of calling interrupt vectors for atmegas <3
  {
    if(interrupt_counter > 2){ //to make the clock slower bc clkI/O (16MHz) can only be divided down by 1024 #lol
      digitalWrite(RESETPIN, LOW);                      //send reset signal to counter chip
      digitalWrite(RESETPIN, HIGH);
      TCNT1 = TIMER_10S_PRELOAD;                        // preload timer
      //digitalWrite(LEDPIN, digitalRead(LEDPIN) ^ 1);    //toggle on-board LED to see timing intervals 

      light_count_save = light_count;
      light_count = 0;
      switch(light_count_save){
        case 0:
          program_state = 0;
          break;
        case 1:
          program_state = 1;
          break;
        case 2:
          program_state = 2;
          break;
        //if it is not 0, 1, or 2, it will maintain its previous state <3
        
       // default: 
         // program_state = 1; //if entering action, set to "start programming" state
      }
      timer_flag = 1;    
      interrupt_counter = 0;
    }
    interrupt_counter++;
  }


  
  









void loop()
{
  /*
  turn_left(7, QUARTER_SPEED);
  delay(1000);
  forward(11, HALF_SPEED);
   delay(1000);
  turn_left(7, QUARTER_SPEED);
   delay(1000);*/
  
  while(timer_flag){ //process commands loop 
    Serial.print("light pulse count: ");
    Serial.println(light_count_save);
    switch(program_state){
      case 1: //"start programming"
        if(light_count_save != 1 && light_count_save < 16){     //don't add start command or anything hgiher than 15 to buffer 
          if(!buff.add_data(light_count_save)){
            do_it = 1;                                          //if buffer is full then start doing actions 
          }
        }
        break;
      case 2: //"stop programming"
        do_it = 1;
        break;
      //otherwise, do nothing 
    }
    timer_flag = 0; //clear timer interrupt flag
  }
  
  while(do_it){                                                  //perform actions loop 
    switch(buff.get_data()){                                     //buff.get_data should never = 0-2 bc these are commands not actions 
      case -1: //means that buffer is empty
        Serial.println("buffer is empty <3");
        do_it = 0;                                               //change to 0 to exit action loop
        break;
      case 1:                                                    //these are commands, not actions so they should never enter the buffer
        Serial.println("entered 1 state -- something is wrong"); 
        break;
      case 2:                                                    //these are commands, not actions so they should never enter the buffer
        Serial.println("entered 2 state -- something is wrong");
        break;
      case 3:
        Serial.println("number 3 action -- pause for 5 sec");
        delay(3000);
        break;
      case 4:
        Serial.println("number 4 action -- forward 1 ft slowly");
        forward(11, EIGHTH_SPEED);      
        break;
      case 5:
        Serial.println("number 5 action -- forward 1 ft medium");
        forward(11, QUARTER_SPEED);
        break;
      case 6:
        Serial.println("number 6 action -- forward 1 ft fast");
        forward(11, HALF_SPEED);
        break;
      case 7:
        Serial.println("number 7 action -- turn right 90 deg"); 
        turn_right(8, QUARTER_SPEED);
        break;
      case 8:
        Serial.println("number 8 action -- turn left 90 deg");
        turn_left(7, QUARTER_SPEED);
        break;
      case 9:
        Serial.println("number 9 action -- turn 180");
        turn_right(15.6, QUARTER_SPEED);//CW turn
        break;
      case 10:
        Serial.println("number 10 action -- tornado (turn 360)"); 
        turn_right(114, HALF_SPEED);//CW turn
        break;
      case 11:
        Serial.println("number 11 action -- moonwalk");
        turn_right(15.6, QUARTER_SPEED);//turn 180
        turn_right(3, QUARTER_SPEED);
        forward(6, QUARTER_SPEED);
        turn_left(5, QUARTER_SPEED);
        forward(12, QUARTER_SPEED);
        turn_right(8, QUARTER_SPEED);
        forward(12, QUARTER_SPEED);
        turn_left(7, QUARTER_SPEED);
        forward(12, QUARTER_SPEED);
        //turn_right(7, QUARTER_SPEED);
        turn_left(11, QUARTER_SPEED);//turn 180
        break;
      case 12:
        Serial.println("number 12 action -- boomerang");
        forward (22, QUARTER_SPEED); //2 feet forward
        turn_right(15.6, QUARTER_SPEED);//CW turn
        forward (22, QUARTER_SPEED); //2 feet forward
        turn_left(15.7, QUARTER_SPEED);//CCW turn
        break;
      case 13:
        Serial.println("number 13 action -- flash LED yellow");
        digitalWrite(YELLOW_LED_PIN, HIGH);
        delay(500);
        digitalWrite(YELLOW_LED_PIN, LOW);
        delay(500);
        digitalWrite(YELLOW_LED_PIN, HIGH);
        delay(500);
        digitalWrite(YELLOW_LED_PIN, LOW);
        delay(500);
        digitalWrite(YELLOW_LED_PIN, HIGH);
        delay(500);
        digitalWrite(YELLOW_LED_PIN, LOW);
        delay(500);
        break;
      case 14:
        Serial.println("number 14 action -- flash LED red");
        digitalWrite(GREEN_LED_PIN, HIGH);
        delay(500);
        digitalWrite(GREEN_LED_PIN, LOW);
        delay(500);
        digitalWrite(GREEN_LED_PIN, HIGH);
        delay(500);
        digitalWrite(GREEN_LED_PIN, LOW);
        delay(500);
        digitalWrite(GREEN_LED_PIN, HIGH);
        delay(500);
        digitalWrite(GREEN_LED_PIN, LOW);
        break;
      case 15:
        Serial.println("number 15 action -- flash LED blue");
        digitalWrite(BLUE_LED_PIN, HIGH);
        delay(500);
        digitalWrite(BLUE_LED_PIN, LOW);
        delay(500);
        digitalWrite(BLUE_LED_PIN, HIGH);
        delay(500);
        digitalWrite(BLUE_LED_PIN, LOW);
        delay(500);
        digitalWrite(BLUE_LED_PIN, HIGH);
        delay(500);
        digitalWrite(BLUE_LED_PIN, LOW);
        delay(500);
        /*
        while(color_loop_count < 5){
          led_on ^= led_on;
          digitalWrite(BLUE_LED_PIN, led_on);
          color_loop_count++;
          delay(500);
        }
        led_on = 0;
        digitalWrite(BLUE_LED_PIN, led_on);
        color_loop_count = 0;*/
        break;
      default:
        Serial.println("entered defalut state - something is wrong");
        do_it = 0;                                                    //something went wrong, set do_it to 0
        
    }
  }
 
}



