#pragma once
//**************************************
// cDeclNode
//
// Defines base class for all declarations.
// Future labs will add features to this class.
//
// Author: Phil Howard 
// phil.howard@oit.edu
//

#include <string>
using std::string;
#include "cAstNode.h"

class cDeclNode : public cAstNode
{
    public:
        cDeclNode() : cAstNode() {}

        virtual bool IsVar()        { return false; }
        virtual bool IsInt()        { return false; }
        virtual bool IsChar()       { return false; }
        virtual bool IsFloat()      { return false; }
        virtual int GetSize()       { return 0;     }
        virtual bool IsFunc()       { return false; }
        virtual bool IsStruct()     { return false; }
        //virtual bool IsType()       { return false; }
        virtual int ChildCount()    { return NumChildren(); }
        virtual string GetName()    = 0 ;
};
