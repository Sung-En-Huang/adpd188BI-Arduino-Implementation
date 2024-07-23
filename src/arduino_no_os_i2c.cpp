#include "errno.h"
#include "stdlib.h"
#include "arduino_no_os_i2c.h"
#include "Wire.h"

// arduino init function
int32_t arduino_no_os_i2c_init(struct no_os_i2c_desc **desc, const struct no_os_i2c_init_param *param)
{
    // Allocate memory for the I2C descriptor
    *desc = (struct no_os_i2c_desc *)malloc(sizeof(struct no_os_i2c_desc));
    if (!*desc)
    {
        return -ENOMEM; // Memory allocation failed
    }

    // Initialize extra arduino-specific descriptor here
    arduino_no_os_i2c_init_param *arduinoParam = (arduino_no_os_i2c_init_param *)param->extra;
    arduino_no_os_i2c_desc *arduinoDesc = new arduino_no_os_i2c_desc();
    if (!arduinoDesc)
    {
        free(*desc);
        return -ENOMEM; // Memory allocation failed
    }

    (*desc)->extra = arduinoDesc;
    (*desc)->device_id = param->device_id;
    (*desc)->max_speed_hz = param->max_speed_hz;
    (*desc)->slave_address = param->slave_address;

    // Initialize the Wire library
    Wire.begin(param->slave_address);

    return 0; // Return success
}

// arduino write function
int32_t arduino_no_os_i2c_write(struct no_os_i2c_desc *desc, uint8_t *data, uint8_t bytes, uint8_t stop_bit)
{
    // Use extra arduino specific descriptor anyway you want
    arduino_no_os_i2c_desc *arduinoDesc = (arduino_no_os_i2c_desc *)desc->extra;

    Wire.beginTransmission(desc->slave_address);
    for (uint8_t i = 0; i < bytes; i++)
    {
        Wire.write(data[i]);
    }
    uint8_t result = Wire.endTransmission(stop_bit); // Send stop bit if stop_bit is non-zero
    return (result == 0) ? 0 : -1;                   // Return 0 if successful, -1 if there was an error
}

// arduino read function
int32_t arduino_no_os_i2c_read(struct no_os_i2c_desc *desc, uint8_t *data, uint8_t bytes, uint8_t stop_bit)
{
    // Use extra arduino specific descriptor anyway you want
    arduino_no_os_i2c_desc *arduinoDesc = (arduino_no_os_i2c_desc *)desc->extra;

    uint8_t bytesRead = Wire.requestFrom(desc->slave_address, bytes, stop_bit); // Request bytes from the device
    if (bytesRead != bytes)
    {
        return -1; // Error: didn't read the expected number of bytes
    }

    for (uint8_t i = 0; i < bytes; i++)
    {
        if (Wire.available())
        {
            data[i] = Wire.read();
        }
        else
        {
            return -1; // Error: not enough data available
        }
    }

    return 0; // Return success
}

// arduino remove function
int32_t arduino_no_os_i2c_remove(struct no_os_i2c_desc *desc)
{
    if (desc == nullptr)
    {
        return 0;
    }

    if (desc->extra != nullptr)
    {
        delete desc->extra;
    }

    free(desc); // Free the allocated memory for the descriptor
    return 0;   // Return success
}

const struct no_os_i2c_platform_ops arduino_no_os_i2c_ops = {
    .i2c_ops_init = arduino_no_os_i2c_init,
    .i2c_ops_write = arduino_no_os_i2c_write,
    .i2c_ops_read = arduino_no_os_i2c_read,
    .i2c_ops_remove = arduino_no_os_i2c_remove};