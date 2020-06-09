// ModbusLib.h - Contains freeModbus implementation
#pragma once

#ifdef MODEBUSLIB_EXPORTS
#define MODBUSLIB_API __declspec(dllexport)
#else
#define MODBUSLIB_API __declspec(dllimport)
#endif

enum class statusCode
{
    ST_ENOERR,                  /*!< no error. */
    ST_ENOREG,                  /*!< illegal register address. */
    ST_EINVAL,                  /*!< illegal argument. */
    ST_EPORTERR,                /*!< porting layer error. */
    ST_ENORES,                  /*!< insufficient resources. */
    ST_EIO,                     /*!< I/O error. */
    ST_EILLSTATE,               /*!< protocol stack in illegal state. */
    ST_ETIMEDOUT                /*!< timeout error occurred. */
};

enum class threadState
{
    IS_STOPPED,
    IS_RUNNING,
    IS_SHUTDOWN
};


/*! \ingroup modbus
 * \brief Modbus serial transmission modes (RTU/ASCII/TCP).
 *
 * Modbus serial supports two transmission modes - ASCII or RTU. RTU
 * is faster but has more hardware requirements and requires a network with
 * a low jitter. ASCII is slower and more reliable on slower links (E.g. modems)
 */
enum class ModbusType
{
    MB_RTU      = 0,            /*!< RTU transmission mode. */
    MB_ASCII    = 1,            /*!< ASCII transmission mode. */
    MB_TCP      = 2             /*!< TCP mode. */
};

extern "C" MODBUSLIB_API bool SetInputRegisterValue(unsigned char address, unsigned char value);

extern "C" MODBUSLIB_API statusCode InitializeDevice(ModbusType modbusMode, unsigned char modbusSlaveAddress, unsigned char serialPortNumber, unsigned long baudRate);
extern "C" MODBUSLIB_API threadState GetDeviceStatus();
extern "C" MODBUSLIB_API bool EnableDevice();
extern "C" MODBUSLIB_API void DisableDevice();
extern "C" MODBUSLIB_API void CloseDevice();
