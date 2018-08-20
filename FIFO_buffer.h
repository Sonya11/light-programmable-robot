#ifndef FIFO_BUFFER_H
#define FIFO_BUFFER_H

#include "Arduino.h"

class FIFO_Buffer{
private:
  uint8_t buff_size;
  uint8_t * buff_array;
  uint8_t head; //index, not poitner
  uint8_t tail; //index, not poitner
  uint8_t count;
  bool isFull();
  bool isEmpty();
public:
  FIFO_Buffer(uint8_t _buff_size); //constructor
  ~FIFO_Buffer(); //destructor
  bool add_data(uint8_t data);
  int8_t get_data();
  //void clear_buffer();
  uint8_t * get_buffer();  //for debugging 
};

#endif
