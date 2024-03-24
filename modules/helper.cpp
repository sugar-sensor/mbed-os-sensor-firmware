/*
 * Created on: 24.03.24
 * Author: vadimkim
 * Project: mbed-os-sensor
 */
 
#include "helper.h"

void boardInit() {
  printf("Initializing interface board: ");
  MAX32630FTHR mbed_board;
  int res = mbed_board.init(MAX32630FTHR::VIO_1V8);
  while (res < 0) {
    printf(". ");
    wait_us(200000);
  }
  printf("done!\n");
}
  