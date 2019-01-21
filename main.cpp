//**************************************
// main.cpp
//
// Main routine for lang compiler.
// This version only runs the lexer
//
// Author: Phil Howard & Dayton Wilks
// phil.howard@oit.edu
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include "cSymbol.h"
#include "cSymbolTable.h"
#include "lex.h"
#include "tokens.h"

// Define the global symbol table.
// If we had a cSymbolTable.cpp, this would go there
cSymbolTable g_symbolTable;

// Define the cSymbol nextId static member
long long cSymbol::nextId = 0;

// Define the yylval global variable. It contains the semantic value of tokens.
yylval_t yylval;

// Scan a file looking for symbols
int main(int argc, char **argv)
{
    const char *outfile_name;
    int result = 0;
    int token;

    std::cout << "Dayton Wilks" << std::endl;

    if (argc > 1)
    {
        yyin = fopen(argv[1], "r");
        if (yyin == NULL)
        {
            std::cerr << "Unable to open file " << argv[1] << "\n";
            exit(-1);
        }
    }

    if (argc > 2)
        outfile_name = argv[2];
    else
        outfile_name = "/dev/tty";


    FILE *output = fopen(outfile_name, "w");
    if (output == nullptr)
    {
        std::cerr << "ERROR: Unable to open file " << outfile_name << "\n";
        exit(-1);
    }

    // redirect stdout to the output file
    int output_fd = fileno(output);
    if (dup2(output_fd, 1) != 1)
    {
        std::cerr << "Unable to duplicate the file descriptor\n";
        exit(-1);
    }

    std::cout << "<program>\n";

    token = yylex();
    while (token != 0)
    {
        // if we found an identifier, print it out
        if (token == IDENTIFIER) 
            std::cout << yylval.symbol->ToString() << "\n";
        // else
        //     std::cout << token << ":" << yytext << "\n";
        token = yylex();
    }

    std::cout << "</program>\n";

    return result;
}
