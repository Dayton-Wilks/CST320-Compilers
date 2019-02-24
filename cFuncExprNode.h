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
            cDeclNode * decl = name->getDecl();
            cFuncDeclNode* fdecl= dynamic_cast<cFuncDeclNode*>(decl);

            if (!decl)
            {
                SemanticError(name->GetName() + " is not declared ");
            }
            else if (!decl->IsFunc())
            {
                SemanticError(name->GetName() + " is not a function ");
            }
            else if (!fdecl->HasBody())
            {
                SemanticError(name->GetName() + " is not fully defined ");
            }
            else if (
                (fdecl->GetParams() == nullptr && params != nullptr) ||
                (fdecl->GetParams() != nullptr && params == nullptr) ||
                (fdecl->GetParams() != 0 && params == nullptr) ||
                (params != nullptr && fdecl->GetParams() != nullptr && fdecl->GetParams()->ChildCount() != params->ChildCount()))
            {
                SemanticError(name->GetName() + " called with wrong number of arguments ");
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