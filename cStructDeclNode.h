#pragma once
//**************************************
// cStructDeclNode.h
//
// Defines an AST node for an struc declaration.
//
// Inherits from cDeclNode so that the struct decl can be used anywhere 
// declarations are made.
//
// Author: Dayton Wilks
//

#include "cAstNode.h"
#include "cDeclNode.h"

class cStructDeclNode : public cDeclNode
{
    public:
        cStructDeclNode(cDeclsNode * members, cSymbol * name) : cDeclNode() 
        {
            AddChild(members);
            AddChild(name);
            name->setDecl(this);
            g_symbolTable.Insert(name);
            
            //fprintf(stderr, "StrucDecl Name:<%s> this:<%i> NamPtr:<%i>\n", GetName().c_str(), this, name);
        }

        virtual cDeclNode * GetType() 
        {
            return this;
        }

        virtual string GetName()
        {
            cSymbol * sym = dynamic_cast<cSymbol*>(GetChild(1));
            return sym->GetName();
        }

        virtual bool IsStruct() { return true; }
        
        virtual string NodeType() { return string("struct_decl"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};