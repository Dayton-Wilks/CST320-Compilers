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
            cExprNode * lExpr = dynamic_cast<cExprNode*>(GetChild(0));
            cExprNode * rExpr = dynamic_cast<cExprNode*>(GetChild(2));
            cDeclNode * left = lExpr->GetType();
            cDeclNode * right = rExpr->GetType();

            if(left->IsFloat() || right->IsFloat())
            {
                return g_symbolTable.Find("float")->getDecl();
            }
            else if (
                left->IsChar() || right->IsChar() ||
                ( 
                    (!lExpr->IsVar() && lExpr->IsChar()) &&
                    (!rExpr->IsVar() && rExpr->IsChar()) 
                )
            )
            {
                return g_symbolTable.Find("char")->getDecl();
            }
            else 
            {
                return g_symbolTable.Find("int")->getDecl();
            }
        }

        virtual bool IsVar() 
        {
            cExprNode * left = dynamic_cast<cExprNode*>(GetChild(0));
            cExprNode * right = dynamic_cast<cExprNode*>(GetChild(2));
            if (left->IsVar() || right->IsVar())
                return true;
            return false;
        }
        virtual string NodeType() { return string("expr"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};