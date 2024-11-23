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
#include <vector>

class BioZ {

public:
  BioZ(SPI &spi, DigitalOut &cs);
  int revision();
  int readRegister(MAX30002::REG_map_t reg, uint32_t *data);
  int writeRegister(MAX30002::REG_map_t reg, uint32_t data);
  vector<pair<uint32_t, uint32_t>> readFIFOdata();

private:
  SPI &p_spi;
  DigitalOut &p_cs;
  MAX30002 p_chip;
};

#endif // _BIOZ_H_