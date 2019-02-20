#pragma once 
#include <iostream>
#include <string>
using std::string;

extern bool g_semanticErrorHappened;

#define CHECK_ERROR() { if (g_semanticErrorHappened) \
{ g_semanticErrorHappened = false; } }

#define PROP_ERROR() { if (g_semanticErrorHappened) \
{ g_semanticErrorHappened = false; YYERROR; } }

// Function that gets called when a semantic error happens
extern void SemanticError(std::string error);