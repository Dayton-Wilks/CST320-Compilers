#pragma once
//**************************************
// cReturnNode.h
//
// Defines an AST node for a return, inherits from stmt 
// so a return can be used anywhere a stmt can.
//
// Author: Dayton Wilks
//

#include "cAstNode.h"
#include "cStmtNode.h"

class cReturnNode : public cStmtNode
{
    public:
        cReturnNode(cExprNode * child) : cStmtNode() 
        {
            AddChild(child);
        }
        virtual string NodeType() { return string("return"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};