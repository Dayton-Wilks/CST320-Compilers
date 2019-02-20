#pragma once
//**************************************
// cFuncDeclNode.h
//
// Creates a function declaration
//
// Author: Dayton Wilks
//

#include "cAstNode.h"
#include "cDeclNode.h"
//#include "cSymbol.h"

class cFuncDeclNode : public cDeclNode
{
    public:
        // param is the first decl in this decls
        cFuncDeclNode(cSymbol * type, cSymbol * name) : cDeclNode()
        {
            AddChild(type);
            AddChild(name);
            name->setDecl(this);
            g_symbolTable.Insert(name);
        }

        // Add a decl to the list
        void Insert(cAstNode * node)
        {
            AddChild(node);
        }

        virtual cDeclNode * GetType()
        {
            cSymbol * t = dynamic_cast<cSymbol*>(GetChild(0));
            return t->getDecl();
        }
        virtual string GetName()
        {
            cSymbol * sym = dynamic_cast<cSymbol*>(GetChild(1));
            return sym->GetName();
        }

        virtual bool IsFunc() { return true; }

        virtual string NodeType() { return string("func"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};