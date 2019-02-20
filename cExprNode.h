#pragma once
//**************************************
// cExprNode.h
//
// Defines base class for all expressions
//
// This is a pure virtual class because there is no definition for
// cAstNode::ToString()
//
// Author: Phil Howard 
// phil.howard@oit.edu
//

#include "cStmtNode.h"

class cExprNode : public cStmtNode
{
    public:
        cExprNode() : cStmtNode() {}

        cExprNode(cAstNode *left, cAstNode *op, cAstNode *right) : cExprNode()
        {
            AddChild(left);
            AddChild(op);
            AddChild(right);
        }

        // pure virtual
        virtual cDeclNode * GetType() = 0;

        virtual bool IsVar() { return false; }
        virtual bool IsChar() { return false; }
        
        virtual string NodeType() { return string("expr"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
