/*
 * Created on: 30.03.24
 * Author: vadimkim
 * Project: mbed-os-sensor
 */

#ifndef _STORAGE_H_
#define _STORAGE_H_

#include "SDBlockDevice.h"
#include "FATFileSystem.h"

// Maximum number of elements in buffer
#define BUFFER_MAX_LEN 10
#define FORCE_REFORMAT false

class Storage {

public:
  Storage();
  int test();

private:
  BlockDevice *bd;
};

#endif // _STORAGE_H_