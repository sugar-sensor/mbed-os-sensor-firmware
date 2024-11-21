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
#include <cstdio>
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
uint32_t data[1];       // BioZ data single element array
MAX30002::fifo fifo;    // BioZ FIFO    

int main() {
  printf("\nSensor module firmware v0.0.2\n");
  // --------------- HW initialization ----------------//
  set_time(1733004000); // set time to 01.12.2024 00:00
  boardInit();
  
  Optics optics(spi, cso);
  BioZ bioz(spi, csb); // BioZ initialized after optics faster than vise-a-versa. 
                       // Might be interrupt that is registered for Optics during initialization in constructor
  Storage storage; // Init SD card
  storage.list(); // list storage files

  /* Configure optical chip*/
  optics.p_chip.writeRegister(MAX8614X::MAX8614X_LED1_PA_REG, 0xC8);  // LED1 current C8 (200) * 0.12 = 24 mA
  optics.p_chip.writeRegister(MAX8614X::MAX8614X_PPG_CFG2_REG, 0x00); // sampling rate 25sps
  
  /* Configure BioZ*/
  // generator settings
  MAX30002::cnfg_gen_reg cnfg_gen;
  cnfg_gen.bit.en_bioz = 1;
  bioz.p_chip.writeRegister(MAX30002::CNFG_GEN, cnfg_gen.all);
  
  // bioz settings
  MAX30002::cnfg_bioz_reg cnfg_bioz;
  cnfg_bioz.bit.ahpf = 0b111; // bypass AHPF
  cnfg_bioz.bit.fcgen = 0; // max modulation frequency = 4 * fMSTR
  bioz.p_chip.writeRegister(MAX30002::CNFG_BIOZ, cnfg_bioz.all);
  bioz.p_chip.writeRegister(MAX30002::SYNCH, 0x00);
  
  // --------------- working cycle ----------------//
  int rounds = 360; // 1 round ~10 sec = 1 hour
  while (rounds > 0) {
      
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
  
      rounds--;
  }
  
  // --------------- finalization ----------------//

  storage.close();

};