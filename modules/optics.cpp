/*
 * Created on: 24.03.24
 * Author: vadimkim
 * Project: mbed-os-sensor
 */

#include "optics.h"
#include <numeric>
#include <unordered_map>
#include <utility>
#include <vector>

/****************************************************/
Optics::Optics(SPI &spi, DigitalOut &cs)
    : p_spi(spi), p_cs(cs), p_chip(spi, cs, INT_PIN) {

  while (p_chip.init() < 0) {
    wait_us(500000);
  };

}
/****************************************************/
int Optics::disable() {
    // TODO disable readings
    return 0;
}

/****************************************************/
int Optics::enable() {
    // TODO enable readings
    return 0;
}

/****************************************************/
int Optics::writeRegister(uint8_t reg, const uint8_t data) {
    return p_chip.writeRegister(reg, data);
}

/****************************************************/
vector<pair<uint32_t, uint32_t>> Optics::readFIFOdata() {

    vector<pair<uint32_t, uint32_t>> input;
    vector<pair<uint32_t, uint32_t>> result;
    uint8_t sampleCnt[1];
    uint8_t dataBuf[128 * 3];

    p_chip.readRegister(MAX8614X::MAX8614X_FIFO_DATA_CNT_REG, sampleCnt, 1);
    p_chip.readRegister(MAX8614X::MAX8614X_FIFO_DATA_REG, dataBuf, sampleCnt[0] * 3);

    // printf("FIFO cnt: %d\n", sampleCnt[0]);
    // Gather samples
    for (int i = 0; i < sampleCnt[0] / 3; i++) {
        unsigned int pos = i * 3;
        unsigned int value = 0;

        value = ((dataBuf[pos]) & 0x03) << 16 | dataBuf[pos + 1] << 8 |
                dataBuf[pos + 2];
        // printf("tag, value: %d, %d\n", (dataBuf[pos]) >> 3 & 0x1f, value);
        pair<uint32_t, uint32_t> element = {(dataBuf[pos]) >> 3 & 0x1f, value};
        input.push_back(element);
        
    }

    // Group samples by tag
    unordered_map<uint32_t, vector<uint32_t>> groups;

    for (const auto& tuple : input) {
        int tag = tuple.first;
        int value = tuple.second;
        groups[tag].push_back(value);
    }

    // Prepare the result as a list of tuples with averages
    for (const auto& group : groups) {
        int tag = group.first;
        const vector<uint32_t>& values = group.second;

        // Calculate the average
        double average = accumulate(values.begin(), values.end(), 0.0) / values.size();
        result.emplace_back(tag, static_cast<uint32_t>(ceil(average)));
    }

    return result;    
}
