#pragma once
//**************************************
// cAssignNode.h
//
// Defines AST node for assignment statments
//
// Author: Phil Howard 
// phil.howard@oit.edu
//
// Date: Nov. 28, 2015
//

#include "cAstNode.h"
#include "cStmtNode.h"
#include "cExprNode.h"
#include "cVarExprNode.h"

class cAssignNode : public cStmtNode
{
    public:
        // params are the lval and the expression
        cAssignNode(cVarExprNode *lval, cExprNode *expr)
            : cStmtNode()
        {
            AddChild(lval);
            AddChild(expr);

            if (!lval->GetDecl()->IsVar())
            {
                SemanticError(lval->GetDecl()->GetName() +
                        " is not an lval");
            }
            else if (!lval->GetType()->IsCompatibleWith(expr->GetType()))
            {
                SemanticError("Cannot assign " +
                        expr->GetType()->GetName() +
                        " to " +
                        lval->GetType()->GetName());
            }
        }

        virtual string NodeType() { return string("assign"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
