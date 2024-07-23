#pragma once

#include <stdint.h>

extern "C"
{
#include "no_os_i2c.h"
}

struct arduino_no_os_i2c_desc
{
    // Add extra arduino-specific descriptor params here
};
struct arduino_no_os_i2c_init_param
{
    // Add extra arduino-specific init params here
};

int32_t arduino_no_os_i2c_init(struct no_os_i2c_desc **desc, const struct no_os_i2c_init_param *param);
int32_t arduino_no_os_i2c_write(struct no_os_i2c_desc *desc, uint8_t *data, uint8_t bytes, uint8_t stop_bit);
int32_t arduino_no_os_i2c_read(struct no_os_i2c_desc *desc, uint8_t *data, uint8_t bytes, uint8_t stop_bit);
int32_t arduino_no_os_i2c_remove(struct no_os_i2c_desc *desc);

extern const struct no_os_i2c_platform_ops arduino_no_os_i2c_ops;