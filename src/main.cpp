
#include <stdlib.h>
#include <Arduino.h>
#include "arduino_no_os_i2c.h"

extern "C"
{
#include "no-OS/drivers/photo-electronic/adpd188/adpd188.h"
}

#define DEVICE_ID 0x16
#define MAX_TRANSFER_SPEED 400000
#define SLAVE_ADDRESS 0x16

int32_t adpd_init()
{
    struct adpd188_dev *adpd_desc = nullptr;
    struct adpd188_init_param adpd_init;

    adpd_init.phy_opt = ADPD188_I2C;
    /* I2C initialization is generally dependent on the platform used */
    adpd_init.phy_init.i2c_phy.device_id = DEVICE_ID;             /* Platform dependent; this is a dummy value */
    adpd_init.phy_init.i2c_phy.max_speed_hz = MAX_TRANSFER_SPEED; /* Platform dependent; this is a dummy value */
    adpd_init.phy_init.i2c_phy.slave_address = SLAVE_ADDRESS;     /* Platform dependent; this is usually a good value */
    adpd_init.phy_init.i2c_phy.platform_ops = &arduino_no_os_i2c_ops;
    /* GPIO initialization is generally dependent on the platform used */
    adpd_init.gpio0_init.number = 1;   /* Platform dependent; this is a dummy value */
    adpd_init.gpio0_init.extra = NULL; /* Platform dependent; this is a dummy value */
    adpd_init.gpio1_init.number = 2;   /* Platform dependent; this is a dummy value */
    adpd_init.gpio1_init.extra = NULL; /* Platform dependent; this is a dummy value */

    // Defining the i2c init param for the arduino
    const struct no_os_i2c_init_param init_param = adpd_init.phy_init.i2c_phy;

    int32_t ret = adpd188_init(&adpd_desc, &adpd_init);
    if (ret != 0)
    {
        return -1;
    }

    uint16_t reg_data = 0;

    /* Read device ID. For ADPD188BI it must be 0x16. */
    ret = adpd188_reg_read(adpd_desc, ADPD188_REG_DEVID, &reg_data);
    if (ret != 0)
    {
        return -1;
    }

    if ((reg_data & ADPD188_DEVID_DEV_ID_MASK) != 0x16)
    {
        return -1;
    }

    /** Enable 32kHz clock */
    ret = adpd188_reg_read(adpd_desc, ADPD188_REG_SAMPLE_CLK, &reg_data);
    if (ret != 0)
    {
        return -1;
    }

    reg_data |= ADPD188_SAMPLE_CLK_CLK32K_EN_MASK;
    ret = adpd188_reg_write(adpd_desc, ADPD188_REG_SAMPLE_CLK, reg_data);
    if (ret != 0)
    {
        return -1;
    }

    /* Activate program mode */
    ret = adpd188_mode_set(adpd_desc, ADPD188_PROGRAM);
    if (ret != 0)
    {
        return -1;
    }

    /* Initialize device in the datasheet smoke detection configuration */
    ret = adpd188_smoke_detect_setup(adpd_desc);
    if (ret != 0)
    {
        return -1;
    }
}

void setup()
{
    int ret = adpd_init();
}

void loop()
{
}