/*
 * Created on: 24.03.24
 * Author: vadimkim
 * Project: mbed-os-sensor
 */

#ifndef _OPTICS_H_
#define _OPTICS_H_

#include "mbed.h"
#include "MAX8614X.h"
#include <vector>

class Optics {

public:
  Optics(SPI &spi, DigitalOut &cs);
  int writeRegister(uint8_t reg, const uint8_t data);
  vector<pair<uint32_t, uint32_t>> readFIFOdata();

private:
  static const PinName INT_PIN = P3_2; // interrupt pin for MAX86141
  SPI &p_spi;
  DigitalOut &p_cs;
  MAX8614X p_chip;

};

#endif //_OPTICS_H_