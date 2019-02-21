#pragma once
//**************************************
// cFuncExprNode.h
//
// Defines an AST node for an function.
//
// Inherits from cExprNode so that function can be used anywhere 
// expressions are used.
//
// Author: Dayton Wilks
//

#include "cAstNode.h"
#include "cExprNode.h"
#include "cSymbolTable.h"

class cFuncExprNode : public cExprNode
{
    public:
        // param is the value of the integer constant
        cFuncExprNode(cSymbol * name, cParamListNode * params) : cExprNode()
        {
            if (!name->getDecl())
            {
                SemanticError(name->GetName() + " is not declared ");
                CHECK_ERROR();
            }
            else if (!name->getDecl()->IsFunc())
            {
                SemanticError(name->GetName() + " is not a function ");
                CHECK_ERROR();
            }
            AddChild(name);
            AddChild(params);
        }

        virtual cDeclNode * GetType()
        {
            cSymbol * sym = dynamic_cast<cSymbol*>(GetChild(0));
            cFuncDeclNode* decl =  dynamic_cast<cFuncDeclNode*>(sym->getDecl());
            return decl->GetType();
        }
        virtual bool IsVar() { return true; }
        virtual bool IsChar() { return GetType()->IsChar(); }
        virtual string NodeType() { return string("funcCall"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};