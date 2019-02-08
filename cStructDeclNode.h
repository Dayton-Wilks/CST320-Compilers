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
        }
        
        virtual string NodeType() { return string("struct_decl"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};