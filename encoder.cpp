#include "encoder.h"


void Encoder::encoderInit(void) 
{
    pinMode(pad_a,INPUT);
  pinMode(pad_b,INPUT);
  encoder_state = (digitalRead(pad_a) << 0) | (digitalRead(pad_a) << 1);
}


int8_t  Encoder::doUpdate(uint8_t state) 
{
   int8_t changed = 0;
   uint8_t resolution = 4;
   int8_t encoder_LUT[] = {0, -1, 1, 0, 1, 0, 0, -1, -1, 0, 0, 1, 0, 1, -1, 0};
   encoder_pulses += encoder_LUT[state & 0xF];
    if (encoder_pulses >= resolution) {
        encoder_value++;
        changed = 1;
    }
    if (encoder_pulses <= -resolution) {  // direction is arbitrary here, but this clockwise
        encoder_value--;
        changed = -1;
    }
    encoder_pulses %= resolution;
    return changed;
}

int8_t Encoder::encoderUpdate(void)
{
    encoder_state <<= 2;
    encoder_state |= (digitalRead(pad_a) << 0) | (digitalRead(pad_b) << 1);
    return  doUpdate(encoder_state);
}

//
Encoder::Encoder(uint8_t padA, uint8_t padB) 
{
  pad_a = padA;
  pad_b = padB;
}
Encoder::~Encoder() 
{
 
}
