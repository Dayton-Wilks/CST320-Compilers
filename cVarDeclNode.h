#pragma once
//**************************************
// cVarDeclNode.h
//
// Defines an AST node for a variable declaration, inherits from cDeclnNode 
// so a var delc can be used anywhere a decl can be made.
//
// Author: Dayton Wilks
//

#include "cAstNode.h"
#include "cDeclNode.h"

class cVarDeclNode : public cDeclNode
{
    public:
        cVarDeclNode(cSymbol * sym) : cDeclNode()
        {
            AddChild(sym);
        }

        void Insert(cSymbol *decl)
        {
            AddChild(decl);
        }

        virtual string NodeType() { return string("var_decl"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};