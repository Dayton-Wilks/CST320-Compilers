#pragma once

#define DEBUG 0

void DebugPrint(const char *msg)
{
    if (DEBUG) std::cerr << msg << std::endl; 
}