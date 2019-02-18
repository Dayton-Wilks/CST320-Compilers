#pragma once
//**************************************
// cBinaryExprNode.h
//
// Class for creating a binary operation
//
// Author: Dayton Wilks
// Date: 2/8/18

#include "cAstNode.h"
#include "cSymbolTable.h"

class cBinaryExprNode : public cExprNode
{
    public:
        cBinaryExprNode(cExprNode * left, cOpNode * op, cExprNode * right) : cExprNode(left, op, right) { }

        virtual cDeclNode * GetType() 
        {
            cExprNode * left = dynamic_cast<cExprNode*>(GetChild(0));
            cExprNode * right = dynamic_cast<cExprNode*>(GetChild(2));
            if(left->GetType()->IsFloat() || right->GetType()->IsFloat())
            {
                return g_symbolTable.Find("float")->getDecl();
            }
            else if(left->GetType()->IsInt() || right->GetType()->IsInt())
            {
                return g_symbolTable.Find("int")->getDecl();
            }
            else if(left->GetType()->IsChar() || right->GetType()->IsChar())
            {
                return g_symbolTable.Find("char")->getDecl();
            }
            else
            {
                return nullptr;
            }
        }

        virtual string NodeType() { return string("expr"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};