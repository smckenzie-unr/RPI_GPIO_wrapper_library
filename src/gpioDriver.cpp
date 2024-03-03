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

#include "../inc/gpioDrive.h"

#define CLOSE_FD(FD)        \
    do{                     \
        close(FD);          \
        FD = 0;             \
    }while(false)

gpioDriver::gpioDriver(const std::string& pinNumber, pin_type pinDir) noexcept(false)
: m_s4FD(0)
, m_pinType(pinDir)
, m_strPin(pinNumber)
{
    if(m_strPin.size() == 0)
    {
        throw std::invalid_argument("No pin number was given in constructor");
    } 

    std::string pinDirFile(std::string("/sys/class/gpio/gpio") + m_strPin + std::string("/direction"));
    std::string pinValueFile(std::string("/sys/class/gpio/gpio") + m_strPin + std::string("/value"));
    std::string pinDirSet((m_pinType == pin_type::output_pin) ? "out" : "in");

    m_s4FD = open("/sys/class/gpio/export", O_WRONLY);
    if(m_s4FD < 0)
    {
        throw std::runtime_error("Unable to open /sys/class/gpio/export");
    }
    
    if(write(m_s4FD, m_strPin.c_str(), m_strPin.size()) != static_cast<ssize_t>(m_strPin.size()))
    {
        throw std::runtime_error("Error writing to /sys/class/gpio/export");
    }
    
    CLOSE_FD(m_s4FD);

    m_s4FD = open(pinDirFile.c_str(), O_WRONLY);
    if(m_s4FD < 0) 
    {
        throw std::runtime_error(std::string("Unable to open ") + pinDirFile);
    }

    if(write(m_s4FD, pinDirSet.c_str(), pinDirSet.size()) != static_cast<ssize_t>(pinDirSet.size())) 
    {
        throw std::runtime_error(std::string("Error writing to ") + pinDirFile);
    }
    
    CLOSE_FD(m_s4FD);

    m_s4FD = open(pinValueFile.c_str(), O_WRONLY);
    if(m_s4FD < 0)
    {
        throw std::runtime_error(std::string("Unable to open ") + pinValueFile);
    }
}
gpioDriver::gpioDriver(const uint64_t pinNumber, pin_type pinDir) noexcept(false)
: m_s4FD(0)
, m_pinType(pinDir)
, m_strPin(std::to_string(pinNumber))
{
    std::string pinDirFile(std::string("/sys/class/gpio/gpio") + m_strPin + std::string("/direction"));
    std::string pinValueFile(std::string("/sys/class/gpio/gpio") + m_strPin + std::string("/value"));
    std::string pinDirSet((m_pinType == pin_type::output_pin) ? "out" : "in");

    m_s4FD = open("/sys/class/gpio/export", O_WRONLY);
    if(m_s4FD < 0)
    {
        throw std::runtime_error("Unable to open /sys/class/gpio/export");
    }
    
    if(write(m_s4FD, m_strPin.c_str(), m_strPin.size()) != static_cast<ssize_t>(m_strPin.size()))
    {
        throw std::runtime_error("Error writing to /sys/class/gpio/export");
    }
    
    CLOSE_FD(m_s4FD);

    m_s4FD = open(pinDirFile.c_str(), O_WRONLY);
    if(m_s4FD < 0) 
    {
        throw std::runtime_error(std::string("Unable to open ") + pinDirFile);
    }

    if(write(m_s4FD, pinDirSet.c_str(), pinDirSet.size()) != static_cast<ssize_t>(pinDirSet.size())) 
    {
        throw std::runtime_error(std::string("Error writing to ") + pinDirFile);
    }
    
    CLOSE_FD(m_s4FD);

    m_s4FD = open(pinValueFile.c_str(), O_WRONLY);
    if(m_s4FD < 0)
    {
        throw std::runtime_error(std::string("Unable to open ") + pinValueFile);
    }
}
gpioDriver::gpioDriver(const char* __restrict pinNumber, pin_type pinDir) noexcept(false)
: m_s4FD(0)
, m_pinType(pinDir)
, m_strPin(pinNumber)
{
    if(m_strPin.size() == 0)
    {
        throw std::invalid_argument("No pin number was given in constructor");
    } 
    std::string pinDirFile(std::string("/sys/class/gpio/gpio") + m_strPin + std::string("/direction"));
    std::string pinValueFile(std::string("/sys/class/gpio/gpio") + m_strPin + std::string("/value"));
    std::string pinDirSet((m_pinType == pin_type::output_pin) ? "out" : "in");

    m_s4FD = open("/sys/class/gpio/export", O_WRONLY);
    if(m_s4FD < 0)
    {
        throw std::runtime_error("Unable to open /sys/class/gpio/export");
    }
    
    if(write(m_s4FD, m_strPin.c_str(), m_strPin.size()) != static_cast<ssize_t>(m_strPin.size()))
    {
        throw std::runtime_error("Error writing to /sys/class/gpio/export");
    }
    
    CLOSE_FD(m_s4FD);

    m_s4FD = open(pinDirFile.c_str(), O_WRONLY);
    if(m_s4FD < 0) 
    {
        throw std::runtime_error(std::string("Unable to open ") + pinDirFile);
    }

    if(write(m_s4FD, pinDirSet.c_str(), pinDirSet.size()) != static_cast<ssize_t>(pinDirSet.size())) 
    {
        throw std::runtime_error(std::string("Error writing to ") + pinDirFile);
    }
    
    CLOSE_FD(m_s4FD);

    m_s4FD = open(pinValueFile.c_str(), O_WRONLY);
    if(m_s4FD < 0)
    {
        throw std::runtime_error(std::string("Unable to open ") + pinValueFile);
    }
}
gpioDriver::~gpioDriver(void) noexcept(false)
{
    CLOSE_FD(m_s4FD);

    m_s4FD = open("/sys/class/gpio/unexport", O_WRONLY);
    if(m_s4FD < 0) 
    {
        throw std::runtime_error("Unable to open /sys/class/gpio/unexport");
    }

    if(write(m_s4FD, m_strPin.c_str(), m_strPin.size()) != static_cast<ssize_t>(m_strPin.size())) 
    {
        throw std::runtime_error("Error writing to /sys/class/gpio/unexport");
    }

    CLOSE_FD(m_s4FD);
}

void gpioDriver::set_pin_direction(pin_type pinDir) noexcept(false)
{
    if(pinDir != m_pinType)
    {
        std::string pinDirFile(std::string("/sys/class/gpio/gpio") + m_strPin + std::string("/direction"));
        std::string pinValueFile(std::string("/sys/class/gpio/gpio") + m_strPin + std::string("/value"));
        std::string pinDirSet((m_pinType == pin_type::output_pin) ? "out" : "in");

        m_pinType = pinDir;

        CLOSE_FD(m_s4FD);

        m_s4FD = open("/sys/class/gpio/unexport", O_WRONLY);
        if(m_s4FD < 0) 
        {
            throw std::runtime_error("Unable to open /sys/class/gpio/unexport");
        }

        if(write(m_s4FD, m_strPin.c_str(), m_strPin.size()) != static_cast<ssize_t>(m_strPin.size())) 
        {
            throw std::runtime_error("Error writing to /sys/class/gpio/unexport");
        }

        CLOSE_FD(m_s4FD);

        m_s4FD = open(pinDirFile.c_str(), O_WRONLY);
        if(m_s4FD < 0) 
        {
            throw std::runtime_error(std::string("Unable to open ") + pinDirFile);
        }

        if(write(m_s4FD, pinDirSet.c_str(), pinDirSet.size()) != static_cast<ssize_t>(pinDirSet.size())) 
        {
            throw std::runtime_error(std::string("Error writing to ") + pinDirFile);
        }
        
        CLOSE_FD(m_s4FD);

        m_s4FD = open(pinValueFile.c_str(), O_WRONLY);
        if(m_s4FD < 0)
        {
            throw std::runtime_error(std::string("Unable to open ") + pinValueFile);
        }
    }
}
    
void gpioDriver::set_output_pin(void) noexcept(false)
{
    if(write(m_s4FD, "1", 1) != 1) 
    {
        throw std::runtime_error("Error writing to /sys/class/gpio/gpio24/value");
    }
}
void gpioDriver::reset_output_pin(void) noexcept(false)
{
    if(write(m_s4FD, "0", 1) != 1) 
    {
        throw std::runtime_error("Error writing to /sys/class/gpio/gpio24/value");
    }
}
void gpioDriver::toggle_output_pin(void) noexcept(false)
{
    char val(0);
    if(read(m_s4FD, &val, sizeof(char)) < 0)
    {
        throw std::runtime_error(std::string("Error reading /sys/class/gpio/gpio" + m_strPin + std::string("/value")));
    }

    if(val == '0')
    {
        this->set_output_pin();
    }
    else
    {
        this->reset_output_pin();
    }
}
void gpioDriver::output_pin_state(bool state) noexcept(false)
{
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
    char val(0);
    if(read(m_s4FD, &val, sizeof(char)) < 0)
    {
        throw std::runtime_error(std::string("Error reading /sys/class/gpio/gpio" + m_strPin + std::string("/value")));
    }

    return (val == '0') ? false : true;
}