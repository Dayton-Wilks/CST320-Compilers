#pragma once
//**************************************
// cExprNode.h
//
// Defines base class for all expressions
//
// Author: Phil Howard 
// phil.howard@oit.edu
//
// Date: Nov. 28, 2015
//

#include "cAstNode.h"
#include "cStmtNode.h"

class cExprNode : public cStmtNode
{
    public:
        cExprNode() : cStmtNode() {}

        // return the type of the expression
        virtual cDeclNode *GetType() = 0;
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
