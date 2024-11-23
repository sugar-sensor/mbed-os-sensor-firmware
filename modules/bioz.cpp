/*
 * Created on: 24.03.24
 * Author: vadimkim
 * Project: mbed-os-sensor
 */

#include "bioz.h"
#include "mbed.h"
#include <cstdio>


/****************************************************/
BioZ::BioZ(SPI &spi, DigitalOut &cs)
    : p_spi(spi), p_cs(cs), p_chip(spi, cs) {

  uint32_t data[1];
  printf("Initilizing MAX30002: ");
  MAX30002::info_reg result;
  result.all = data[0];
  p_chip.writeRegister(MAX30002::SW_RST, 0);
  while (result.bit.rev_id != 2) {
    printf(".");
    wait_us(500000);
    p_chip.readRegister(MAX30002::CNFG_BIOZ, data); // first read after SW_RESET shouldn't be INFO
    p_chip.readRegister(MAX30002::INFO, data);
    result.all = data[0];
  }

  printf("at revision: %d\n", result.bit.rev_id);
}

/****************************************************/
int BioZ::revision() {
  uint32_t data[1];
  MAX30002::info_reg result;
  p_chip.readRegister(MAX30002::INFO, data);
  result.all = data[0];
  return result.bit.rev_id;

}

/****************************************************/
int BioZ::readRegister(MAX30002::REG_map_t reg, uint32_t *data) {
    return p_chip.readRegister(reg, data);
}

/****************************************************/
int BioZ::writeRegister(MAX30002::REG_map_t reg, uint32_t data) {
    return p_chip.writeRegister(reg, data);
}
