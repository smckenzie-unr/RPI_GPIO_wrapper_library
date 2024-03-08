/*! \file gpioDriver.h
    \brief The header definitions for the gpioDriver source code.
    
    \author Scott L. McKenzie
    \date 2024/02/17
    \version 0.0.1

    \details 
*/

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdexcept>
#include <filesystem>
#include <string.h>

#include "../inc/gpioDrive.h"

#ifdef NULL
#undef NULL
#define NULL (0)
#endif

#ifdef TRUE
#undef TRUE
#endif

#ifdef FALSE
#undef FALSE
#endif

#define TRUE (1 == 1)
#define FALSE (1 == 0)

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

gpioDriver::gpioDriver(const uint& pinNumber,  
                       const std::string& gpioChip, 
                       pin_type pinDir) noexcept(false)
: m_outputState(false)
, m_gpioPin(pinNumber)
, m_gpioChip(nullptr)
, m_gpioLine(nullptr)
, m_pinType(pinDir)
{
    if(!gpioChip.size())
    {
        throw GPIO_ERROR_INVALID_ARGUMENT_CHIP_STRING_SIZE_ZERO;
    }

    m_gpioChip = gpiod_chip_open_by_name(gpioChip.c_str());
    if(!m_gpioChip)
    {
        throw GPIO_ERROR_FAILED_TO_CREATE_GPIO_CHIP_STRUCT;
    }
    
    m_gpioLine = gpiod_chip_get_line(m_gpioChip, m_gpioPin);
    if(!m_gpioLine)
    {
        gpiod_chip_close(m_gpioChip);
        throw GPIO_ERROR_FAILED_TO_CREATE_GPIO_LINE_STRUCT;
    }

    const std::string driverID("gpioDriverObject");
    if(m_pinType == pin_type::output_pin)
    {
        if(gpiod_line_request_output(m_gpioLine, driverID.c_str(), NULL) < NULL)
        {
            gpiod_chip_close(m_gpioChip);
            gpiod_line_release(m_gpioLine);
            throw GPIO_ERROR_FAILED_OUTPUT_LINE_REQUEST_FAILED;
        }
    }
    else if(m_pinType == pin_type::input_pin)
    {
        if(gpiod_line_request_input(m_gpioLine, driverID.c_str()) < NULL)
        {
            gpiod_chip_close(m_gpioChip);
            gpiod_line_release(m_gpioLine);
            throw GPIO_ERROR_FAILED_INPUT_LINE_REQUEST_FAILED;
        }
    }
}
gpioDriver::~gpioDriver(void) noexcept(false)
{
    gpiod_chip_close(m_gpioChip);
    gpiod_line_release(m_gpioLine);

    m_gpioChip = nullptr;
    m_gpioChip = nullptr;
}

void gpioDriver::set_pin_direction(pin_type pinDir) noexcept(false)
{
    if(pinDir != m_pinType)
    {
        const std::string driverID("gpioDriverObject");
        if(m_pinType == pin_type::output_pin)
        {
            if(gpiod_line_request_output(m_gpioLine, driverID.c_str(), NULL) < NULL)
            {
                throw GPIO_ERROR_FAILED_OUTPUT_LINE_REQUEST_FAILED;
            }
        }
        else if(m_pinType == pin_type::input_pin)
        {
            if(gpiod_line_request_input(m_gpioLine, driverID.c_str()) < NULL)
            {
                throw GPIO_ERROR_FAILED_INPUT_LINE_REQUEST_FAILED;
            }
        }
    }
}
    
void gpioDriver::set_output_pin(void) noexcept(false)
{
    if(!m_outputState)
    {
        m_outputState = true;
        if(gpiod_line_set_value(m_gpioLine, TRUE) < NULL) 
        {
            throw GPIO_ERROR_COULD_NOT_SET_LINE_VALUE_TRUE;
        }
    }
}
void gpioDriver::reset_output_pin(void) noexcept(false)
{
    if(m_outputState)
    {
        m_outputState = false;
        if(gpiod_line_set_value(m_gpioLine, FALSE) < NULL) 
        {
            throw GPIO_ERROR_COULD_NOT_SET_LINE_VALUE_FALSE;
        }
    }
}
void gpioDriver::toggle_output_pin(void) noexcept(false)
{
    if(m_outputState)
    {
        m_outputState = false;
        if(gpiod_line_set_value(m_gpioLine, FALSE) < NULL) 
        {
            throw GPIO_ERROR_COULD_NOT_SET_LINE_VALUE_FALSE;
        }
    }
    else
    {
        m_outputState = true;
        if(gpiod_line_set_value(m_gpioLine, TRUE) < NULL) 
        {
            throw GPIO_ERROR_COULD_NOT_SET_LINE_VALUE_TRUE;
        }
    }
}
void gpioDriver::output_pin_state(bool state) noexcept(false)
{
    // TODO: Not sure if a function calling this object method in a try-catch block
    // will catch an error thrown by another called object method in the callee method...
    // Might need a try-catch block around this method calls and just re-throw the 
    // exception if it is thrown...
    if(state)
    {
        this->reset_output_pin();
    }
    else
    {
        this->set_output_pin();
    }
}

bool gpioDriver::read_input_pin(void) noexcept(false)
{
    bool __return(false);
    int check(-1);
    
    if(m_pinType == pin_type::input_pin)
    {
        check = gpiod_line_get_value(m_gpioLine);
        if(check < NULL)
        {
            throw GPIO_ERROR_COULD_NOT_READ_INPUT_LINE;
        }
        else if(check)
        {
            __return = true;
        }
    }

    return __return;
}

std::deque<std::string> gpioDriver::get_availible_chips(void) noexcept
{
    namespace fs = std::filesystem;

    std::string filePath("/dev");
    std::deque<std::string> devices;

    for (const auto& entry : fs::directory_iterator(filePath))
    {
        if(entry.path().filename().generic_string().find(std::string("gpiochip")) != std::string::npos)
        {
            devices.push_back(entry.path().filename().generic_string());
        }
    }

    return devices;
}