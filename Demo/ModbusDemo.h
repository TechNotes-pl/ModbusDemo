#pragma once

#include "ModbusLib.h"

const char* PROG = "freeModbus";
const char* VERSION = "0.1";

void Usage(char* arg = nullptr);
void Version();
void CliMessage();
void Prompt();

bool Initialize(ModbusType mode);
void ShowStatus();
void Enable();
void Disable();