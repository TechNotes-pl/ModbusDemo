# ModbusDemo for Windows 10
This project contains a demo application to test freeModbus library code, written by Christian Walter. 
Original project is avaliable at https://github.com/cwalter-at/freemodbus, and is not maintened since 2006, so we want to create our own demo application to test freeModbus library ported to MS Windows 10.

Project contains:
  ModbusLib.dll - freemodbus library
  Demo.exe      - Simple client application for Windows 10, x64.

### To build this project Microsoft Visual Studio 2019 is required.
If you need some basic information how to create C++ command line application and DLL library in Visual Studio 2019, please read Microsoft article
https://docs.microsoft.com/en-us/cpp/build/walkthrough-creating-and-using-a-dynamic-link-library-cpp?view=vs-2019

Usage:
  syntax: Demo <<-h|--help> | <-v|--version>>
  Optional parameters:
  -h, --help  Print usage information and exit
  -v, --version Print version information and exit
  
  If called without parameters, CLI starts.

## ModbusLib.dll revision history

Revision 0.1, 3/06/2020
    - original code by Christian Walter used implement Modebus RTU funcionality.
    - simple API designed, see ModbusLib.h
         
