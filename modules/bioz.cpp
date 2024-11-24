/*
 * Created on: 24.03.24
 * Author: vadimkim
 * Project: mbed-os-sensor
 */

#include "bioz.h"
#include "mbed.h"
#include <cstdio>

/****************************************************/
BioZ::BioZ(SPI &spi, DigitalOut &cs) : p_spi(spi), p_cs(cs), p_chip(spi, cs) {

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
int BioZ::disable() {
    MAX30002::cnfg_gen_reg cnfg_gen;
    uint32_t data[1];

    readRegister(MAX30002::CNFG_GEN, data);
    cnfg_gen.all = data[0]; // persist old settings
    cnfg_gen.bit.en_bioz = 0;
    return writeRegister(MAX30002::CNFG_GEN, cnfg_gen.all);
}
/****************************************************/
int BioZ::enable() {
    MAX30002::cnfg_gen_reg cnfg_gen;
    uint32_t data[1];

    readRegister(MAX30002::CNFG_GEN, data);
    cnfg_gen.all = data[0]; // persist old settings
    cnfg_gen.bit.en_bioz = 1;
    return writeRegister(MAX30002::CNFG_GEN, cnfg_gen.all);
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

/****************************************************/
vector<pair<uint32_t, uint32_t>> BioZ::readFIFOdata() {
  vector<pair<uint32_t, uint32_t>> result;
  MAX30002::cnfg_bioz_reg cnfg_bioz;
  MAX30002::fifo fifo;
  MAX30002::status_reg status;
  uint32_t data[1];

  // iterate through 10 FCGEN frequency values
  for (int i = 0; i <= 10; i++) {
    cnfg_bioz.bit.fcgen = i;
    p_chip.writeRegister(MAX30002::CNFG_BIOZ, cnfg_bioz.all);
    p_chip.writeRegister(MAX30002::SYNCH, 0x00);

    int samples = 0;
    int voltage = 0;

    while (samples <= 10) {
      p_chip.readRegister(MAX30002::STATUS, data);
      status.all = data[0];

      if (status.bit.bovf == 1) {
        p_chip.writeRegister(MAX30002::FIFO_RST, 0);
      } else if (status.bit.bint == 1) {
        p_chip.readRegister(MAX30002::FIFO, data);
        fifo.all = data[0];
        while (fifo.bit.btag == 0b000 || fifo.bit.btag == 0b010) {
          voltage += fifo.bit.voltage;
          samples++;
          p_chip.readRegister(MAX30002::FIFO, data);
          fifo.all = data[0];
        }
      } else {
        p_chip.writeRegister(MAX30002::SYNCH, 0x00);
      }
      wait_us(50000);
    }

    pair<uint32_t, uint32_t> element = {i, voltage / samples};
    result.push_back(element);
    
  }
  return result;
}
