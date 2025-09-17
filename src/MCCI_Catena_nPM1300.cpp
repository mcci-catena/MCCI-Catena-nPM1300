/*

Module: MCCI_Catena_nPM1300.cpp

Function:
    Implementation code for MCCI Catena nPM1300 power management IC library.

Copyright and License:
    See accompanying LICENSE file for copyright and license information.

Author:
    Pranau R, MCCI Corporation   May 2025

*/

#include "MCCI_Catena_nPM1300.h"
#include <Arduino.h>
#include <Wire.h>

using namespace McciCatenaNpm1300;

cNPM1300::cNPM1300()
    {
    //default constructor
    }

cNPM1300::~cNPM1300()
    {
    }

bool cNPM1300::begin()
    {
    // if no Wire is bound, fail.
    if (this->m_wire == nullptr)
        {
        this->setLastError(Error::NoWire);
        return false;
        }

    if (this->isRunning())
        {
        return true;
        }

    this->m_wire->begin();
    this->setState(State::Initial);

    bool result = this->reset();
    return result;
    }

void cNPM1300::end()
    {
    if (this->isRunning())
        {
        this->setState(State::End);
        }
    }

bool cNPM1300::reset()
    {
    if (this->writeRegister(Register::TaskSwReset, (uint8_t)Command::Trigger))
        {
        return true;
        }
    else
        {
        this->setState(State::Uninitialized);
        return false;
        }
    }

float cNPM1300::measureVbat()
    {
    uint8_t vBatMsb;
    uint8_t vBatLsb;

    this->writeRegister(Register::TaskVbatMeasure, (uint8_t)Command::Trigger);

    this->readRegister(Register::AdcVbatResultMsb, vBatMsb);
    this->readRegister(Register::AdcGpResultLsbs_0, vBatLsb);
    // We need only data from bit 0 and 1
    vBatLsb = (vBatLsb & 0x03);
    // Get the 10 bit ADC result
    uint16_t result = (vBatMsb << 2) | vBatLsb;

    return (result / 1023.0) * 5.0;
    }

float cNPM1300::measureVbus()
    {
    uint8_t vBusMsb;
    uint8_t vBusLsb;

    this->writeRegister(Register::TaskVbusMeasure_7, (uint8_t)Command::Trigger);

    this->readRegister(Register::AdcVbatResultMsb_3, vBusMsb);
    this->readRegister(Register::AdcGpResultLsbs_1, vBusLsb);
    // We need only data from bit 0 and 1
    vBusLsb = (vBusLsb & 0x03);
    // Get the 10 bit ADC result
    uint16_t result = (vBusMsb << 2) | vBusLsb;

    return (result / 1023.0) * 7.5;
    }

float cNPM1300::measureVsys()
    {
    uint8_t vSysMsb;
    uint8_t vSysLsb;

    this->writeRegister(Register::TaskVsysMeasure, (uint8_t)Command::Trigger);

    this->readRegister(Register::AdcVsysResultMsb, vSysMsb);
    this->readRegister(Register::AdcGpResultLsbs_0, vSysLsb);
    // We need only data from bit 6 and 7
    vSysLsb = (vSysLsb & 0xC0) >> 6;
    // Get the 10 bit ADC result
    uint16_t result = (vSysMsb << 2) | vSysLsb;

    return (result / 1023.0) * 6.375;
    }

float cNPM1300::measureTemperature()
    {
    uint8_t tempMsb;
    uint8_t tempLsb;

    this->writeRegister(Register::TaskTempMeasure, (uint8_t)Command::Trigger);

    this->readRegister(Register::AdcTempResultMsb, tempMsb);
    this->readRegister(Register::AdcGpResultLsbs_0, tempLsb);
    // We need only data from bit 5 and 4
    tempLsb = (tempLsb & 0x30) >> 4;
    // Get the 10 bit ADC result
    uint16_t result = (tempMsb << 2) | tempLsb;

    return 394.67 - (0.7926 * result);
    }

uint8_t cNPM1300::getVbusStatus()
    {
    uint8_t vBusStatus;

    this->readRegister(Register::VbusInStatus, vBusStatus);
    return vBusStatus;
    }

bool cNPM1300::setBuck_1(bool state)
    {
    bool result;
    if (state == true)
        {
        // enable the Buck 1
        result = this->writeRegister(Register::BuckEnaSet_1, (uint8_t)Command::Trigger);
        }
    else
        {
        // disable the Buck 1
        result = this->writeRegister(Register::BuckEnaClr_1, (uint8_t)Command::Trigger);
        }

    return result;
    }

bool cNPM1300::setBuck_2(bool state)
    {
    bool result;
    if (state == true)
        {
        // enable the Buck 2
        result = this->writeRegister(Register::BuckEnaSet_2, (uint8_t)Command::Trigger);
        }
    else
        {
        // disable the Buck 2
        result = this->writeRegister(Register::BuckEnaClr_2, (uint8_t)Command::Trigger);
        }

    return result;
    }

bool cNPM1300::setBuckVoltage_1(cNPM1300::Voltage voltage)
    {
    bool result;
    result = this->writeRegister(Register::BuckNormVout_1, (uint8_t)voltage);
    // enable bit 0 to override the voltage selection
    this->writeRegister(Register::BuckSwCtrlSel, 0x01);

    return result;
    }

bool cNPM1300::setBuckVoltage_2(cNPM1300::Voltage voltage)
    {
    bool result;
    result = this->writeRegister(Register::BuckNormVout_2, (uint8_t)voltage);
    // enable bit 1 to override the voltage selection
    this->writeRegister(Register::BuckSwCtrlSel, 0x02);

    return result;
    }

bool cNPM1300::selectLdoLoadSwitchMode_1(bool mode)
    {
    bool result;
    // mode true - LDO mode, slection false - Load Switch mode
    if (mode)
        {
        result = this->writeRegister(Register::LdswLdoSel_1, (uint8_t)mode);
        }
    else
        {
        result = this->writeRegister(Register::LdswLdoSel_1, (uint8_t)mode);
        }

    return result;
    }

bool cNPM1300::selectLdoLoadSwitchMode_2(bool mode)
    {
    bool result;
    // mode true - LDO mode, slection false - Load Switch mode
    if (mode)
        {
        result = this->writeRegister(Register::LdswLdoSel_2, (uint8_t)mode);
        }
    else
        {
        result = this->writeRegister(Register::LdswLdoSel_2, (uint8_t)mode);
        }

    return result;
    }

bool cNPM1300::setLoadSwitch_1(bool state)
    {
    bool result;
    if (state == true)
        {
        // enable the load switch 1
        result = this->writeRegister(Register::TaskLdswSet_1, (uint8_t)Command::Trigger);
        }
    else
        {
        // disable the load switch 1
        result = this->writeRegister(Register::TaskLdswClr_1, (uint8_t)Command::Trigger);
        }

    return result;
    }

bool cNPM1300::setLoadSwitch_2(bool state)
    {
    bool result;
    if (state == true)
        {
        // enable the load switch 2
        result = this->writeRegister(Register::TaskLdswSet_2, (uint8_t)Command::Trigger);
        }
    else
        {
        // disable the load switch 2
        result = this->writeRegister(Register::TaskLdswClr_2, (uint8_t)Command::Trigger);
        }

    return result;
    }

bool cNPM1300::setLdoVoltage_1(cNPM1300::Voltage voltage)
    {
    bool result;
    result = this->writeRegister(Register::LdswVoutSel_1, (uint8_t)voltage);

    return result;
    }

bool cNPM1300::setLdoVoltage_2(cNPM1300::Voltage voltage)
    {
    bool result;
    result = this->writeRegister(Register::LdswVoutSel_2, (uint8_t)voltage);

    return result;
    }

bool cNPM1300::ledSetMode(cNPM1300::Led led, cNPM1300::LedMode mode)
    {
    bool result;
    static const Register reg[3] = {
        Register::LedDrvModeSel_0,
        Register::LedDrvModeSel_1,
        Register::LedDrvModeSel_2
        };

    result = this->writeRegister(reg[(uint8_t)led], (uint8_t)mode);

    return result;
    }

bool cNPM1300::ledOn(cNPM1300::Led led)
    {
    bool result;
    static const Register reg[3] = {
        Register::LedDrvSet_0,
        Register::LedDrvSet_1,
        Register::LedDrvSet_2
        };

    result = this->writeRegister(reg[(uint8_t)led], (uint8_t)Command::Trigger);
    this->ledState[(uint8_t)led] = true;

    return result;
    }

bool cNPM1300::ledOff(cNPM1300::Led led)
    {
    bool result;
    static const Register reg[3] = {
        Register::LedDrvClr_0,
        Register::LedDrvClr_1,
        Register::LedDrvClr_2
        };

    result = this->writeRegister(reg[(uint8_t)led], (uint8_t)Command::Trigger);
    this->ledState[(uint8_t)led] = false;

    return result;
    }

bool cNPM1300::ledSetState(cNPM1300::Led led, bool state)
    {
    bool result;
    if(state)
        {
        result = ledOn(led);
        }
    else
        {
        result = ledOff(led);
        }

    return result;
    }

bool cNPM1300::ledGetState(cNPM1300::Led led)
    {
    return this->ledState[(uint8_t)led];
    }

bool cNPM1300::ledToggle(cNPM1300::Led led)
    {
    bool result;
    this->ledState[(uint8_t)led] = !this->ledState[(uint8_t)led];

    result = ledSetState(led, this->ledState[(uint8_t)led]);

    return result;
    }

bool cNPM1300::setInCurrentLimitVbus(bool limit)
    {
    bool result;
    result = this->writeRegister(Register::TaskUpdateILimSw, (uint8_t)limit);

    return result;
    }

bool cNPM1300::setStartupInCurrentLimitVbus(cNPM1300::Current limit)
    {
    bool result;
    result = this->writeRegister(Register::VbusInILimStartup, (uint8_t)limit);

    return result;
    }

bool cNPM1300::suspendVbus(bool mode)
    {
    bool result;
    result = this->writeRegister(Register::VbusSuspend, (uint8_t)mode);

    return result;
    }

uint8_t cNPM1300::getVbusComparatorStatus()
    {
    uint8_t vBusComparatorStatus;
    this->readRegister(Register::UsbCDetectStatus, vBusComparatorStatus);
    return vBusComparatorStatus;
    }

bool cNPM1300::chargerOn()
    {
    bool result_1, result_2;
    // enable bit 1
    result_1 = this->writeRegister(Register::BChgDisableSet, 0x02);
    result_2 = this->writeRegister(Register::BChgEnableSet, (uint8_t)Command::Trigger);

    return (result_1 && result_2);
    }

bool cNPM1300::ignoreNtc(bool ignore)
    {
    bool result;
    if (ignore)
        {
        // enable bit 1
        result = this->writeRegister(Register::BChgDisableSet, 0x02);
        }
    else
        {
        result = this->writeRegister(Register::BChgDisableClr, (uint8_t)Command::Trigger);
        }

    return result;
    }

bool cNPM1300::enableRecharge(bool enable)
    {
    bool result;
    if (enable)
        {
        result = this->writeRegister(Register::BChgDisableClr, (uint8_t)Command::Trigger);
        }
    else
        {
        result = this->writeRegister(Register::BChgDisableSet, (uint8_t)(Command::Trigger));
        }

    return result;
    }

bool cNPM1300::chargerOff()
    {
    bool result;
    result = this->writeRegister(Register::BChgEnableClr, (uint8_t)Command::Trigger);

    return result;
    }

uint8_t cNPM1300::getVterm()
    {
    uint8_t vTerm;
    this->readRegister(Register::BChgVTerm, vTerm);

    return vTerm;
    }

bool cNPM1300::setVterm(cNPM1300::Vterm vTerm)
    {
    bool result_1, result_2;
    // set voltage at which to stop charging
    result_1 = this->writeRegister(Register::BChgVTerm, (uint8_t)vTerm);
    // set voltage at which to stop charging when the die temp is warm
    result_2 = this->writeRegister(Register::BChgVTermR, (uint8_t)vTerm);

    return (result_1 && result_2);
    }

uint8_t cNPM1300::getChargeStatus()
    {
    uint8_t status;
    this->readRegister(Register::BChgChargeStatus, status);

    return status;
    }

uint8_t cNPM1300::getErrorReason()
    {
    uint8_t errReason;
    this->readRegister(Register::BChgErrReason, errReason);

    return errReason;
    }

uint8_t cNPM1300::getErrorSensor()
    {
    uint8_t errSensor;
    this->readRegister(Register::BChgErrSensor, errSensor);

    return errSensor;
    }

bool cNPM1300::setChargeCurrent(uint16_t limit)
    {
    bool result_1, result_2;

    // make sure value is between 32 and 800
    if(limit < 32)
        {
        limit = 32;
        }
    if(limit > 800)
        {
        limit = 800;
        }

    uint8_t msb = limit/4;
    uint8_t lsb = 1;
    if ((limit/2) % 2)
        {
        lsb = 0;
        }

    result_1 = this->writeRegister(Register::BChgISetMsb, msb);
    result_2 = this->writeRegister(Register::BChgISetLsb, lsb);

    return (result_1 && result_2);
    }

bool cNPM1300::setGpioMode(cNPM1300::GpioMode mode, uint8_t gpioNumber)
    {
    bool result;
    // Validate the GPIO number
    if (gpioNumber >= 5)
        {
        return false;
        }

    static const Register gpioModeRegisters[] =
        {
        Register::GpioMode_0,
        Register::GpioMode_1,
        Register::GpioMode_2,
        Register::GpioMode_3,
        Register::GpioMode_4
        };

    result = this->writeRegister(gpioModeRegisters[gpioNumber], (uint8_t)(mode));
    return result;
    }

bool cNPM1300::setGpioDrive(cNPM1300::GpioDrive current, uint8_t gpioNumber)
    {
    bool result;
    // Validate the GPIO number
    if (gpioNumber >= 5)
        {
        return false;
        }

    static const Register gpioDriveRegisters[] =
        {
        Register::GpioDrive_0,
        Register::GpioDrive_1,
        Register::GpioDrive_2,
        Register::GpioDrive_3,
        Register::GpioDrive_4
        };

    result = this->writeRegister(gpioDriveRegisters[gpioNumber], (uint8_t)(current));
    return result;
    }

bool cNPM1300::setGpioPullUp(bool enable, uint8_t gpioNumber)
    {
    bool result;
    // Validate the GPIO number
    if (gpioNumber >= 5)
        {
        return false;
        }

    static const Register gpioPullUpRegisters[] =
        {
        Register::GpioPuEn_0,
        Register::GpioPuEn_1,
        Register::GpioPuEn_2,
        Register::GpioPuEn_3,
        Register::GpioPuEn_4
        };

    result = this->writeRegister(gpioPullUpRegisters[gpioNumber], (uint8_t)(enable));
    return result;
    }

bool cNPM1300::setGpioPullDown(bool enable, uint8_t gpioNumber)
    {
    bool result;
    // Validate the GPIO number
    if (gpioNumber >= 5)
        {
        return false;
        }

    static const Register gpioPullDownRegisters[] =
        {
        Register::GpioPdEn_0,
        Register::GpioPdEn_1,
        Register::GpioPdEn_2,
        Register::GpioPdEn_3,
        Register::GpioPdEn_4
        };

    result = this->writeRegister(gpioPullDownRegisters[gpioNumber], (uint8_t)(enable));
    return result;
    }

bool cNPM1300::setGpioOpenDrain(bool enable, uint8_t gpioNumber)
    {
    bool result;
    // Validate the GPIO number
    if (gpioNumber >= 5)
        {
        return false;
        }

    static const Register gpioOpenDrainRegisters[] =
        {
        Register::GpioOpenDrain_0,
        Register::GpioOpenDrain_1,
        Register::GpioOpenDrain_2,
        Register::GpioOpenDrain_3,
        Register::GpioOpenDrain_4
        };

    result = this->writeRegister(gpioOpenDrainRegisters[gpioNumber], (uint8_t)(enable));
    return result;
    }

bool cNPM1300::setGpioDebounce(bool enable, uint8_t gpioNumber)
    {
    bool result;
    // Validate the GPIO number
    if (gpioNumber >= 5)
        {
        return false;
        }

    static const Register gpioDebounceRegisters[] =
        {
        Register::GpioDebounce_0,
        Register::GpioDebounce_1,
        Register::GpioDebounce_2,
        Register::GpioDebounce_3,
        Register::GpioDebounce_4
        };

    result = this->writeRegister(gpioDebounceRegisters[gpioNumber], (uint8_t)(enable));
    return result;
    }

uint8_t cNPM1300::getGpioStatus()
    {
    uint8_t gpioStatus;
    this->readRegister(Register::GpioStatus, gpioStatus);

    return gpioStatus;
    }

bool cNPM1300::setLoadSwitchGpioControl_1(uint8_t gpioNumber, bool invert)
    {
    // Validate GPIO number (valid range is 1 to 5, as 0 means no GPIO selected)
    if (gpioNumber > 5)
        {
        return false;
        }

    // Construct the value to write to the register
    std::uint8_t value = (gpioNumber & 0x07) | (invert ? 0x08 : 0x00);

    return this->writeRegister(Register::LdswGpiSel_1, value);
    }

bool cNPM1300::setLoadSwitchGpioControl_2(uint8_t gpioNumber, bool invert)
    {
    // Validate GPIO number (valid range is 1 to 5, as 0 means no GPIO selected)
    if (gpioNumber > 5)
        {
        return false;
        }

    // Construct the value to write to the register
    std::uint8_t value = (gpioNumber & 0x07) | (invert ? 0x08 : 0x00);

    return this->writeRegister(Register::LdswGpiSel_2, value);
    }

bool cNPM1300::setBuckGpioControl(uint8_t buckNumber, uint8_t gpioNumber, bool invert)
    {
    bool result;
    // Validate buck number (must be 1 or 2)
    if (buckNumber < 1 || buckNumber > 2)
        {
        return false;
        }

    // Validate GPIO number (valid range is 0 to 5)
    if (gpioNumber > 5)
        {
        return false;
        }

    // Read the current value of the register
    std::uint8_t regValue;
    if (!this->readRegister(Register::BuckEnCtrl, regValue))
        {
        return false;
        }

    // Construct the new value based on the buck number
    if (buckNumber == 1)
        {
        // Clear and set BUCK1ENGPISEL (Bits 2:0) and BUCK1ENGPIINV (Bit 6)
        // Clear bits 2:0 and 6
        regValue &= ~0x47;
        // Set BUCK1ENGPISEL
        regValue |= (gpioNumber & 0x07);
        // Set BUCK1ENGPIINV
        regValue |= (invert ? 0x40 : 0x00);
        }
    else if (buckNumber == 2)
        {
        // Clear and set BUCK2ENGPISEL (Bits 5:3) and BUCK2ENGPIINV (Bit 7)
        // Clear bits 5:3 and 7
        regValue &= ~0xB8;
        // Set BUCK2ENGPISEL
        regValue |= ((gpioNumber & 0x07) << 3);
        // Set BUCK2ENGPIINV
        regValue |= (invert ? 0x80 : 0x00);
        }

    // Write the updated value back to the register
    result = this->writeRegister(Register::BuckEnCtrl, regValue);
    return result;
    }

// protected
bool cNPM1300::readRegister(cNPM1300::Register reg, std::uint8_t &value)
    {
    return this->readRegisters(reg, &value, 1);
    }

// protected
bool cNPM1300::readRegisters(cNPM1300::Register reg, std::uint8_t *pBuffer, size_t nBuffer)
    {
    if (pBuffer == nullptr || nBuffer > 32)
        return this->setLastError(Error::InternalInvalidParameter);

    this->m_wire->beginTransmission((uint8_t) Address::NPM1300);
    if (this->m_wire->write((uint8_t)((uint16_t)reg >> 8)) != 1 || this->m_wire->write((uint8_t)((uint16_t)reg & 0xFF)) != 1)
        {
        return this->setLastError(Error::I2cReadRequest);
        }
    if (this->m_wire->endTransmission() != 0)
        {
        return this->setLastError(Error::I2cReadRequest);
        }

    auto nReadFrom = this->m_wire->requestFrom((uint8_t) Address::NPM1300, std::uint8_t(nBuffer));

    if (nReadFrom != nBuffer)
        return this->setLastError(Error::I2cReadRequest);
    auto const nResult = unsigned(this->m_wire->available());

    if (nResult > nBuffer)
        return this->setLastError(Error::I2cReadLong);

    for (unsigned i = 0; i < nResult; ++i)
        pBuffer[i] = this->m_wire->read();

    if (nResult != nBuffer)
        return this->setLastError(Error::I2cReadShort);

    return true;
    }

// protected
bool cNPM1300::writeRegister(cNPM1300::Register reg, std::uint8_t value)
    {
    const std::uint8_t cmdbuf[3] = {
        (std::uint8_t)((uint16_t)reg >> 8),
        (std::uint8_t)((uint16_t)reg & 0xFF),
        value
        };

    this->m_wire->beginTransmission((uint8_t) Address::NPM1300);

    if (this->m_wire->write(cmdbuf, sizeof(cmdbuf)) != sizeof(cmdbuf))
        return this->setLastError(Error::I2cWriteBufferFailed);

    if (this->m_wire->endTransmission() != 0)
        return this->setLastError(Error::I2cWriteFailed);

    return true;
    }

/****************************************************************************\
|   String handling for error routines
\****************************************************************************/

static const char *scanMultiSzString(const char *p, unsigned eIndex)
    {
    // iterate based on error index.
    for (; eIndex > 0; --eIndex)
        {
        // stop when we get to empty string
        if (*p == '\0')
            break;
        // otherwise, skip this one.
        p += strlen(p) + 1;
        }

    // if we have a valid string, return it
    if (*p != '\0')
        return p;
    // otherwise indicate that the input wasn't valid.
    else
        return "<<unknown>>";
    }

const char * cNPM1300::getErrorName(cNPM1300::Error e)
    {
    return scanMultiSzString(m_szErrorMessages, unsigned(e));
    }

const char * cNPM1300::getStateName(cNPM1300::State s)
    {
    return scanMultiSzString(m_szStateNames, unsigned(s));
    }

/**** end of MCCI_Catena_nPM1300.cpp ****/