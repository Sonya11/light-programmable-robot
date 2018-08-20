#include "FIFO_buffer.h"

//START: (H/T) _ _ _ _ _ _ _ _ _
//_ H _ _ _ _ T _ _ _

FIFO_Buffer::FIFO_Buffer(uint8_t _buff_size){
  buff_size = _buff_size;
  buff_array = new uint8_t[buff_size]; 
  head = 0;
  tail = 0;
  count = 0;
}

FIFO_Buffer::~FIFO_Buffer(){
  delete[] buff_array;
  buff_array = nullptr;
}

bool FIFO_Buffer::isFull(){
  return (buff_size == count);
}

bool FIFO_Buffer::isEmpty(){
  return (count == 0);
}

bool FIFO_Buffer::add_data(uint8_t data){
  if(!isFull()){
    buff_array[tail++] = data; //put in location of tail and then increment tail
    //tail++; //increment tail
    tail %= buff_size; //loop tail back around to front when reaches end (as long as not full)
    count++; //increment count
    return 1; //return 1 means data added successfully
  }else{
    return 0; //data not added
  }
}

int8_t FIFO_Buffer::get_data(){
  if(!isEmpty()){
    int8_t data = buff_array[head++]; //store data and then increment head
    head %= buff_size; //loop head back to beginning when it reaches end (will always be 0 to 9)
    count--; //decrement count 
    return data; 
  }else{
    return -1; 
  }
}

/*
void FIFO_Buffer::clear_buffer(){
  head = 0;
  tail = 0;
}*/

uint8_t * FIFO_Buffer::get_buffer(){
  return buff_array;
}
