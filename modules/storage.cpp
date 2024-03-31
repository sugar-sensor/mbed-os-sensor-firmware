/*
 * Created on: 30.03.24
 * Author: vadimkim
 * Project: mbed-os-sensor
 */

#include "storage.h"
#include "errno.h"
#include "mbed.h"
#include <cstdio>

FATFileSystem fs("fs");

Storage::Storage() { bd = SDBlockDevice::get_default_instance(); }

int Storage::test() {
  printf("--- File system test ---\n");
  printf("--- Mounting filesystem ---\n");
  fflush(stdout);
  int err = fs.mount(bd);
  printf("%s\n", (err ? "Fail :(" : "OK"));

  if (err) {
    error("error: %s (%d)\n", strerror(-err), err);
    return -1;
  }

  printf("Opening the root directory...\n");
  Dir d;
  err = d.open(&fs, ".");
  if (err) {
    error("error: %s (%d)\n", strerror(-err), err);
    return -1;
  }

  while (true) {
    dirent entry;
    ssize_t res = d.read(&entry);
    if (res < 0) {
      printf("Error reading directory\n");
      break;
    } else if (res == 0) {
      // End of directory
      break;
    } else {
      printf("%s , type: %d\n", entry.d_name, entry.d_type);
    }
  }

  d.close();

  return 0;
}