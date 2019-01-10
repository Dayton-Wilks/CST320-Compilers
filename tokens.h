#pragma once

//**************************************
// tokens.h
//
// Defines the values for the tokens returned by yylex
//
// Author: Phil Howard 
// phil.howard@oit.edu
//

#define PROGRAM     1000        // program
#define IF          1001        // if
#define ELSE        1002        // else
#define ENDIF       1003        // endif
#define WHILE       1004        // while
#define PRINT       1005        // print
#define INT         1006        // int
#define FLOAT       1007        // float
#define CHAR        1008        // char
#define INT_VAL     1009        // <an integer constant>
#define FLOAT_VAL   1010        // <a float constant>
#define STRUCT      1011        // struct
#define ARRAY       1012        // array
#define RETURN      1013        // return
#define IDENTIFIER  1014        // <a C style identifier>
#define NOT_EQUALS  1015        // !=
#define EQUALS      1016        // ==
#define AND         1017        // &&
#define OR          1018        // ||

#define JUNK_TOKEN  2000

