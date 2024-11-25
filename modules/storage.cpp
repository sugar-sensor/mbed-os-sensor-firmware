/*
 * Created on: 30.03.24
 * Author: vadimkim
 * Project: mbed-os-sensor
 */

#include "storage.h"
#include "errno.h"
#include "mbed.h"
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

FATFileSystem fs("fs");

/****************************************************/
Storage::Storage() {
  bd = SDBlockDevice::get_default_instance();
  printf("--- Mounting filesystem ---\n");
  fflush(stdout);
  int err = fs.mount(bd);
  printf("%s\n", (err ? "Fail :(" : "OK"));

  if (err) {
    error("error: %s (%d)\n", strerror(-err), err);
  }
  std::string fileName = "measurement" + std::to_string(files()) + ".csv";
  measurements.open(&fs, fileName.c_str(), O_CREAT | O_WRONLY);
  printf("Setting working file to: %s\n", fileName.c_str());
}

/****************************************************/
int Storage::list() {
  printf("--- List root folder ---\n");
  Dir d;
  int err = d.open(&fs, "/");
  if (err) {
    error("error: %s (%d)\n", strerror(-err), err);
    return -1;
  }

  while (true) {
    dirent entry;
    ssize_t res = d.read(&entry);
    if (res < 0) {
      printf("Error reading directory\n");
      return res;
    } else if (res == 0) {
      // End of directory
      break;
    } else {
      if (entry.d_type == 5) {
        File f;
        f.open(&fs, entry.d_name);
        printf("%s, type: %d, size:%lu\n", entry.d_name, entry.d_type,
               f.size());
        f.close();
      } else {
        printf("%s, type: %d\n", entry.d_name, entry.d_type);
      }
    }
  }
  d.close();
  return 0;
}
/****************************************************/
int Storage::append(std::string str) {
  const char *data = str.c_str();
  int written = measurements.write(data, str.length());
  if (written < 0) {
    printf("Error writing to file\n");
    measurements.close();
    fs.unmount();
    return -1;
  }
  measurements.sync();
  return 0;
}

/****************************************************/
void Storage::append(Sample sample) {
  std::string str;
  str.assign(std::to_string(sample.getTime()));
  str.append(";");
  str.append(std::to_string(sample.getType()));
  str.append(";");
  
  for (const auto &tuple : sample.getData()) {
    str.append("(" + std::to_string(tuple.first) + "," + std::to_string(tuple.second) + ")");
    str.append("-");
  }
  
  str.pop_back(); // remove last "-"
  str.append("\n"); 
  append(str);
}

/****************************************************/
int Storage::files() {
  Dir d;
  int err = d.open(&fs, ".");
  if (err) {
    error("error: %s (%d)\n", strerror(-err), err);
    return -1;
  }
  return d.size();
}

/****************************************************/
void Storage::close() {
  measurements.close();
  fs.unmount();
}