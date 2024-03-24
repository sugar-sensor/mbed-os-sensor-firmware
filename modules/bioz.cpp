/*
 * Created on: 24.03.24
 * Author: vadimkim
 * Project: mbed-os-sensor
 */

#include "bioz.h"
#include "mbed.h"

BioZ::BioZ(SPI &spi, DigitalOut &cs)
    : p_spi(spi), p_cs(cs), p_chip(p_spi, p_cs) {
  
  uint32_t data[1];
  printf("Initilizing MAX30002: ");
  int res = p_chip.readRegister(MAX30002::INFO, data);
  while (res < 0) {
    res = p_chip.readRegister(MAX30002::INFO, data);
    printf("%d. ", res);
    wait_us(500000);
  }
  MAX30002::info_reg result;
  result.all = data[0];
  printf("at revision: %d\n", result.bit.rev_id);

}
