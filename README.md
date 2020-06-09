# ModbusDemo x64 (for Windows 10)
**MODBUS** is a request/response protocol where multiple functions provide access to standardized services. The standardized services include a predefined data model based on discrete bits and registers suitable for representing analog and digital information. If not sufficient this services can be extended using custom function code and application specific data models. MODBUS is located at layer 7 of the Open Systems Interconnection (OSI) model. It supports master / slave communication between devices attached on RS485/RS422 or TCP/IP networks.

This project contains a demo application to test freeModbus library code, written by "@cwalter-at" (Christian Walter). 
Original project is avaliable at [github](https://github.com/cwalter-at/freemodbus), and is not maintened since 2006, so we intend to create our own demo application to test freeModbus library ported to **MS Windows 10**.

Solution contains projects for:
* **ModbusLib.dll** - freemodbus library,
* **Demo.exe**      - Simple client application for Windows 10, x64.

### To build this project Microsoft Visual Studio 2019 is required.
If you need some basic information how to create C++ command line application and DLL library in Visual Studio 2019, please read [Microsoft article](https://docs.microsoft.com/en-us/cpp/build/walkthrough-creating-and-using-a-dynamic-link-library-cpp?view=vs-2019).

## Demo command line application
Usage:

> *syntax:*
>
>**Demo.exe [<-h|--help> | <-v|--version>] [-p <serial_port_number>]**
  
Optional parameters:

*-h, --help* - print usage information and exit,

*-v, --version* - print version information and exit,

*-p 5* - use COM5 to connect with Modbus Master Device (COM1 is used by default).
  
If called without parameters, CLI starts using COM1.


TESTING
=======

1. Use com0com utility to setup vitrual serial ports pair, in this case ports COM5 and COM6 will be used:
![Alt text](doc/com0com-setup.png?raw=true "com0com setup")
2. Open Windows Powershell terminal window and start the **Demo.exe** application for virtual serial port *COM5*. When prompt shows up then press the key **'e'** to enable protocol stack (status message *"Is running..."* appears).

```bash
PS > .\Demo.exe -p 5
freeModbus CLI, version 0.1
Press h for help, q to exit
> Is running...
>
```
3. Start **modpoll** utility for *COM6* to test if the input registers starting at protocol address 1000 can be read. There are four registers values available in Demo application.
The output should look like:

```
PS > ./modpoll.exe -m rtu -a 10 -r 1000 -c 4 -t 3 -b 38400 -d 8 -p even COM6
modpoll 3.7 - FieldTalk(tm) Modbus(R) Master Simulator
Copyright (c) 2002-2019 proconX Pty Ltd
Visit https://www.modbusdriver.com for Modbus libraries and tools.

Protocol configuration: Modbus RTU
Slave configuration...: address = 10, start reference = 1000, count = 4
Communication.........: COM6, 38400, 8, 1, even, t/o 1.00 s, poll rate 1000 ms
Data type.............: 16-bit register, input register table

-- Polling slave... (Ctrl-C to stop)
[1000]: 0
[1001]: 0
[1002]: 0
[1003]: 0
-- Polling slave... (Ctrl-C to stop)
[1000]: 0
[1001]: 0
[1002]: 0
[1003]: 0.
```



ModbusLib.dll revision history
==============================

***Revision 0.1*, 3/06/2020:**
* original code by Christian Walter used implement Modebus RTU funcionality,
* simple API designed, see ModbusLib.h.
         
