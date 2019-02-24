#pragma once 
//**************************************
// cSemantics.h
//
// For Semantic error messages
//
// Author: Dayton Wilks
//

#include <iostream>
#include <string>
using std::string;

extern bool g_semanticErrorHappened;

// Function that gets called when a semantic error happens
extern void SemanticError(std::string error);