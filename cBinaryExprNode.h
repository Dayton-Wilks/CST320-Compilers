#pragma once

#include "cAstNode.h"
#include "cExprNode.h"

class cBinaryExprNode : public cExprNode
{
    public:
        // param is the value of the integer constant
        cBinaryExprNode(cExprNode * left, cOpNode * op, cExprNode * right) : cExprNode(left, op, right)
        {
        }
        virtual string NodeType() { return string("expr"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};