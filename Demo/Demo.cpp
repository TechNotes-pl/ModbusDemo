// Demo.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <sstream>
#include <conio.h>

using namespace std;

const char* PROG = "freeModbus";

void Usage();
int InitializeDevice();
void CloseDevice();

void ShowStatus();
void Enable();
void Disable();

int main()
{
    string modestr;
    int mode = 0;
    char selection;

    int status = InitializeDevice();
    if (status != EXIT_SUCCESS)
        return status;

    cout << "Enter mode (0 - rtu, 1 - tcp): ";
    getline(cin, modestr);
    stringstream(modestr) >> mode;
    cout << PROG << ": Mode: " << mode << ". Type 'q' for quit or 'h' for help!\n";

    while ((selection = _getch()) != 'q')
    {
        switch (selection)
        {
        case 'h':
            Usage();
            break;
        case 'd':
            Disable();
            break;
        case 'e':
            Enable();
            break;
        case 's':
            ShowStatus();
            break;
        default:
            break;
        }
    }

    CloseDevice();
    return EXIT_SUCCESS;
}


void Usage()
{
    std::cout << "FreeModbus demo application help:" << endl <<
        "  'd' ... disable protocol stack." << endl <<
        "  'e' ... enabled the protocol stack" << endl <<
        "  's' ... show current status" << endl <<
        "  'q' ... quit application" << endl <<
        "  'h' ... print this information" << endl <<
        "\nCopyright 2020 Technotes-pl <office@technotes.pl>" << endl;
}

int InitializeDevice()
{
    //if (eMBInit(MB_RTU, 0x0A, 1, 38400, MB_PAR_EVEN) != MB_ENOERR)
    //{
    //    std::cerr << PROG << ":" << "Can't initialize modbus stack!\r\n";
    //    return EXIT_FAILURE;
    //}

    //if (eMBSetSlaveID(0x34, TRUE, ucSlaveID, 3) != MB_ENOERR)
    //{
    //    std::cerr << PROG << ":" << "Can't set slave id!\r\n";
    //    return EXIT_FAILURE;
    //}

    ///* Create synchronization primitives and set the current state
    // * of the thread to STOPPED.
    // */
    //InitializeCriticalSection(&hPollLock);
    //eSetPollingThreadState(STOPPED);

    return EXIT_SUCCESS;
}

void CloseDevice()
{
    /* Release hardware resources. */
    //(void)eMBClose();
}

void ShowStatus()
{
    //switch (eGetPollingThreadState())
    //{
    //case RUNNING:
    //    cout << "Protocol stack is running." << endl;
    //    break;
    //case STOPPED:
    //    cout << "Protocol stack is stopped." << endl;
    //    break;
    //case SHUTDOWN:
    //    cout << "Protocol stack is shuting down." << endl;
    //    break;
    //}
    cout << "Not implemented yet." << endl;
}

void Enable()
{
    //if (bCreatePollingThread() != TRUE)
    //{
    //    cout << "Can't start protocol stack! Already running?" << endl;
    //}
    cout << "Is running..." << endl;
}

void Disable()
{
    // eSetPollingThreadState(SHUTDOWN);
    cout << "Stopped..." << endl;
}
