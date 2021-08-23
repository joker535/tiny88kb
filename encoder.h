#ifndef ENCODER_h
#define ENCODER_h

#include"Arduino.h" 


class Encoder
{
  private:
    
    uint8_t encoder_state;
    int8_t  encoder_pulses;
    uint8_t encoder_value;
    uint8_t pad_a;
    uint8_t pad_b;
    int8_t doUpdate(uint8_t state) ;
  public:
    
    Encoder(uint8_t pad_a, uint8_t pad_b);
    ~Encoder();
    void encoderInit(void);
    /**
     * 不变返回0 ，顺时针返回1，逆时针返回-1
     */
    int8_t encoderUpdate(void);
      
};
#endif
