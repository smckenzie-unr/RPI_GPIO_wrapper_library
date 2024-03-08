#include <stdexcept>

#include "../inc/rpiGpio.h"

uint64_t raspberryPiGpio::m_openedPins(0);

raspberryPiGpio::raspberryPiGpio(RPI5_J8HEADER pin, 
                                 const std::string& gpioChip, 
                                 pin_type pinDir) noexcept(false)
: gpioDriver(static_cast<uint>(pin), gpioChip, pinDir)
, m_rpiPin(pin)
{
    if((m_openedPins & (1 << static_cast<uint64_t>(m_rpiPin))) != static_cast<uint64_t>((1 << static_cast<uint64_t>(m_rpiPin))))
    {
        m_openedPins |= (1 << static_cast<uint64_t>(m_rpiPin));
    }
    else
    {
        throw std::invalid_argument("Pin is already in use.");
    }
}
raspberryPiGpio::~raspberryPiGpio(void) noexcept
{
    m_openedPins &= ~(1 << static_cast<uint64_t>(m_rpiPin));
}

std::vector<RPI5_J8HEADER> raspberryPiGpio::get_opened_pins(void) noexcept
{
    std::vector<RPI5_J8HEADER> usedPins;

    for(uint64_t bitIdx = 0; bitIdx < (sizeof(uint64_t) * 8); bitIdx ++)
    {
        if(m_openedPins & (1 << bitIdx))
        {
            usedPins.push_back(static_cast<RPI5_J8HEADER>(bitIdx));
        }
    }

    return usedPins;
}
RPI5_J8HEADER raspberryPiGpio::this_pin(void) noexcept
{
    return this->m_rpiPin;
}