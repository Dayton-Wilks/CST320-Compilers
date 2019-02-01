%{
//**************************************
// lang.l
//
// scanner definition file. flex uses this file to create the scanner
//
// Author: Dayton Wilks
//
#include <stdlib.h>
#include "lex.h"
//#include "tokens.h"
#include "langparse.h"
#include "cSymbol.h"
#include "cSymbolTable.h"

// Macros that can be used to create debug output from the scanner
// The first one does NOT create debug output, the second does.
// Uncomment one or the other then use DO_RETURN(val) for all your return 
// statements
#define DO_RETURN(a) { return (a); }
//#define DO_RETURN(a) { return Return(a); }

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
"("                         DO_RETURN('(');
")"                         DO_RETURN(')');
"{"                         {
                                //g_symbolTable.IncreaseScope();
                                DO_RETURN(yytext[0]);
                            }
"}"                         {
                                //g_symbolTable.DecreaseScope();
                                DO_RETURN(yytext[0]);
                            }
"["                         DO_RETURN('[');
"]"                         DO_RETURN(']');
";"                         DO_RETURN(';');
","                         DO_RETURN(',');
{period}                    DO_RETURN('.');
"+"                         DO_RETURN('+');
"-"                         DO_RETURN('-');
"*"                         DO_RETURN('*');
"/"                         DO_RETURN('/');
"%"                         DO_RETURN('%');
"="                         DO_RETURN('=');
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
"int"                       DO_RETURN(TYPE_ID);
"float"                     DO_RETURN(TYPE_ID);
"char"                      DO_RETURN(TYPE_ID);
"struct"                    DO_RETURN(STRUCT);
"array"                     DO_RETURN(ARRAY);
"return"                    DO_RETURN(RETURN);
[a-zA-Z_][a-zA-Z0-9_]*      {
                                yylval.symbol = g_symbolTable.FindLocal(yytext);

                                if (yylval.symbol == nullptr)
                                {
                                    yylval.symbol = new cSymbol(yytext);
                                    g_symbolTable.Insert(yylval.symbol);
                                }

                                DO_RETURN(IDENTIFIER);
                            }
[0-9]+                      { yylval.int_val = atoi(yytext); DO_RETURN(INT_VAL); }
[0-9]+{period}[0-9]+        { yylval.float_val = atof(yytext); DO_RETURN(FLOAT_VAL); }
[ \t\n]+                    
"//".*                      
.                           DO_RETURN(JUNK_TOKEN);

%%

// This function allows us to do extra processing on each token
// It is used to generate lexer debug info
int Return(int val)
{
    printf("Scanned '%s': %d\n", yytext, val);
    return val;
}


