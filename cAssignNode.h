#pragma once
//**************************************
// cAssignNode.h
//
// Class for an assignment operation
//
// Author: Dayton Wilks
// Date: 2/8/18
#include "cAstNode.h"

class cAssignNode : public cStmtNode
{
public:
    cAssignNode(cVarExprNode * left, cExprNode * right) : cStmtNode() 
    {
        IsAssignable(left, right);
        AddChild(left);
        AddChild(right);
    }
    
    virtual string NodeType() { return string("assign"); }
    virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
private:
    bool IsAssignable(cVarExprNode * left, cExprNode * right)
    {
        if (left == nullptr || right == nullptr) return false;
        
        cDeclNode * leftType = left->GetType();
        cDeclNode * rightType = right->GetType();

        if(leftType != nullptr && rightType != nullptr)
        {
            string err;
            if(leftType->IsChar() && rightType->IsFloat())
            {
                err = "Cannot assign float to char ";
                SemanticError(err);
            }
            else if(leftType->IsChar() && rightType->IsInt())
            {
                if (!right->IsVar())
                {
                    cIntExprNode * num = (static_cast<cIntExprNode*>(right));
                    if (num->IsChar())
                    {
                        return true;
                    }
                }
                err = "Cannot assign int to char ";
                SemanticError(err);
            }
            else if(leftType->IsInt() && rightType->IsFloat())
            {
                err = "Cannot assign float to int ";
                SemanticError(err);
            }
            else if (leftType->IsStruct() || rightType->IsStruct())
            {
                string rightTypeName = dynamic_cast<cVarDeclNode*>(rightType)->GetType()->getDecl()->GetName();
                    string leftTypeName = dynamic_cast<cVarDeclNode*>(leftType)->GetType()->getDecl()->GetName();

                if (rightTypeName != leftTypeName)
                {
                    err = "Cannot assign ";
                    err += rightTypeName + " to ";
                    err += leftTypeName + " ";
                    SemanticError(err);
                }
            }
            else
            {
                return true;
            }
        }
        return false;
    }
};