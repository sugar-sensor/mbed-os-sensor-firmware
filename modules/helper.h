/*
 * Created on: 24.03.24
 * Author: vadimkim
 * Project: mbed-os-sensor
 */

#ifndef _HELPER_H_
#define _HELPER_H_

#include "max32630fthr.h"
#include <ctime>
#include <string>
#include <vector>

void boardInit();

class Sample {
    
    private:
        std::time_t time;
        vector<pair<uint32_t,uint32_t>> data;

    public:

        Sample() : time(NULL), data(NULL) { }
        Sample(std::time_t t, vector<pair<uint32_t,uint32_t>> d) : time(t), data(d) {}

        std::time_t getTime() {
            return time;
        }

        void setTime(std::time_t t) {
            time = t;
        }

        vector<pair<uint32_t,uint32_t>> getData() {
            return data;
        }

        void setData(vector<pair<uint32_t,uint32_t>> d) {
            data = d;
        }

        void print() const {
            std::tm* localTime = std::localtime(&time); // Convert time_t to local time
            printf("Time: %04d-%02d-%02d %02d:%02d:%02d\n",
               localTime->tm_year + 1900, // Year (years since 1900)
               localTime->tm_mon + 1,    // Month (0-based)
               localTime->tm_mday,       // Day of the month
               localTime->tm_hour,       // Hour (24-hour format)
               localTime->tm_min,        // Minutes
               localTime->tm_sec);
        
            for (const auto& tuple : data) {
                printf("  type: %d, value: %d\n", tuple.first, tuple.second);
            }
        }

};

#endif // _HELPER_H_