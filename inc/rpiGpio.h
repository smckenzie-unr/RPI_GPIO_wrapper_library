#pragma once
#ifndef __RPIGPIO_H__
#define __RPIGPIO_H__

/*! \file       rpiGpio.h
    \brief      The header definitions for the raspberry pi 5 j8 header. Inherets from gpioDriver.h
    
    \author     Scott L. McKenzie
    \date       2024/02/17
    \version    0.0.1

    \details    The J8 header on the raspberry pi 5 is as follows:

                function    J8 Pin  ||  J8 Pin      function
                3V3         [1]     ||  [2]         5V
                GPIO2       [3]     ||  [4]         5V
                GPIO3       [5]     ||  [6]         GND
                GPIO4       [7]     ||  [8]         GPIO14
                GND         [9]     ||  [10]        GPIO15
                GPIO17      [11]    ||  [12]        GPIO18
                GPIO27      [13]    ||  [14]        GND
                GPIO22      [15]    ||  [16]        GPIO23
                3V3         [17]    ||  [18]        GPIO24
                GPIO10      [19]    ||  [20]        GND
                GPIO9       [21]    ||  [22]        GPIO25
                GPIO11      [23]    ||  [24]        GPIO8
                GND         [25]    ||  [26]        GPIO7
                GPIO0       [27]    ||  [28]        GPIO1
                GPIO5       [29]    ||  [30]        GND
                GPIO6       [31]    ||  [32]        GPIO12
                GPIO13      [33]    ||  [34]        GND
                GPIO19      [35]    ||  [36]        GPIO16
                GPIO26      [37]    ||  [38]        GPIO20
                GND         [39]    ||  [40]        GPIO21
*/

#include <vector>

#include "gpioDrive.h"

enum class RPI5_J8HEADER : uint64_t
{
    PIN_3   = 2,
    PIN_5   = 3,
    PIN_7   = 4,
    PIN_8   = 14,
    PIN_10  = 15,
    PIN_11  = 17,
    PIN_12  = 18,
    PIN_13  = 27,
    PIN_15  = 22,
    PIN_16  = 23,
    PIN_18  = 24,
    PIN_19  = 10,
    PIN_21  = 9,
    PIN_22  = 25,
    PIN_23  = 11,
    PIN_24  = 8,
    PIN_26  = 7,
    PIN_27  = 0,
    PIN_28  = 1,
    PIN_29  = 5,
    PIN_31  = 6,
    PIN_32  = 12,
    PIN_33  = 13,
    PIN_35  = 19,
    PIN_36  = 16,
    PIN_37  = 26,
    PIN_38  = 20,
    PIN_40  = 21
};

class raspberryPiGpio final : public gpioDriver
{
public:
    raspberryPiGpio(const raspberryPiGpio &) = delete;
    raspberryPiGpio(raspberryPiGpio &&) = delete;
    explicit raspberryPiGpio(RPI5_J8HEADER pin, pin_type pinDir = pin_type::output_pin) noexcept(false);
    ~raspberryPiGpio(void) noexcept override;

    raspberryPiGpio& operator =(const raspberryPiGpio &) = delete;
    raspberryPiGpio& operator =(raspberryPiGpio &&) = delete;

    static std::vector<RPI5_J8HEADER> get_opened_pins(void) noexcept;
    RPI5_J8HEADER this_pin(void) noexcept;

private:
    const RPI5_J8HEADER m_rpiPin;
    static uint64_t m_openedPins;
};

#endif //__RPIGPIO_H__