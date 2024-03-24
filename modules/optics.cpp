/*
 * Created on: 24.03.24
 * Author: vadimkim
 * Project: mbed-os-sensor
 */

#include "optics.h"

Optics::Optics(SPI &spi, DigitalOut &cs)
    : p_spi(spi), p_cs(cs), p_chip(spi, cs, INT_PIN) {

  while (p_chip.init() < 0) {
    wait_us(500000);
  };
}