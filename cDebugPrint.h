#pragma once

#define DEBUG 1

void DebugPrint(const char *msg)
{
    if (DEBUG) std::cerr << msg << std::endl; 
}