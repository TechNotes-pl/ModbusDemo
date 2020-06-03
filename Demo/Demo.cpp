// Demo.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <sstream>
#include <conio.h>

#include "ModbusLib.h"

using namespace std;

const char* PROG = "freeModbus";
const char* VERSION = "0.1";

void Usage(char* arg);
void Version();
void CliMessage();

int Initialize();
void ShowStatus();
void Enable();
void Disable();

int main(int argc, char* argv[])
{
    string modestr;
    int mode = 0;
    char selection;

    // Process command line parameters
    if (argc > 1) {

        for (int i = 1; i < argc; ++i) {
            string arg = argv[i];
            if ((arg == "-h") || (arg == "--help")) {
                Usage(argv[0]);
                return 0;
            }
            else if ((arg == "-v") || (arg == "--version")) {
                Version();
                return 0;
            }
        }
    }

    // Starting CLI
    CliMessage();

    if (Initialize() != EXIT_SUCCESS)
        return -1;

    while ((selection = _getch()) != 'q')
    {
        switch (selection)
        {
        case 'h':
            Usage(NULL);
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

int Initialize()
{
    statusCode status;
    if ((status = InitializeDevice(0)) != statusCode::ST_ENOERR)
    {
        cerr << PROG << ":" << "Can't initialize modbus stack or can't set slave id!" << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
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
        cout << "Protocol stack is shuting down." << endl;
        break;
    }
    cout << "Not implemented yet." << endl;
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
