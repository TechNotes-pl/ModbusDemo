// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

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


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
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

/* Demo app unused staff */
//
///* ----------------------- Static functions ---------------------------------*/
//static BOOL     bCreatePollingThread(void);
//static enum ThreadState eGetPollingThreadState(void);
//static void     eSetPollingThreadState(enum ThreadState eNewState);
//static DWORD WINAPI dwPollingThread(LPVOID lpParameter);
//
//int
//_tmain(int argc, _TCHAR* argv[])
//{
//    int             iExitCode;
//    TCHAR           cCh;
//    BOOL            bDoExit;
//
//    if (eMBTCPInit(MB_TCP_PORT_USE_DEFAULT) != MB_ENOERR)
//    {
//        _ftprintf(stderr, _T("%s: can't initialize modbus stack!\r\n"), PROG);
//        iExitCode = EXIT_FAILURE;
//    }
//    else
//    {
//        /* Create synchronization primitives and set the current state
//         * of the thread to STOPPED.
//         */
//        InitializeCriticalSection(&hPollLock);
//        eSetPollingThreadState(STOPPED);
//
//        /* CLI interface. */
//        _tprintf(_T("Type 'q' for quit or 'h' for help!\r\n"));
//        bDoExit = FALSE;
//        do
//        {
//            _tprintf(_T("> "));
//            cCh = _gettchar();
//            switch (cCh)
//            {
//            case _TCHAR('q'):
//                bDoExit = TRUE;
//                break;
//            case _TCHAR('d'):
//                eSetPollingThreadState(SHUTDOWN);
//                break;
//            case _TCHAR('e'):
//                if (bCreatePollingThread() != TRUE)
//                {
//                    _tprintf(_T("Can't start protocol stack! Already running?\r\n"));
//                }
//                break;
//            case _TCHAR('s'):
//                switch (eGetPollingThreadState())
//                {
//                case RUNNING:
//                    _tprintf(_T("Protocol stack is running.\r\n"));
//                    break;
//                case STOPPED:
//                    _tprintf(_T("Protocol stack is stopped.\r\n"));
//                    break;
//                case SHUTDOWN:
//                    _tprintf(_T("Protocol stack is shuting down.\r\n"));
//                    break;
//                }
//                break;
//            case _TCHAR('h'):
//                _tprintf(_T("FreeModbus demo application help:\r\n"));
//                _tprintf(_T("  'd' ... disable protocol stack.\r\n"));
//                _tprintf(_T("  'e' ... enabled the protocol stack\r\n"));
//                _tprintf(_T("  's' ... show current status\r\n"));
//                _tprintf(_T("  'q' ... quit applicationr\r\n"));
//                _tprintf(_T("  'h' ... this information\r\n"));
//                _tprintf(_T("\r\n"));
//                _tprintf(_T("Copyright 2006 Christian Walter <wolti@sil.at>\r\n"));
//                break;
//            default:
//                if (cCh != _TCHAR('\n'))
//                {
//                    _tprintf(_T("illegal command '%c'!\r\n"), cCh);
//                }
//                break;
//            }
//
//            /* eat up everything untill return character. */
//            while (cCh != '\n')
//            {
//                cCh = _gettchar();
//            }
//        } while (!bDoExit);
//
//        /* Release hardware resources. */
//        (void)eMBClose();
//        iExitCode = EXIT_SUCCESS;
//    }
//    return iExitCode;
//}
//
//BOOL
//bCreatePollingThread(void)
//{
//    BOOL            bResult;
//
//    if (eGetPollingThreadState() == STOPPED)
//    {
//        if ((hPollThread = CreateThread(NULL, 0, dwPollingThread, NULL, 0, NULL)) == NULL)
//        {
//            /* Can't create the polling thread. */
//            bResult = FALSE;
//        }
//        else
//        {
//            bResult = TRUE;
//        }
//    }
//    else
//    {
//        bResult = FALSE;
//    }
//
//    return bResult;
//}
//
//DWORD           WINAPI
//dwPollingThread(LPVOID lpParameter)
//{
//    eSetPollingThreadState(RUNNING);
//
//    if (eMBEnable() == MB_ENOERR)
//    {
//        do
//        {
//            if (eMBPoll() != MB_ENOERR)
//                break;
//        } while (eGetPollingThreadState() != SHUTDOWN);
//    }
//
//    (void)eMBDisable();
//
//    eSetPollingThreadState(STOPPED);
//
//    return 0;
//}
//
//enum ThreadState
//    eGetPollingThreadState()
//{
//    enum ThreadState eCurState;
//
//    EnterCriticalSection(&hPollLock);
//    eCurState = ePollThreadState;
//    LeaveCriticalSection(&hPollLock);
//
//    return eCurState;
//}
//
//void
//eSetPollingThreadState(enum ThreadState eNewState)
//{
//    EnterCriticalSection(&hPollLock);
//    ePollThreadState = eNewState;
//    LeaveCriticalSection(&hPollLock);
//}

