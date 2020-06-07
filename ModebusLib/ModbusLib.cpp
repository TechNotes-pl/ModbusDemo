// ModbusLib.cpp : Defines the exported functions for the DLL.
#include "pch.h" // use stdafx.h in Visual Studio 2017 and earlier
#include <utility>
#include <limits.h>
#include "ModbusLib.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

/* ----------------------- Defines ------------------------------------------*/
#define REG_INPUT_START 1000
#define REG_INPUT_NREGS 4
#define REG_HOLDING_START 2000
#define REG_HOLDING_NREGS 130

/* ----------------------- Static variables ---------------------------------*/
static USHORT   usRegInputStart = REG_INPUT_START;
static USHORT   usRegInputBuf[REG_INPUT_NREGS];
static USHORT   usRegHoldingStart = REG_HOLDING_START;
static USHORT   usRegHoldingBuf[REG_HOLDING_NREGS];

static HANDLE   hPollThread;
static CRITICAL_SECTION hPollLock;

static enum ThreadState
{
    STOPPED,
    RUNNING,
    SHUTDOWN
} ePollThreadState;

// DLL internal functions:
static BOOL     bCreatePollingThread(void);
static enum ThreadState eGetPollingThreadState(void);
static void     eSetPollingThreadState(enum ThreadState eNewState);
static DWORD WINAPI dwPollingThread(LPVOID lpParameter);

statusCode InitializeDevice(ModbusType modbusMode, unsigned char modbusSlaveAddress, unsigned char serialPortNumber, unsigned long baudRate)
{
    eMBErrorCode ret_val;
    char blah[] = "???";
    eMBMode mode;

    // Convert types: ModbusType -> enum eMBMode
    if (modbusMode == ModbusType::MB_ASCII)
        mode = MB_ASCII;
    else if (modbusMode == ModbusType::MB_RTU)
        mode = MB_RTU;
    else mode = MB_TCP;

    if ((ret_val = eMBInit(mode, modbusSlaveAddress, serialPortNumber, baudRate, MB_PAR_EVEN)) != MB_ENOERR)
        return (statusCode)ret_val;

    if ((ret_val = eMBSetSlaveID(0x34, TRUE, (UCHAR*)blah, 3)) != MB_ENOERR)
        return (statusCode)ret_val;

    // Create synchronization primitives and set the current state
    // of the thread to STOPPED.
    InitializeCriticalSection(&hPollLock);
    eSetPollingThreadState(STOPPED);

    return statusCode::ST_ENOERR;
}


threadState GetDeviceStatus()
{
    return (threadState)eGetPollingThreadState();
}

bool EnableDevice()
{
    return bCreatePollingThread();
}

void DisableDevice()
{
   eSetPollingThreadState(SHUTDOWN);
}

void CloseDevice()
{
    /* Release hardware resources. */
    (void)eMBClose();
}



// DEMO Callbacks

eMBErrorCode
eMBRegInputCB(UCHAR* pucRegBuffer, USHORT usAddress, USHORT usNRegs)
{
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex;

    if ((usAddress >= REG_INPUT_START)
        && (usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS))
    {
        iRegIndex = (int)(usAddress - usRegInputStart);
        while (usNRegs > 0)
        {
            *pucRegBuffer++ = (unsigned char)(usRegInputBuf[iRegIndex] >> 8);
            *pucRegBuffer++ = (unsigned char)(usRegInputBuf[iRegIndex] & 0xFF);
            iRegIndex++;
            usNRegs--;
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }

    return eStatus;
}

eMBErrorCode
eMBRegHoldingCB(UCHAR* pucRegBuffer, USHORT usAddress, USHORT usNRegs, eMBRegisterMode eMode)
{
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex;

    if ((usAddress >= REG_HOLDING_START) &&
        (usAddress + usNRegs <= REG_HOLDING_START + REG_HOLDING_NREGS))
    {
        iRegIndex = (int)(usAddress - usRegHoldingStart);
        switch (eMode)
        {
            /* Pass current register values to the protocol stack. */
        case MB_REG_READ:
            while (usNRegs > 0)
            {
                *pucRegBuffer++ = (UCHAR)(usRegHoldingBuf[iRegIndex] >> 8);
                *pucRegBuffer++ = (UCHAR)(usRegHoldingBuf[iRegIndex] & 0xFF);
                iRegIndex++;
                usNRegs--;
            }
            break;

            /* Update current register values with new values from the
             * protocol stack. */
        case MB_REG_WRITE:
            while (usNRegs > 0)
            {
                usRegHoldingBuf[iRegIndex] = *pucRegBuffer++ << 8;
                usRegHoldingBuf[iRegIndex] |= *pucRegBuffer++;
                iRegIndex++;
                usNRegs--;
            }
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }
    return eStatus;
}

eMBErrorCode
eMBRegCoilsCB(UCHAR* pucRegBuffer, USHORT usAddress, USHORT usNCoils, eMBRegisterMode eMode)
{
    return MB_ENOREG;
}

eMBErrorCode
eMBRegDiscreteCB(UCHAR* pucRegBuffer, USHORT usAddress, USHORT usNDiscrete)
{
    return MB_ENOREG;
}


BOOL
bCreatePollingThread(void)
{
    BOOL            bResult;

    if (eGetPollingThreadState() == STOPPED)
    {
        if ((hPollThread = CreateThread(NULL, 0, dwPollingThread, NULL, 0, NULL)) == NULL)
        {
            /* Can't create the polling thread. */
            bResult = FALSE;
        }
        else
        {
            bResult = TRUE;
        }
    }
    else
    {
        bResult = FALSE;
    }

    return bResult;
}

DWORD           WINAPI
dwPollingThread(LPVOID lpParameter)
{
    eSetPollingThreadState(RUNNING);

    if (eMBEnable() == MB_ENOERR)
    {
        do
        {
            if (eMBPoll() != MB_ENOERR)
                break;
        } while (eGetPollingThreadState() != SHUTDOWN);
    }

    (void)eMBDisable();

    eSetPollingThreadState(STOPPED);

    return 0;
}

enum ThreadState
    eGetPollingThreadState()
{
    enum ThreadState eCurState;

    EnterCriticalSection(&hPollLock);
    eCurState = ePollThreadState;
    LeaveCriticalSection(&hPollLock);

    return eCurState;
}

void
eSetPollingThreadState(enum ThreadState eNewState)
{
    EnterCriticalSection(&hPollLock);
    ePollThreadState = eNewState;
    LeaveCriticalSection(&hPollLock);
}