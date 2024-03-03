#pragma once
#ifndef __GPIODRIVER_H__
#define __GPIODRIVER_H__

/*! \file       gpioDriver.h
    \brief      The header definitions for the gpioDriver source code.
    
    \author     Scott L. McKenzie
    \date       2024/02/17
    \version    0.0.1

    \details    
*/

#include <string>
#include <cstdint>

enum class pin_type : uint64_t
{
    output_pin,
    input_pin
};

class gpioDriver
{
public:
    gpioDriver(const gpioDriver&) = delete;
    gpioDriver(gpioDriver&&) = delete;
    gpioDriver(const std::string& pinNumber, pin_type pinDir = pin_type::output_pin) noexcept(false);
    gpioDriver(const uint64_t pinNumber, pin_type pinDir = pin_type::output_pin) noexcept(false);
    gpioDriver(const char* __restrict pinNumber, pin_type pinDir = pin_type::output_pin) noexcept(false);
    virtual ~gpioDriver(void) noexcept(false);

    gpioDriver& operator =(const gpioDriver&) = delete;
    gpioDriver& operator =(gpioDriver&&) = delete;

    void set_pin_direction(pin_type pinDir) noexcept(false);

    void set_output_pin(void) noexcept(false);
    void reset_output_pin(void) noexcept(false);
    void toggle_output_pin(void) noexcept(false);
    void output_pin_state(bool state) noexcept(false);

    bool read_input_pin(void) noexcept(false);
    
private:
    signed long int m_s4FD;
    pin_type m_pinType;
    const std::string m_strPin;
};

#endif //__GPIODRIVER_H__