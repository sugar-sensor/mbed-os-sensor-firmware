/*
 * Created on: 30.03.24
 * Author: vadimkim
 * Project: mbed-os-sensor
 */

#ifndef _STORAGE_H_
#define _STORAGE_H_

#include "mbed.h"
#include "FATFileSystem.h"
#include "SDBlockDevice.h"
#include <string>


// Maximum number of elements in buffer
#define BUFFER_MAX_LEN 10
#define FORCE_REFORMAT false

class Storage {

public:
  Storage();
  int list();
  int append(std::string str);
  void close();

private:
  BlockDevice *bd;
  File measurements;
  // Calculate number of files/folders in SD root
  int files();
};

#endif // _STORAGE_H_