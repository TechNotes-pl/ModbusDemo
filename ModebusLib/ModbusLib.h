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

extern "C" MODBUSLIB_API statusCode InitializeDevice(int param);
extern "C" MODBUSLIB_API threadState GetDeviceStatus();
extern "C" MODBUSLIB_API bool EnableDevice();
extern "C" MODBUSLIB_API void DisableDevice();
extern "C" MODBUSLIB_API void CloseDevice();
