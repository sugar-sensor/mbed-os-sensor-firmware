/*
 * Created on: 24.03.24
 * Author: vadimkim
 * Project: mbed-os-sensor
 */

#include "bioz.h"
#include "helper.h"
#include "mbed.h"
#include "optics.h"

// Common definitions
SPI spi(P5_1, P5_2, P5_0); // mosi, miso, sclk for common SPI bus
DigitalOut cso(P5_3);      // MAX86141 selection, optics
DigitalOut csb(P5_4, 1);   // MAX30002 selection, bioz

int main() {
  printf("\nSensor module firmware v0.0.1\n");
  boardInit();
  Optics optics(spi, cso);
  BioZ bioz(spi, csb);
};