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
#include <gpiod.h>
#include <deque>

enum class pin_type : uint64_t
{
    output_pin,
    input_pin
};

typedef enum error_enum : uint64_t
{
    GPIO_SUCCESS,
    GPIO_ERROR_INVALID_ARGUMENT_CHIP_STRING_SIZE_ZERO,
    GPIO_ERROR_FAILED_TO_CREATE_GPIO_CHIP_STRUCT,
    GPIO_ERROR_FAILED_TO_CREATE_GPIO_LINE_STRUCT,
    GPIO_ERROR_FAILED_OUTPUT_LINE_REQUEST_FAILED,
    GPIO_ERROR_FAILED_INPUT_LINE_REQUEST_FAILED,
    GPIO_ERROR_COULD_NOT_SET_LINE_VALUE_TRUE,
    GPIO_ERROR_COULD_NOT_SET_LINE_VALUE_FALSE,
    GPIO_ERROR_COULD_NOT_READ_INPUT_LINE
}gpioDriverError_t;

class gpioDriver
{
    using gpioChip_t = struct gpiod_chip;
    using gpioLine_t = struct gpiod_line;

public:
    gpioDriver(const gpioDriver&) = delete;
    gpioDriver(gpioDriver&&) = delete;
    gpioDriver(const uint& pinNumber, 
               const std::string& gpioChip, 
               pin_type pinDir = pin_type::output_pin) noexcept(false);
    virtual ~gpioDriver(void) noexcept(false);

    gpioDriver& operator =(const gpioDriver&) = delete;
    gpioDriver& operator =(gpioDriver&&) = delete;

    void set_pin_direction(pin_type pinDir) noexcept(false);

    void set_output_pin(void) noexcept(false);
    void reset_output_pin(void) noexcept(false);
    void toggle_output_pin(void) noexcept(false);
    void output_pin_state(bool state) noexcept(false);

    bool read_input_pin(void) noexcept(false);

    static std::deque<std::string> get_availible_chips(void) noexcept;
    
private:
    bool m_outputState;
    uint m_gpioPin;
    gpioChip_t* m_gpioChip;
    gpioLine_t* m_gpioLine;
    pin_type m_pinType;
    
};

#endif //__GPIODRIVER_H__