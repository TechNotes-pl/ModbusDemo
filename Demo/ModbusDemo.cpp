// ModbusDemo.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <conio.h>
#include "ModbusDemo.h"

using namespace std;

int main(const int argc, char* argv[])
{
	auto mode = 0;
    char selection;
    unsigned char serial_port_number = 1;

    // Process command line parameters
    if (argc > 1) {

        for (auto i = 1; i < argc; ++i) {
            string arg = argv[i];
            if (arg == "-h" || arg == "--help") {
                Usage(argv[0]);
                return 0;
            }
            else if ((arg == "-v") || (arg == "--version")) {
                Version();
                return 0;
            }
            else if (arg == "-p") {
                if (++i < argc) {
                    serial_port_number = atoi(argv[i]);
                    if (serial_port_number == 0)
                    {
                        cerr << "Error: Wrong port number, use digits only." << endl;
                        return 0;
                    }
                }                 
                else {
                    cerr << "Error: Port number missing." << endl;
                    return 0;
                }
            }
        }
    }

    // Set sample values to registers    
    SetInputRegisterValue(0, 18);
    SetInputRegisterValue(0x02, 255);

    // Starting CLI
    CliMessage();
    Prompt();

    if (!Initialize(ModbusType::MB_RTU, serial_port_number))
        return -1;

    while ((selection = _getch()) != 'q')
    {
        switch (selection)
        {
        case 'h':
            Usage();
            Prompt();
            break;
        case 'd':
            Disable();
            Prompt();
            break;
        case 'e':
            Enable();
            Prompt();
            break;
        case 's':
            ShowStatus();
            Prompt();
            break;
        default:
            break;
        }
    }

    CloseDevice();
    return 0;
}


void Usage(char *arg)
{
    cout << PROG << " demo application help:" << endl <<
        "  'd' ... disable protocol stack." << endl <<
        "  'e' ... enabled the protocol stack" << endl <<
        "  's' ... show current status" << endl <<
        "  'q' ... quit application" << endl <<
        "  'h' ... print this information" << endl << endl <<
        "Copyright 2020 Technotes-pl <office@technotes.pl>" << endl;
}

void Version()
{
    cout << PROG << " demo application, version " << VERSION << endl;
}

void CliMessage()
{
    cout << PROG << " CLI, version " << VERSION << endl <<
            "Press h for help, q to exit" << endl; 
}

void Prompt()
{
    cout << "> ";
}

/**
 * \brief 
 * \param mode 
 * \return 
 */
bool Initialize(ModbusType mode, unsigned char serial_port_number)
{
    const unsigned char modbus_slave_number = 0x0A;
    const unsigned long baud_rate = 38400;

    statusCode status;
    if ((status = InitializeDevice(mode, modbus_slave_number, serial_port_number, baud_rate)) != statusCode::ST_ENOERR)
    {
        switch (status) {
        case statusCode::ST_EPORTERR:
            cerr << PROG << ": Can't initialize serial port COM" << serial_port_number << endl;
            break;
        default:
            cerr << PROG << ": " << "Error " << static_cast<int>(status) << " - Can't initialize modbus stack or can't set slave id!" << endl;
            break;
        }
        
        return false;
    }

    return true;
}

void ShowStatus()
{
    switch (GetDeviceStatus())
    {
    case threadState::IS_RUNNING:
        cout << "Protocol stack is running." << endl;
        break;
    case threadState::IS_STOPPED:
        cout << "Protocol stack is stopped." << endl;
        break;
    case threadState::IS_SHUTDOWN:
        cout << "Protocol stack is shutting down." << endl;
        break;
    }
}

void Enable()
{
    if (!EnableDevice())
        cerr << "Can't start protocol stack! Already running?" << endl;
    else
        cout << "Is running..." << endl;
}

void Disable()
{
    DisableDevice();
    cout << "Stopped..." << endl;
}
