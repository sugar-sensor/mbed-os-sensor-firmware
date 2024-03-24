/*
 * Created on: 24.03.24
 * Author: vadimkim
 * Project: mbed-os-sensor
 */

#ifndef _BIOZ_H_
#define _BIOZ_H_

#include "max30002.h"
#include "mbed.h"
#include <cstdint>

class BioZ {

public:
  BioZ(SPI &spi, DigitalOut &cs);

private:
  SPI &p_spi;
  DigitalOut &p_cs;
  MAX30002 p_chip;
};

#endif // _BIOZ_H_