/*
 * Created on: 24.03.24
 * Author: vadimkim
 * Project: mbed-os-sensor
 */

#ifndef _OPTICS_H_
#define _OPTICS_H_

#include "mbed.h"
#include "MAX8614X.h"

class Optics {

public:
  Optics(SPI &spi, DigitalOut &cs);

private:
  static const PinName INT_PIN = P3_2; // interrupt pin for MAX86141
  SPI &p_spi;
  DigitalOut &p_cs;
  MAX8614X p_chip;

};

#endif //_OPTICS_H_