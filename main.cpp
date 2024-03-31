/*
 * Created on: 24.03.24
 * Author: vadimkim
 * Project: mbed-os-sensor
 */

#include "bioz.h"
#include "helper.h"
#include "mbed.h"
#include "optics.h"
#include "storage.h"
#include <ctime>
#include <string>

SPI spi(P5_1, P5_2, P5_0); // mosi, miso, sclk for common SPI bus
DigitalIn s0(P4_2, PullDown);  // accordin to spec unused SPIS ports
DigitalIn s1(P4_4, PullDown);  // must be configured as high-impedance
DigitalIn s2(P4_5, PullDown);  // inputs
DigitalIn s3(P4_6, PullDown);
DigitalIn s4(P4_7, PullDown);
DigitalOut cso(P5_3);   // MAX86141 selection, optics
DigitalOut csb(P5_4);   // MAX30002 selection, bioz

time_t seconds;

int main() {
  printf("\nSensor module firmware v0.0.1\n");
  set_time(1704060000); // set time to 01.01.2024 00:00
  boardInit();
  Optics optics(spi, cso);
  BioZ bioz(spi, csb); // BioZ initialized after optics faster than vise-a-versa. 
                       // Might be interrupt that is registered for Optics during initialization in constructor
  Storage storage; // Init SD card
  storage.list();
  std::string str = std::to_string(time(NULL));
  str.append(";");
  str.append("test value\n");
  storage.append(str);
  wait_us(2000000);
  str.clear();
  str.assign(std::to_string(time(NULL)));
  str.append(";");
  str.append("test 2 value\n");
  storage.append(str);
  storage.list();
  storage.close();

  // TODO logic  
};