/*
 * Copyright (c) 2015 Intel Corporation.  All right reserved.
 *
 * TODO - add appropriate license header
*/

#include "Arduino.h"
#include "portable.h"

#ifdef __cplusplus
 extern "C" {
#endif

void pinMode( uint8_t pin, uint8_t mode )
{
    PinDescription *p = &g_APinDescription[pin];

    if (mode == OUTPUT) {
        if (p->ulGPIOType == SS_GPIO) {
            uint32_t reg = p->ulGPIOBase + SS_GPIO_SWPORTA_DDR;
            SET_ARC_BIT(reg, p->ulGPIOId);
        }
        else if (p->ulGPIOType == SOC_GPIO) {
            uint32_t reg = p->ulGPIOBase + SOC_GPIO_SWPORTA_DDR;
            SET_MMIO_BIT(reg, p->ulGPIOId);
        }
    } else {
        if (p->ulGPIOType == SS_GPIO) {
            uint32_t reg = p->ulGPIOBase + SS_GPIO_SWPORTA_DDR;
            CLEAR_ARC_BIT(reg, p->ulGPIOId);
        }
        else if (p->ulGPIOType == SOC_GPIO) {
            uint32_t reg = p->ulGPIOBase + SOC_GPIO_SWPORTA_DDR;
            CLEAR_MMIO_BIT(reg, p->ulGPIOId);
        }
    }

    /* Set SoC pin mux configuration */
    SET_PIN_PULLUP(p->ulSocPin, (mode == INPUT_PULLUP) ? 1 : 0);
    if (p->ulPinMode != GPIO_MUX_MODE) {
        SET_PIN_MODE(p->ulSocPin, GPIO_MUX_MODE);
        p->ulPinMode = GPIO_MUX_MODE;
    }
}

void digitalWrite( uint8_t pin, uint8_t val )
{
    PinDescription *p = &g_APinDescription[pin];

    if (pin >= NUM_DIGITAL_PINS) return;

    if (p->ulGPIOType == SS_GPIO)
    {
        uint32_t reg = p->ulGPIOBase + SS_GPIO_SWPORTA_DR;
        if (val)
            SET_ARC_BIT(reg, p->ulGPIOId);
        else
            CLEAR_ARC_BIT(reg, p->ulGPIOId);
    }
    else if (p->ulGPIOType == SOC_GPIO)
    {
        uint32_t reg = p->ulGPIOBase + SOC_GPIO_SWPORTA_DR;
        if (val)
            SET_MMIO_BIT(reg, p->ulGPIOId);
        else
            CLEAR_MMIO_BIT(reg, p->ulGPIOId);
    }
}

int digitalRead( uint8_t pin )
{
    PinDescription *p = &g_APinDescription[pin];

    if (pin >= NUM_DIGITAL_PINS) return LOW;

    if (p->ulGPIOType == SS_GPIO)
    {
        uint32_t reg = p->ulGPIOBase + SS_GPIO_EXT_PORTA;
        if (READ_ARC_REG(reg) & (1 << p->ulGPIOId)) return HIGH;
    }
    else if (p->ulGPIOType == SOC_GPIO)
    {
        uint32_t reg = p->ulGPIOBase + SOC_GPIO_EXT_PORTA;
        if (MMIO_REG_VAL(reg) & (1 << p->ulGPIOId)) return HIGH;
    }

    return LOW;
}

#ifdef __cplusplus
}
#endif
