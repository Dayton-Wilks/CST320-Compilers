%{
//**************************************
// lang.l
//
// scanner definition file. flex uses this file to create the scanner
//
// Author: Phil Howard 
// phil.howard@oit.edu
//
// Date: Feb. 20, 2016
//

#include "cSymbol.h"
#include "cSymbolTable.h"
#include "lex.h"
#include "astnodes.h"
#include "langparse.h"

// Macros that can be used to create debug output from the scanner
// The first one does NOT create debug output, the second does.
// Uncomment one or the other then use DO_RETURN(val) for all your return 
// statements
#define DO_RETURN(a) { return (a); }
//#define DO_RETURN(a) { return Return(a); }

int Return(int val);
static int Process_Id();

%}

%option noyywrap
%option noinput
%option nounput
%option yylineno


    /* definitions go here. period is an example. */
period          ['.']
whitespace      '[\t\n\r ]'
%%

    // token definitions 
[\t\n\t ]*                  ;   /* Ignore whitespace */
[{}()\[\],;\.+\-*/%=]       { 
                                yylval.int_val = yytext[0];
                                DO_RETURN(yytext[0]);
                            }
"program"                   DO_RETURN(PROGRAM);
"if"                        DO_RETURN(IF);
"else"                      DO_RETURN(ELSE);
"endif"                     DO_RETURN(ENDIF);
"while"                     DO_RETURN(WHILE);
"print"                     DO_RETURN(PRINT);
"int"|"float"|"char"        {
                                yylval.symbol = g_SymbolTable.Find(yytext);
                                DO_RETURN(TYPE_ID);
                            }
"struct"                    DO_RETURN(STRUCT);
"array"                     DO_RETURN(ARRAY);
"return"                    DO_RETURN(RETURN);
[a-zA-Z_][a-zA-Z0-9_]*      { return Process_Id(); }
[0-9]+                      {
                                yylval.int_val = atoi(yytext); 
                                DO_RETURN(INT_VAL); 
                            }
[0-9]+\.[0-9]+              {
                                yylval.float_val = atof(yytext);
                                DO_RETURN(FLOAT_VAL);
                            }
"!="                        { yylval.int_val = NEQUALS; DO_RETURN(NEQUALS); }
"=="                        { yylval.int_val = EQUALS; DO_RETURN(EQUALS); }
"&&"                        { yylval.int_val = AND; DO_RETURN(AND); }
"||"                        { yylval.int_val = OR; DO_RETURN(OR); }
"//".*"\n"                  ;   /* Ignore comments */
.                           DO_RETURN(JUNK_TOKEN);
%%

// This function allows us to do extra processing on each token
// It is used to generate lexer debug info
int Return(int val)
{
    printf("Scanned '%s': %d\n", yytext, val);
    return val;
}

// Process identifiers to see if they are types of identifiers
static int Process_Id()
{
    yylval.symbol = g_SymbolTable.Find(yytext);
    if (yylval.symbol == nullptr)
    {
        yylval.symbol = new cSymbol(yytext); 
    }
    cDeclNode *decl = yylval.symbol->GetDecl();
    if (decl != nullptr && decl->IsType())
    { 
        DO_RETURN(TYPE_ID); 
    }
    else
    { 
        DO_RETURN(IDENTIFIER); 
    }
}
