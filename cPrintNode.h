#pragma once
//**************************************
// cPrintNode.h
//
// Defines AST node for print statements
//
// Inherits from cStmtNode so print statments can go in statement lists, and be
// used anywhere a statement is legal.
//
// Author: Phil Howard 
// phil.howard@oit.edu
//
// Date: Nov. 28, 2015
//

#include "cAstNode.h"
#include "cStmtNode.h"
#include "cExprNode.h"

class cPrintNode : public cStmtNode
{
    public:
        // param is the value to be printed
        cPrintNode(cExprNode *expr) : cStmtNode()
        {
            AddChild(expr);
        }

        // return a string representation of the node
        virtual string NodeType() { return string("print"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
