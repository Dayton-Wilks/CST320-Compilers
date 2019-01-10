//**************************************
// main.cpp
//
// main routine for lang compiler.
// This version only runs the lexer
//
// Author: Phil Howard 
// phil.howard@oit.edu
//
// Date: Nov. 23, 2015
//

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "lex.h"

int main(int argc, char **argv)
{
    const char *outfile_name;
    int result = 0;
    int token;

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

    std::streambuf *cout_buf = std::cout.rdbuf();
    std::ofstream output(outfile_name);

    if (!output.is_open())
    {
        std::cerr << "Unable to open output file " << outfile_name << "\n";
        exit(-1);
    }
    std::cout.rdbuf(output.rdbuf());

    token = yylex();
    while (token != 0)
    {
        std::cout << token << ":" << yytext << "\n";
        token = yylex();
    }

    output.close();
    std::cout.rdbuf(cout_buf);


    return result;
}
