#pragma once
//**************************************
// cCodeGen.h
//
// Defines a visitor that calculates size and offset
//
// Author: Dayton Wilks
// Date: 3/1/2019
//

#include <string>
using std::string;

#include "cVisitor.h"
#include "emit.h"

class cCodeGen : public cVisitor
{
    public:
        cCodeGen(string filename) : cVisitor()
        {
            InitOutput(filename.c_str());
        }

        ~cCodeGen()
        {
            FinalizeOutput();
        }

        virtual void Visit(cProgramNode* node)
        {
            EmitString("main:\n");
            VisitAllChildren(node);
        }
};