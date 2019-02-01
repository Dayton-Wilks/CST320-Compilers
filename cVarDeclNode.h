#pragma once
//**************************************
// cDeclsNode.h
//
// 
//
// Author: Dayton Wilks
//

#include "cAstNode.h"
#include "cDeclNode.h"
#include "cSymbol.h"

class cVarDeclNode : public cDeclNode
{
    public:
        // param is the first decl in this decls
        cVarDeclNode(cSymbol * sym) : cDeclNode()
        {
            AddChild(sym);
        }

        // Add a decl to the list
        void Insert(cSymbol *decl)
        {
            AddChild(decl);
        }

        virtual string NodeType() { return string("var_decl"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};