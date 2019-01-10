%{
//**************************************
// lang.l
//
// scanner definition file. flex uses this file to create the scanner
//
// Author: Dayton Wilks
//

#include "lex.h"
#include "tokens.h"

// Macros that can be used to create debug output from the scanner
// The first one does NOT create debug output, the second does.
// Uncomment one or the other then use DO_RETURN(val) for all your return 
// statements
//#define DO_RETURN(a) { return (a); }
#define DO_RETURN(a) { return Return(a); }

int Return(int val);

%}

%option noyywrap
%option noinput
%option nounput
%option yylineno


    /* definitions go here. period is an example. */
    /* NOTE: You do not need to use definitions */
period          "."

%%

    // token definitions. I gave "program" as an example
    // Punctuation
"("                         DO_RETURN(40);
")"                         DO_RETURN(41);
"{"                         DO_RETURN(123);
"}"                         DO_RETURN(125);
"["                         DO_RETURN(91);
"]"                         DO_RETURN(93);
";"                         DO_RETURN(59);
","                         DO_RETURN(44);
{period}                    DO_RETURN(46);
"+"                         DO_RETURN(43);
"-"                         DO_RETURN(45);
"*"                         DO_RETURN(42);
"/"                         DO_RETURN(47);
"%"                         DO_RETURN(37);
"="                         DO_RETURN(61);
"!="                        DO_RETURN(NOT_EQUALS);
"=="                        DO_RETURN(EQUALS);
"&&"                        DO_RETURN(AND);
"||"                        DO_RETURN(OR);
"program"                   DO_RETURN(PROGRAM);
"if"                        DO_RETURN(IF);
"else"                      DO_RETURN(ELSE);
"endif"                     DO_RETURN(ENDIF);
"while"                     DO_RETURN(WHILE);
"print"                     DO_RETURN(PRINT);
"int"                       DO_RETURN(INT);
"float"                     DO_RETURN(FLOAT);
"char"                      DO_RETURN(CHAR);
"struct"                    DO_RETURN(STRUCT);
"array"                     DO_RETURN(ARRAY);
"return"                    DO_RETURN(RETURN);
[a-zA-Z_][a-zA-Z0-9_]*      DO_RETURN(IDENTIFIER);
[+-]*[0-9]+                 DO_RETURN(INT_VAL);
[+-]*[0-9]*{period}[0-9]+   DO_RETURN(FLOAT_VAL);
[ \t\n]+                    
"//".*                      
.                          DO_RETURN(JUNK_TOKEN);

%%

// This function allows us to do extra processing on each token
// It is used to generate lexer debug info
int Return(int val)
{
    printf("Scanned '%s': %d\n", yytext, val);
    return val;
}
