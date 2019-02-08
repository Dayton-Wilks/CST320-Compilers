#pragma once
//**************************************
// cBinaryExprNode.h
//
// Class for creating a binary operation
//
// Author: Dayton Wilks
// Date: 2/8/18

#include "cAstNode.h"

class cBinaryExprNode : public cExprNode
{
    public:
        cBinaryExprNode(cExprNode * left, cOpNode * op, cExprNode * right) : cExprNode(left, op, right) { }
        virtual string NodeType() { return string("expr"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};