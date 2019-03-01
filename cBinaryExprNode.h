#pragma once
//**************************************
// cBinaryExprNode.h
//
// Defines AST node for binary expressions.
// Inherits from cExprNode
//
// Author: Phil Howard 
// phil.howard@oit.edu
//
// Date: Nov. 29, 2015
//

#include "cAstNode.h"
#include "cOpNode.h"
#include "cExprNode.h"

class cBinaryExprNode : public cExprNode
{
    public:
        // params are the left and right expressions and the operation.
        // The operation is a char: '+', '-', etc.
        cBinaryExprNode(cExprNode *left, int op, cExprNode *right)
            : cExprNode()
        {
            AddChild(left);
            AddChild(new cOpNode(op));
            AddChild(right);
        }

        // return the type of the expression
        cDeclNode* GetType()
        {
            cExprNode *left = dynamic_cast<cExprNode *>(GetChild(0));
            cExprNode *right= dynamic_cast<cExprNode *>(GetChild(2));
            if (left->GetType() == right->GetType())
                return left->GetType();
            else if (left->GetType()->IsFloat())
                return left->GetType();
            else if (right->GetType()->IsFloat())
                return right->GetType();
            else if (left->GetType()->Sizeof() >=
                     right->GetType()->Sizeof())
                return left->GetType();
            else
                return right->GetType();
        }

        virtual string NodeType() { return string("expr"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
