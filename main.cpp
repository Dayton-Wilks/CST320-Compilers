//**************************************
// main.cpp
//
// Main function for lang compiler
//
// Author: Phil Howard 
// phil.howard@oit.edu
//

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "cSymbolTable.h"
#include "lex.h"
#include "astnodes.h"
#include "langparse.h"
#include "cComputeSize.h"
#include "cCodeGen.h"

// define global variables
cSymbolTable g_SymbolTable;
long long cSymbol::nextId;

// takes two string args: input_file, and output_file
int main(int argc, char **argv)
{
    std::cout << "\nDayton wilks" << std::endl;

    std::string outfile_name;
    int result = 0;

    if (argc > 1)
    {
        yyin = fopen(argv[1], "r");
        if (yyin == nullptr)
        {
            std::cerr << "ERROR: Unable to open file " << argv[1] << "\n";
            exit(-1);
        }
    }

    if (argc > 2)
    {
        outfile_name = argv[2];
    } else {
        outfile_name = "langout";
    }

    g_SymbolTable.InitRootTable();

    result = yyparse();
    if (yyast_root != nullptr)
    {
        if (result == 0)
        {
            cComputeSize sizer;
            sizer.VisitAllNodes(yyast_root);

            //output << yyast_root->ToString() << std::endl;

            // need to make the coder go out of scope before assembling
            {
                cCodeGen coder(outfile_name + ".sl");
                coder.VisitAllNodes(yyast_root);
            }

            string cmd = "slasm " + outfile_name + ".sl io320.sl";
            system(cmd.c_str());
        } else {
            std::cerr << yynerrs << " Errors in compile\n";
        }
    }

    if (result == 0 && yylex() != 0)
    {
        std::cerr << "Junk at end of program\n";
    }

    /*
    // close output and fixup cout
    // If these aren't done, you may get a segfault on program exit
    output.close();
    std::cout.rdbuf(cout_buf);
    */

    return result;
}

